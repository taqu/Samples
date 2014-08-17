#ifndef INC_LCORE_CHUNKALLOCATOR_H__
#define INC_LCORE_CHUNKALLOCATOR_H__
/**
@file ChunkAllocator.h
@author t-sakai
@date 2013/09/19 create
*/
#include "../lcore.h"

namespace lcore
{
    class ChunkAllocator
    {
    public:
#ifdef _DEBUG
#define LCORE_CHUNKALLOCATOR_CHECK
#endif

        static const s32 PageSize = 16*1024;
        static const u32 MinSize = 128;
        static const u32 MaxSize = 1024;

        static const s32 MaxNumBanks= 15;

        static const s32 PageBankSize = 4*1024;

        ChunkAllocator();
        ~ChunkAllocator();

        void* allocate(u32 size);
        void deallocate(void* ptr, u32 size);

        /// 空きページ回収
        void collectEmptyPage();

        /// 割り当てサイズ計算
        static u32 calcAllocSize(u32 size);

    private:
        ChunkAllocator(const ChunkAllocator&);
        ChunkAllocator& operator=(const ChunkAllocator&);

        struct Chunk
        {
            Chunk* next_;
        };

        struct Page
        {
            Page* next_;
            void* chunk_;
            s32 maxChunks_;
            s32 numChunks_;
        };

        struct Bank
        {
            Page* top_;
            Chunk* freeChunk_;
        };

        struct PageBank
        {
            PageBank* next_;
            s32 maxPages_;
            s32 numPages_;
        };

        static u32 calcBankIndex(u32 size);
        static u32 calcBankAllocSize(u32 index);

        static void* allocmmap(s32 size);
        static void freemap(void* ptr);

        Page* createPage(u32 chunkSize);

        void* addCheck(void* ptr);

        Page* getPage();
        PageBank* createPageBank();
        void deallocPage(Page* page);

        // 空きチャンク回収
        void collectFreeChunks(Bank& bank, Page* emptyPage);

        PageBank* pageBank_;
        Page* freePage_;
        Bank banks_[MaxNumBanks];
    };
}
#endif //INC_LCORE_CHUNKALLOCATOR_H__
