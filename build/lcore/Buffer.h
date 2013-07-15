#ifndef INC_LCORE_BUFFER_H__
#define INC_LCORE_BUFFER_H__
/**
@file Buffer.h
@author t-sakai
@date 2009/09/12 create
*/
#include "lcore.h"
#include "utility.h"

namespace lcore
{
    /**
    @brief バッファ管理クラス
    */
    class Buffer
    {
    public:
        static const u32 MemAlignSize = sizeof(f64);

        inline Buffer();
        inline Buffer(u32 size);

        inline ~Buffer();

        inline bool valid() const;

        u32 size() const{ return size_;}
        void resize(u32 size);

        template<class T>
        T* allocate(u32 offset)
        {
            LASSERT(buffer_ != NULL);
            return LIME_PLACEMENT_NEW(&buffer_[offset]) T();
        }

        template<class T>
        void destruct(u32 offset)
        {
            T* t = reinterpret_cast<T*>(&buffer_[offset]);
            t->~T();
        }

        template<class T>
        T* allocate(u32 offset, u32 num);

        template<class T>
        void destruct(u32 offset, u32 num);

        template<class T>
        void destruct(T* t, u32 num);

        template<class T>
        T* get(u32 offset)
        {
            return reinterpret_cast<T*>(buffer_ + offset);
        }

        void swap(Buffer& rhs)
        {
            lcore::swap(size_, rhs.size_);
            lcore::swap(buffer_, rhs.buffer_);
        }

        void destruct();
    private:
        void construct(u32 size);

        u32 size_;
        u8 *buffer_;
    };

    inline Buffer::Buffer()
        :size_(0)
        ,buffer_(NULL)
    {
    }

    inline Buffer::Buffer(u32 size)
        :buffer_(NULL)
    {
        construct(size);
    }

    inline Buffer::~Buffer()
    {
        destruct();
    }

    inline bool Buffer::valid() const
    {
        return (NULL != buffer_);
    }

    template<class T>
    T* Buffer::allocate(u32 offset, u32 num)
    {
        LASSERT(buffer_ != NULL);

        u8* ret = buffer_ + offset;
        u8* t = ret;
        for(u32 i=0; i<num; ++i){
            LIME_PLACEMENT_NEW(t) T();
            t += sizeof(T);
        }
        return reinterpret_cast<T*>(ret);
    }

    template<class T>
    void Buffer::destruct(u32 offset, u32 num)
    {
        T* t = reinterpret_cast<T*>(&buffer_[offset]);
        for(u32 i=0; i<num; ++i, ++t){
            t->~T();
        }
    }

    template<class T>
    void Buffer::destruct(T* t, u32 num)
    {
        LASSERT(t != NULL);
        for(u32 i=0; i<num; ++i, ++t){
            t->~T();
        }
    }
}
#endif //INC_LCORE_BUFFER_H__
