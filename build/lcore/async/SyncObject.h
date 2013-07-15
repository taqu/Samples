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
    //--- ConditionVariable
    //---
    //-------------------------------------------------------
    /**
    @brief 条件変数
    */
    class ConditionVariable
    {
    public:
        ConditionVariable(CriticalSection& cs);
        ~ConditionVariable();

    private:
        friend class CondLock;

        ConditionVariable(const ConditionVariable&);
        ConditionVariable& operator=(const ConditionVariable&);

        void notifyAll();
        void wait();

        CriticalSection& external_;// 外部条件変数のロック
        CriticalSection internal_; // 内部変数のロック

        u32 numWaitings_; //待っているスレッド数
        u32 numToWakes_; //起こすスレッド数
        HANDLE signalNotify_; //起こすシグナル
        HANDLE signalWaked_; //起きたシグナル
    };

    /**
    @brief 条件変数用ロックオブジェクト
    */
    class CondLock
    {
    public:
        CondLock(ConditionVariable& cond)
            :cond_(cond)
            ,lock_(cond.external_)
        {}

        ~CondLock()
        {}

        void notifyAll()
        {
            cond_.notifyAll();
        }

        void wait()
        {
            cond_.wait();
        }

    private:
        CondLock(const CondLock&);
        CondLock& operator=(const CondLock&);

        ConditionVariable& cond_;
        CSLock lock_;
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
}

#endif //INC_LCORE_SYNCOBJECT_H__
