#ifndef INC_LCORE_OBJECTPOOL_H__
#define INC_LCORE_OBJECTPOOL_H__
/**
@file ObjectPool.h
@author t-sakai
@date 2014/03/24 create
*/
#include "../lcore.h"

namespace lcore
{
    template<class T, u32 Size>
    class ObjectPool
    {
    public:
        static const u32 ObjectSize = sizeof(T);

        ObjectPool();
        ~ObjectPool();

        T* allocate();
        void deallocate(T* ptr);

        u8* allocateMemory();
        void deallocateMemory(u8* ptr);

        inline bool rest() const;
    private:
        ObjectPool(const ObjectPool&);
        ObjectPool& operator=(const ObjectPool&);

        struct Entry
        {
            union
            {
                Entry* next_;
                u8 object_[ObjectSize];
            };
        };

        Entry* top_;
        Entry entries_[Size];
    };

    template<class T, u32 Size>
    ObjectPool<T,Size>::ObjectPool()
        :top_(NULL)
    {
        for(s32 i=0; i<(Size-1); ++i){
            entries_[i].next_ = &entries_[i+1];
        }
        entries_[Size-1].next_ = NULL;
        top_ = &entries_[0];
    }

    template<class T, u32 Size>
    ObjectPool<T,Size>::~ObjectPool()
    {
    }

    template<class T, u32 Size>
    T* ObjectPool<T,Size>::allocate()
    {
        LASSERT(NULL != top_);
        Entry* ptr = top_;
        top_ = top_->next_;
        return LIME_PLACEMENT_NEW(ptr) T();
    }

    template<class T, u32 Size>
    void ObjectPool<T,Size>::deallocate(T* ptr)
    {
        LASSERT(NULL != ptr);
        LASSERT(entries_<=ptr && ptr<(entries_+Size));

        ptr->~T();
        Entry* top = reinterpret_cast<Entry*>(ptr);
        top->next_ = top_;
        top_ = top;
    }

    template<class T, u32 Size>
    inline bool ObjectPool<T,Size>::rest() const
    {
        return (NULL != top_);
    }

    template<class T, u32 Size>
    u8* ObjectPool<T,Size>::allocateMemory()
    {
        LASSERT(NULL != top_);
        Entry* ptr = top_;
        top_ = top_->next_;
        return reinterpret_cast<u8*>(ptr);
    }

    template<class T, u32 Size>
    void ObjectPool<T,Size>::deallocateMemory(u8* ptr)
    {
        LASSERT(NULL != ptr);
        LASSERT((void*)entries_<=(void*)ptr && (void*)ptr<(void*)(entries_+Size));
        Entry* top = reinterpret_cast<Entry*>(ptr);
        top->next_ = top_;
        top_ = top;
    }
}

#endif //INC_LCORE_OBJECTPOOL_H__
