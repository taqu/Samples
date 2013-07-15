#ifndef INC_LCORE_THREAD_H__
#define INC_LCORE_THREAD_H__
/**
@file Thread.h
@author t-sakai
@date 2011/08/06 create
*/
#include "../lcore.h"
#include "SyncObject.h"

namespace lcore
{
    /**
    @brief プロセッサ数取得
    */
    u32 getNumberOfProcessors();

    //----------------------------------------------------
    //---
    //--- Thread
    //---
    //----------------------------------------------------
    class Thread
    {
    public:
        enum WaitStatus
        {
            Wait_Abandoned = WAIT_ABANDONED, /// 所有していたオブジェクトが解放しないで終了した
            Wait_Success = WAIT_OBJECT_0 , /// シグナル状態になった
            Wait_Timeout = WAIT_TIMEOUT , /// タイムアウトした
            Wait_Failed = WAIT_FAILED , /// 関数失敗
        };

        static const u32 Infinite = 0xFFFFFFFFU; /// タイムアウト時間に指定すると、シグナル状態になるまで待機

        Thread();
        virtual ~Thread();

        /**
        @return 成否
        @brief スレッドを待機状態で作成
        */
        bool create();

        /// スレッドID取得
        u32 getID() const
        {
            return id_;
        }

        /// 開始
        void start();

        /// 再開
        void resume();

        /// 一時停止
        void suspend();

        /// 終了
        void stop();

        /**
        @brief 終了を待つ
        @return 結果
        @param timeout ... タイムアウト時間（ミリ秒）
        */
        WaitStatus join(u32 timeout=Infinite);

    protected:
        /// スレッド終了すべきか
        bool canRun();

        /// 処理本体
        virtual void run() =0;

    private:
        friend class MultipleWait;

        void release();

        static u32 __stdcall proc(void* args);

        Thread(const Thread&);
        Thread& operator=(const Thread&);

        HANDLE handle_;
        u32 id_;
        CriticalSection cs_;
        bool canRun_;
        bool suspend_;
    };

    //
    class MultipleWait
    {
    public:
        static const u32 NumMaxThreads = 32;
        static const u32 Infinite = 0xFFFFFFFFU; /// タイムアウト時間に指定すると、シグナル状態になるまで待機

        enum WaitStatus
        {
            Wait_Abandoned = WAIT_ABANDONED_0, /// 所有していたオブジェクトが解放しないで終了した
            Wait_Success = WAIT_OBJECT_0, /// シグナル状態になった
            Wait_Timeout = WAIT_TIMEOUT, /// タイムアウトした
            Wait_Failed = WAIT_FAILED,
        };

        MultipleWait();
        ~MultipleWait();

        /**
        @brief スレッドセット
        @param index ...
        @param thread ...
        @warn 0からjoinで指定する個数分埋めること
        */
        void set(u32 index, Thread* thread);

        /**
        @brief 終了を待つ
        @return Wait_Success以上（Wait_Success + numThreads - 1）以下：全てシグナル状態、
        @return Wait_Abandoned以上（Wait_Abandoned + numThreads - 1）以下：どれかひとつが解放しないで終了した
        @return Wait_Timeout
        @param timeout ... タイムアウト時間（ミリ秒）
        */
        u32 join(u32 numThreads, u32 timeout=Infinite);
    private:
        MultipleWait(const MultipleWait&);
        MultipleWait& operator=(const MultipleWait&);

        HANDLE handles_[NumMaxThreads];
    };
}

#endif //INC_LCORE_THREAD_H__

