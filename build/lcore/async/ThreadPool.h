#ifndef INC_LCORE_THREADPOOL_H__
#define INC_LCORE_THREADPOOL_H__
/**
@file ThreadPool.h
@author t-sakai
@date 2014/04/26 create
*/
#include "Thread.h"
#include "SyncObject.h"

namespace lcore
{
    class ThreadPool
    {
    public:
        static const s32 InvalidJobId = -1;
        typedef void (*JobProc)(u32 threadId, s32 jobId, void* data);

        enum WaitStatus
        {
            WaitStatus_Success = 0,
            WaitStatus_Timeout,
        };

        ThreadPool(s32 maxThreads, s32 maxJobs);
        ~ThreadPool();

        void start();

        s32 add(JobProc proc, void* data);

        WaitStatus waitAllFinish(u32 timeout);
        WaitStatus waitFinish(s32 numJobs, u32 timeout);

        s32 getNumMaxJobs() const{ return maxJobs_;}
        s32 getNumMaxThreads() const{ return maxThreads_;}

        s32 getNumFreeJobs();
        s32 getNumPendingJobs();
        s32 getNumActiveJobs();
        void getNumJobs(s32& numFree, s32& numPending, s32& numActive);

        bool removePendingJob(s32 jobId);
        void clearPendingJobs();

        u32 getThreadId(s32 index) const;
    private:
        ThreadPool(const ThreadPool&);
        ThreadPool& operator=(const ThreadPool&);

        struct Job
        {
            s32 jobId_;
            Job* next_;
            JobProc proc_;
            void* data_;
        };

        class WorkerThread : public ThreadRaw
        {
        public:
            WorkerThread(ThreadPool* threadPool);
            virtual ~WorkerThread();

            ThreadPool* threadPool_;

            static void proc(void* data);
        };

        friend class WorkerThread;

        bool canRun();
        void setCanRun(bool value);

        void waitJobAvailable();

        Job* popJob();
        void pushJob(Job* job);

        Job* popPendingJob();
        void pushPendingJob(Job* job);

        bool canRun_;
        CriticalSection localCS_;
        Semaphore jobSemaphore_;

        s32 maxJobs_;
        s32 numPendigJobs_;
        s32 numActiveJobs_;
        Job* jobs_;
        Job* freeJobs_;
        Job* pendingJobs_;

        s32 maxThreads_;
        WorkerThread** threads_;

    };
}

#endif //INC_LCORE_THREADPOOL_H__
