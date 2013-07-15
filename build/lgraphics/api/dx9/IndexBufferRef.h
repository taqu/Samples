#ifndef INC_LGRAPHICS_DX9_INDEXBUFFERREF_H__
#define INC_LGRAPHICS_DX9_INDEXBUFFERREF_H__
/**
@file IndexBufferRef.h
@author t-sakai
@date 2009/05/07 create
@data 2010/01/08 add IndexBufferUPRef
*/
#include <lcore/NonCopyable.h>
#include <lcore/smart_ptr/intrusive_ptr.h>

#include "../../lgraphicscore.h"
#include "Enumerations.h"

struct IDirect3DIndexBuffer9;

namespace lgraphics
{
    struct Face
    {
        u16 indices_[3];
    };

    struct IndexBufferDesc
    {
        BufferFormat format_;
        ResourceType type_;
        u32 usage_;
        Pool pool_;
        u32 size_;
    };

    //------------------------------------------------------------
    //---
    //--- IndexBufferRef
    //---
    //------------------------------------------------------------
    class IndexBufferRef
    {
    public:
        typedef IndexBufferDesc BufferDesc;


        IndexBufferRef()
            :buffer_(NULL)
        {
        }

        IndexBufferRef(const IndexBufferRef& rhs);

        ~IndexBufferRef()
        {
            destroy();
        }

        IndexBufferRef& operator=(const IndexBufferRef& rhs)
        {
            IndexBufferRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        bool valid() const{ return (buffer_ != NULL); }

        void destroy();

        bool lock(u32 offset, u32 size, void** data, Lock lock=Lock_None);
        void unlock();

        // データ転送
        void blit(const void* data, u32 offset, u32 size);

        bool getDesc(BufferDesc& desc);

        void attach() const;

        void swap(IndexBufferRef& rhs)
        {
            lcore::swap(buffer_, rhs.buffer_);
        }
    private:
        friend class IndexBuffer;

        IndexBufferRef(IDirect3DIndexBuffer9* buffer)
            :buffer_(buffer)
        {
        }

        IDirect3DIndexBuffer9 *buffer_;
    };


    //------------------------------------------------------------
    //---
    //--- IndexBuffer
    //---
    //------------------------------------------------------------
    class IndexBuffer
    {
    public:
        static IndexBufferRef create(u32 numIndices, Pool poolType, Usage usage);
    };


    //------------------------------------------------------------
    //---
    //--- IndexBufferUPRef
    //---
    //------------------------------------------------------------
    class IndexBufferUP;

    class IndexBufferUPRef
    {
    public:
        typedef IndexBufferDesc BufferDesc;


        IndexBufferUPRef()
            :buffer_(NULL)
        {
        }

        IndexBufferUPRef(const IndexBufferUPRef& rhs);

        ~IndexBufferUPRef()
        {
            destroy();
        }

        IndexBufferUPRef& operator=(const IndexBufferUPRef& rhs)
        {
            IndexBufferUPRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        bool valid() const{ return (buffer_ != NULL); }

        void destroy();

        bool getDesc(BufferDesc& desc);

        bool lock(void** data);
        bool lock(const void** data) const;

        void unlock(){}
        void unlock() const{}

        void swap(IndexBufferUPRef& rhs)
        {
            lcore::swap(buffer_, rhs.buffer_);
        }

    private:
        friend class IndexBufferUP;

        explicit IndexBufferUPRef(IndexBufferUP* buffer)
            :buffer_(buffer)
        {
        }

        IndexBufferUP *buffer_;
    };

    //------------------------------------------------------------
    //---
    //--- IndexBufferUP
    //---
    //------------------------------------------------------------
    class IndexBufferUP : private lcore::NonCopyable<IndexBufferUP>
    {
    public:
        static IndexBufferUPRef create(u32 numFaces);

        u32 AddRef()
        {
            return (++count_);
        }

        u32 Release()
        {
            if(--count_== 0){
                LIME_DELETE_NONULL this;
            }
            return count_;
        }

        bool lock(void** data)
        {
            LASSERT(data != NULL);
            if(buffer_ != NULL){
                *data = buffer_;
                return true;
            }
            return false;
        }

        bool lock(const void** data) const
        {
            LASSERT(data != NULL);

            if(buffer_ != NULL){
                *data = buffer_;
                return true;
            }
            return false;
        }

        void unlock(){}
        void unlock() const{}

        u32 getNumFaces() const{ return numFaces_;}

        void swap(IndexBufferUP& rhs)
        {
            lcore::swap(buffer_, rhs.buffer_);
            lcore::swap(numFaces_, rhs.numFaces_);
            lcore::swap(count_, rhs.count_);
        }

    private:
        IndexBufferUP()
            :buffer_(NULL)
            ,numFaces_(0)
            ,count_(0)
        {
        }

        IndexBufferUP(u8* buffer, u32 numFaces)
            :buffer_(buffer)
            ,numFaces_(numFaces)
            ,count_(0)
        {
        }

        ~IndexBufferUP();

        u8 *buffer_;
        u32 numFaces_;

        u32 count_;
    };
}

//---------------------------------------------------
//---
//--- lcore::swap特殊化
//---
//---------------------------------------------------
namespace lcore
{
    template<>
    inline void swap<lgraphics::IndexBufferRef>(lgraphics::IndexBufferRef& l, lgraphics::IndexBufferRef& r)
    {
        l.swap(r);
    }

    template<>
    inline void swap<lgraphics::IndexBufferUPRef>(lgraphics::IndexBufferUPRef& l, lgraphics::IndexBufferUPRef& r)
    {
        l.swap(r);
    }
}

#endif //INC_LGRAPHICS_DX9_INDEXBUFFERREF_H__
