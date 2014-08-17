#ifndef INC_LCORE_MEMORYALLOCATOR_H__
#define INC_LCORE_MEMORYALLOCATOR_H__
/**
@file MemoryAllocator.h
@author t-sakai
@date 2014/03/24 create
*/
#include "../lcore.h"

namespace lcore
{
    class MemoryAllocator
    {
    public:
#ifdef _DEBUG
#define LCORE_MEMORYALLOCATOR_CHECK
#endif
        typedef lcore::uintptr_t uintptr_t;
        typedef u32 size_type;
        typedef u32 bitmap_type;
        typedef u32 flag_type;
        typedef u8* pointer_type;
        typedef u32 index_type;
        typedef u32 bitmap_type;

        MemoryAllocator();
        ~MemoryAllocator();

        void* allocate(size_type size);
        void deallocate(void* ptr);

    public:
        static const u32 ExternalMallocAlign = (1U<<3);
        static const u32 ExternalAlignMask = (ExternalMallocAlign - 1U);

        static const u32 MallocAlign = (1U<<4);
        static const u32 ChunkAlignMask = (MallocAlign - 1U);
        static const u32 ChunkOverhead = sizeof(size_type) * 2;

        static const u32 NumSmallBins = 32U;
        static const u32 NumTreeBins = 32U;
        static const u32 SmallBinShift = 4U;
        static const u32 LargeBinShift = 9U;
        static const u32 MaxAllocateShift = 18U;

        static const u32 LargeBinAlign = (1U<<LargeBinShift);
        static const u32 LargeSizeMask = LargeBinAlign - 1;

        static const u32 MinLargeSize = (1U<<LargeBinShift);
        static const u32 MaxSmallSize = (MinLargeSize - 1U);
        static const u32 MaxSmallRequest = (MaxSmallSize - ChunkOverhead);

        static const u32 MinAllocateSize = (1U<<MaxAllocateShift);
        static const u32 MaxLargeSize = (MinAllocateSize - 1U - LargeBinAlign);
        static const u32 MaxLargeRequest = (MaxLargeSize - ChunkOverhead - MallocAlign*2);

        static const u32 MinExternalSize = (MinAllocateSize - LargeBinAlign);

        static const u32 LargePageSize = MinAllocateSize;

        static const u32 PageSize = 64*1024;

        static u32 PageBankSize;// = 4*1024;

    private:
        static inline index_type getSmallIndex(size_type size)
        {
            return (size>>SmallBinShift);
        }

        static inline size_type getSmallSize(index_type index)
        {
            return (index << SmallBinShift);
        }

        static inline size_type paddingRequest(size_type size)
        {
            return (size + ChunkOverhead + ChunkAlignMask) & ~ChunkAlignMask;
        }

        static inline size_type paddingLargeRequest(size_type size)
        {
            return (size + ChunkOverhead + LargeSizeMask) & ~LargeSizeMask;
        }

        static inline size_type paddingExternalRequest(size_type size)
        {
            return (size + ChunkOverhead + ExternalAlignMask) & ~ExternalAlignMask;
        }

        inline void setBinMap(index_type index)
        {
            LASSERT(0<index);
            binMap_ |= (0x01U<<(index-1));
        }

        inline void resetBinMap(index_type index)
        {
            LASSERT(0<index);
            binMap_ &= ~(0x01U<<(index-1));
        }

        struct ChunkBase
        {
            static const flag_type Flag_InUse = 0x01U;
            static const flag_type Flags = (Flag_InUse);

            inline ChunkBase()
                :prevSize_(0)
                ,size_(0)
            {}

            inline size_type getPrevSize() const
            {
                return prevSize_;
            }

            inline void setPrevSize(size_type size)
            {
                prevSize_ = size;
            }

            inline size_type getSize() const
            {
                return size_ & ~Flags;
            }

            inline void setSize(size_type size)
            {
                size_ = size;
            }

            inline void setSizeWithFlag(size_type size)
            {
                size_ = size | (size_&Flags);
            }

            inline void setSizeInUse(size_type size)
            {
                size_ = size | Flag_InUse;
            }

            inline void clearFlags()
            {
                size_ &= ~Flags;
            }

            inline bool isInUse() const
            {
                return 0 != (size_ & Flag_InUse);
            }

            inline void setInUse()
            {
                size_ |= Flag_InUse;
            }

            inline void resetInUse()
            {
                size_ &= ~Flag_InUse;
            }

            inline void* getMemory()
            {
                return (void*)((pointer_type)this + ChunkOverhead);
            }

            static inline ChunkBase* getChunk(void* ptr)
            {
                return (ChunkBase*)((pointer_type)ptr - ChunkOverhead);
            }

            inline ChunkBase* getPrev()
            {
                return (ChunkBase*)((pointer_type)this - prevSize_);
            }

            inline ChunkBase* getNext()
            {
                return (ChunkBase*)((pointer_type)this + getSize());
            }

            /**
            @brief Œã”ösizeƒoƒCƒg‚ð•ªŠ„
            @return •ªŠ„‚µ‚½Chunk
            */
            ChunkBase* splitBack(size_type size);

            size_type prevSize_;
            size_type size_;
        };

        struct Chunk : public ChunkBase
        {
            inline void reset()
            {
                prev_ = next_ = this;
            }

