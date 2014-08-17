#ifndef INC_LCORE_SYNCOBJECT_H__
#define INC_LCORE_SYNCOBJECT_H__
/**
@file SyncObject.h
@author t-sakai
@date 2011/08/06 create
*/
#include "../lcore.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace lcore
{
namespace thread
{
    static const u32 Infinite = 0xFFFFFFFFU; /// タイムアウト時間に指定すると、シグナル状態になるまで待機
    enum WaitStatus
    {
        Wait_Abandoned = WAIT_ABANDONED, /// 所有していたオブジェクトが解放しないで終了した
        Wait_Success = WAIT_OBJECT_0 , /// シグナル状態になった
        Wait_Timeout = WAIT_TIMEOUT , /// タイムアウトした
        Wait_Failed = WAIT_FAILED , /// 関数失敗
    };

    inline void sleep(u32 milliSeconds)
    {
        ::Sleep(milliSeconds);
    }
}

    //-------------------------------------------------------
    //---
    //--- ConditionVariable
    //---
    //-------------------------------------------------------
    /**
    @brief クリティカルセクション
    */
    class CriticalSection
    {
    public:
        static const u32 DefaultSpinCount = 2000;
        explicit CriticalSection(u32 spinCount = DefaultSpinCount)
        {
            InitializeCriticalSectionAndSpinCount(&cs_, spinCount);
        }

        ~CriticalSection()
        {
            DeleteCriticalSection(&cs_);
        }

        void enter()
        {
            EnterCriticalSection(&cs_);
        }

        void leave()
        {
            LeaveCriticalSection(&cs_);
        }

    private:
        CriticalSection(const CriticalSection&);
        CriticalSection& operator=(const CriticalSection&);

        CRITICAL_SECTION cs_;
    };


    /**
    @brief ロックオブジェクト
    */
    template<class T>
    class ScopedLock
    {
    public:
        explicit ScopedLock(T& obj)
            :obj_(obj)
        {
            obj_.enter();
        }

        ~ScopedLock()
        {
            obj_.leave();
        }

    private:
        ScopedLock(const ScopedLock&);
        ScopedLock& operator=(const ScopedLock&);

        T& obj_;
    };

    typedef ScopedLock<CriticalSection> CSLock;

    //-------------------------------------------------------
    //---
    //--- Event
    //---
    //-------------------------------------------------------
#if _WIN32
    class Event
    {
    public:
        Event(bool manualReset, bool initState);
        ~Event();

        thread::WaitStatus wait(u32 timeout)
        {
            return (thread::WaitStatus)WaitForSingleObject(event_, timeout);
        }

        void set()
        {
            SetEvent(event_);
        }

        void reset()
        {
            ResetEvent(event_);
        }
    private:
        Event(const Event&);
        Event& operator=(const Event&);
        HANDLE event_;
    };

#else
    class Event
    {
    public:
        Event(bool manualReset, bool initState);
        ~Event();

        thread::WaitStatus wait(u32 timeout);

        void set();
        void reset();
    private:
        pthread_cond_t condVariable_;
        pthread_mutex_t lock_;
        s16 manualReset_;
        s16 state_;
    };
#endif

    //-------------------------------------------------------
    //---
    //--- ConditionVariable
    //---
    //-------------------------------------------------------
    /**
    @brief 条件変数
    */
    class ConditionVariable
    {
    public:
        ConditionVariable();
        ~ConditionVariable();

        void wait(CriticalSection& externalLock, u32 timeout);
        void broadcast();
        void signal();

    private:
        friend class CondLock;

        ConditionVariable(const ConditionVariable&);
        ConditionVariable& operator=(const ConditionVariable&);

        enum Event
        {
            Event_Signal =0,
            Event_Broadcast = 1,
            Event_Num = 2,
        };

        s32 waitCounter_;
        CriticalSection waitCounterLock_;
        HANDLE events_[Event_Num];
    };


    //-------------------------------------------------------
    //---
    //--- ReadersWriterLock
    //---
    //-------------------------------------------------------
    /**
    @brief Readers/Writer Lock
    */
    class ReadersWriterLock
    {
    public:
        ReadersWriterLock();
        ~ReadersWriterLock();

        void enterReader();
        void leaveReader();

        void enterWriter();
        void leaveWriter();

    private:
        s32 readers_;
        CRITICAL_SECTION csReaders_;
        CRITICAL_SECTION csWrite_;
        HANDLE readersCleared_;
    };

    //-------------------------------------------------------
    //---
    //--- Semaphore
    //---
    //-------------------------------------------------------
    class Semaphore
    {
    public:
        Semaphore(s32 initCount, s32 maxCount);
        ~Semaphore();

        thread::WaitStatus wait(u32 timeout);
        s32 release(s32 count);
    private:
        Semaphore(const Semaphore&);
        Semaphore& operator=(const Semaphore&);

        HANDLE semaphore_;
    };

    //-------------------------------------------------------
    //---
    //--- SpinLock
    //---
    //-------------------------------------------------------
#if defined(__GNUC__)
    class SpinLock
    {
    public:
        SpinLock()
            :value_(0)
        {}

        ~SpinLock()
        {}

        void enter()
        {
            while(__sync_lock_test_and_set(&value_, 1)){
            }
        }

        void leave()
        {
            __sync_lock_release(&value_);
        }

    private:
        s32 value_;
    };

#elif defined(WIN32)
    class SpinLock
    {
    public:
        SpinLock()
            :value_(0)
        {}

        ~SpinLock()
        {}

        void enter()
        {
            while(0 != InterlockedCompareExchange(&value_, 1, 0)){
            }
        }

        void leave()
        {
            InterlockedExchange(&value_, 0);
        }

    private:
        LONG value_;
    };

    typedef ScopedLock<SpinLock> SPLock;
}
#endif

#endif //INC_LCORE_SYNCOBJECT_H__
