/**
@file MemoryArena.h
@author t-sakai
@date 2012/12/09 create
*/
#include "../lcore.h"

namespace lcore
{
    template<class T, u32 NUM_PER_ARENA, u32 NUM_ARENA>
    class MemoryArena
    {
    public:
        MemoryArena();
        ~MemoryArena();

        T* allocate();

        void deallocate(T* t);

    private:
        union Entry
        {
            Entry* next_;
            u8 t_[sizeof(T)];
        };

        Entry* createArena(u32 index);

        Entry* top_;
        Entry* tops_[NUM_ARENA];
    };

    template<class T, u32 NUM_PER_ARENA, u32 NUM_ARENA>
    MemoryArena<T, NUM_PER_ARENA, NUM_ARENA>::MemoryArena()
    {
        LASSERT(sizeof(T)>sizeof(T*));
        LASSERT(NUM_PER_ARENA>0);
        LASSERT(NUM_ARENA>0);

        for(u32 i=0; i<NUM_ARENA; ++i){
            tops_[i] = NULL;
        }
        top_ = createArena(0);
    }

    template<class T, u32 NUM_PER_ARENA, u32 NUM_ARENA>
    MemoryArena<T, NUM_PER_ARENA, NUM_ARENA>::~MemoryArena()
    {
        for(u32 i=0; i<NUM_ARENA; ++i){
            LIME_DELETE_ARRAY(tops_[i]);
        }
    }

    template<class T, u32 NUM_PER_ARENA, u32 NUM_ARENA>
    T* MemoryArena<T, NUM_PER_ARENA, NUM_ARENA>::allocate()
    {
        if(NULL == top_){
            for(u32 i=0; i<NUM_ARENA; ++i){
                if(NULL == tops_[i]){
                    tops_[i] = createArena(i);
                    top_ = tops_[i];
                    break;
                }
            }
            LASSERT(NULL != top_);
        }
        T* ret = reinterpret_cast<T*>(top_->t_);
        top_ = top_->next_;
        return ret;
    }

    template<class T, u32 NUM_PER_ARENA, u32 NUM_ARENA>
    void MemoryArena<T, NUM_PER_ARENA, NUM_ARENA>::deallocate(T* ptr)
    {
        LASSERT(NULL != ptr);
        
        Entry* top = reinterpret_cast<Entry*>(ptr);
        top->next_ = top_;
        top_ = top;
    }

    template<class T, u32 NUM_PER_ARENA, u32 NUM_ARENA>
    typename MemoryArena<T, NUM_PER_ARENA, NUM_ARENA>::Entry* MemoryArena<T, NUM_PER_ARENA, NUM_ARENA>::createArena(u32 index)
    {
        LASSERT(index<NUM_ARENA);
        LASSERT(tops_[index] == NULL);

        tops_[index] = LIME_NEW Entry[NUM_PER_ARENA];
        for(u32 i=0; i<NUM_PER_ARENA-1; ++i){
            tops_[index][i].next_ = &(tops_[index][i+1]);
        }
        tops_[index][NUM_PER_ARENA-1].next_ = NULL;
        return tops_[index];
    }
}