            inline void unlink()
            {
                next_->prev_ = prev_;
                prev_->next_ = next_;
                prev_ = next_ = this;
            }

            inline void link(Chunk* chunk)
            {
                prev_ = chunk->prev_;
                next_ = chunk;
                prev_->next_ = chunk->prev_ = this;
            }

            inline bool empty() const
            {
                return (this == next_);
            }

            Chunk* prev_;
            Chunk* next_;
        };


        static const size_type MinChunkSize = sizeof(Chunk);
        static const size_type MinRequestSize = MinChunkSize - ChunkOverhead;

        static inline void* allocmmap(size_type size);
        static inline void freemmap(void* ptr);

        struct Page
        {
            inline void reset(size_type size, void* memory)
            {
                prev_ = next_ = this;
                size_ = size - MallocAlign*2;
                memory_ = (pointer_type)memory;

                Chunk* chunk = (Chunk*)memory_;
                chunk->setSizeInUse(MallocAlign);
                chunk->setPrevSize(0);

                chunk = (Chunk*)(memory_ + size - MallocAlign);
                chunk->setSizeInUse(MallocAlign);
                chunk->setPrevSize(size_);

                memory_ += MallocAlign;
                chunk = (Chunk*)memory_;
                chunk->setSizeInUse(size_);
                chunk->setPrevSize(MallocAlign);
            }

            inline void unlink()
            {
                next_->prev_ = prev_;
                prev_->next_ = next_;
                prev_ = next_ = this;
            }

            inline void link(Page* chunk)
            {
                prev_ = chunk->prev_;
                next_ = chunk;
                LASSERT(NULL != prev_);
                LASSERT(NULL != next_);
                prev_->next_ = chunk->prev_ = this;
            }

            template<class T>
            inline T* splitMemory(size_type size)
            {
                LASSERT(size<=size_);

                T* chunk = (T*)(memory_ + size_);
                chunk->setPrevSize(size);

                size_ -= size;

                chunk = (T*)(memory_);
                chunk->setSizeInUse(size_);

                chunk = (T*)(memory_ + size_);
                chunk->setSize(size);
                if(0<size_){
                    chunk->setPrevSize(size_);
                }
                return chunk;
            }

            inline void* getRawMemory()
            {
                return (memory_ - MallocAlign);
            }

            Page* prev_;
            Page* next_;
            size_type size_;
            pointer_type memory_;
        };

        struct PageBank
        {
            PageBank* next_;
            u32 maxPages_;
            u32 numPages_;
        };

        struct TreeChunk : public ChunkBase
        {
            static const flag_type FlagShift = 2U;
            static const flag_type Flag_BinIndex = 0x03U;

            TreeChunk();

            inline void resetAllLink();

            inline void setBinIndex(index_type binIndex)
            {
                parent_ = (TreeChunk*)(Flag_BinIndex | (binIndex<<FlagShift));
            }

            inline index_type getBinIndex() const
            {
                return ((index_type)parent_ >> FlagShift);
            }

            inline bool haveBinIndex() const
            {
                return ((index_type)parent_ & Flag_BinIndex) == Flag_BinIndex;
            }

            inline bool empty() const
            {
                return (this == next_);
            }

            void reset();
            void unlink();
            void link(TreeChunk* treeChunk);
            void unlinkFromParent();
            void copyBlood(TreeChunk* from);

            TreeChunk* getNextChild();
            TreeChunk* getPrevChild();

            TreeChunk* prev_;
            TreeChunk* next_;
            TreeChunk* parent_;
            TreeChunk* child_[2];
        };


        struct BitwiseTrie
        {
            static const flag_type Flag_End = 0x01U;
            static const u32 NumTreeBins = 32U;

            inline BitwiseTrie();
            inline ~BitwiseTrie();
            static inline index_type getBinIndex(size_type size);
            static inline size_type getChildIndex(size_type size, size_type keybit);

            inline size_type getTreeKeyBit(index_type index) const;
            inline void setTreeMap(size_type keybit);
            inline void resetTreeMap(size_type keybit);
            inline bool isSetTreeMap(size_type keybit) const;

            void insert(TreeChunk* newChunk);
            TreeChunk* find(size_type size);
            TreeChunk* findLarger(size_type size);
            bool contains(const TreeChunk* target) const;

            void remove(TreeChunk* chunk);

            size_type treeMap_;
            TreeChunk treeBins_[NumTreeBins];
        };

        inline Chunk* combinePrevious(Chunk* chunk);
        inline Chunk* combineAround(Chunk* chunk);

        inline TreeChunk* combinePrevious(TreeChunk* chunk);
        inline TreeChunk* combineAround(TreeChunk* chunk);

        Chunk* allocateChunk(size_type size);
        TreeChunk* allocateTreeChunk(size_type size);

        Page* allocatePage(size_type size);

        PageBank* createPageBank();

        inline bitmap_type getSmallBitmap(index_type index) const;
        static inline bool isInUseSmallBitmap(bitmap_type bitmap);

        PageBank* pageBank_;
        Page page_;
        Page largePage_;
        Page* allocatedPage_;

        size_type binMap_;
        Chunk smallBins_[NumSmallBins+1];

        BitwiseTrie largeTree_;

        void setMagic(void* ptr, size_type size);
        bool checkMagic(void* ptr, size_type size);
    };
}
#endif //INC_LCORE_MEMORYALLOCATOR_H__
