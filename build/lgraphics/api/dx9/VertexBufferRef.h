#ifndef INC_LGRAPHICS_DX9_VERTEXBUFFERREF_H__
#define INC_LGRAPHICS_DX9_VERTEXBUFFERREF_H__
/**
@file VertexBufferRef.h
@author t-sakai
@date 2009/01/18 create
@data 2010/01/08 add VertexBufferUP
*/
#include <lcore/NonCopyable.h>
#include "../../lgraphicscore.h"
#include "Enumerations.h"

struct IDirect3DVertexBuffer9;

namespace lgraphics
{

    struct VertexBufferDesc
    {
        BufferFormat format_;
        ResourceType type_;
        u32 usage_;
        Pool pool_;
        u32 size_;
        u32 fvf_;
    };


    //------------------------------------------------------------
    //---
    //--- VertexBufferRef
    //---
    //------------------------------------------------------------
    class VertexBufferRef
    {
    public:
        typedef VertexBufferDesc BufferDesc;

        VertexBufferRef()
            :vertexSize_(0)
            ,vertexNum_(0)
            ,offset_(0)
            ,buffer_(NULL)
        {
        }

        VertexBufferRef(const VertexBufferRef& rhs);

        ~VertexBufferRef()
        {
            destroy();
        }

        bool getDesc(BufferDesc& desc);

        VertexBufferRef& operator=(const VertexBufferRef& rhs)
        {
            VertexBufferRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        bool valid() const{ return (buffer_ != NULL); }

        void destroy();

        bool lock(u32 offset, u32 size, void** data, Lock lock=Lock_None);
        void unlock();

        /*
        @brief データ転送。GL的インターフェイス
        */
        void blit(void* data, bool )
        {
            blit(data, 0, vertexSize_*vertexNum_);
        }

        // データ転送
        void blit(const void* data, u32 offset, u32 size);

        u32 getVertexSize() const{ return vertexSize_;}
        u32 getVertexNum() const{ return vertexNum_;}

        u32 getOffset() const{ return offset_;}
        void setOffset(u32 offset){ offset_ = offset;}

        void attach(u32 stride) const;
        void attach(u32 stride, u32 stream) const;

        void swap(VertexBufferRef& rhs)
        {
            lcore::swap(vertexSize_, rhs.vertexSize_);
            lcore::swap(vertexNum_, rhs.vertexNum_);
            lcore::swap(offset_, rhs.offset_);
            lcore::swap(buffer_, rhs.buffer_);
        }

    private:
        friend class VertexBuffer;

        VertexBufferRef(u32 vertexSize, u32 vertexNum, IDirect3DVertexBuffer9* buffer)
            :vertexSize_(vertexSize)
            ,vertexNum_(vertexNum)
            ,offset_(0)
            ,buffer_(buffer)
        {
        }

        u32 vertexSize_;
        u32 vertexNum_;
        u32 offset_;
        IDirect3DVertexBuffer9 *buffer_;
    };


    //------------------------------------------------------------
    //---
    //--- VertexBuffer
    //---
    //------------------------------------------------------------
    class VertexBuffer
    {
    public:
        static VertexBufferRef create(u32 vertexSize, u32 vertexNum, Pool poolType, Usage usage);

    };


    //------------------------------------------------------------
    //---
    //--- VertexBufferUPRef
    //---
    //------------------------------------------------------------
    class VertexBufferUP;

    class VertexBufferUPRef
    {
    public:
        typedef VertexBufferDesc BufferDesc;


        VertexBufferUPRef()
            :buffer_(NULL)
        {
        }

        VertexBufferUPRef(const VertexBufferUPRef& rhs);

        ~VertexBufferUPRef()
        {
            destroy();
        }

        VertexBufferUPRef& operator=(const VertexBufferUPRef& rhs)
        {
            VertexBufferUPRef tmp(rhs);
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

        void swap(VertexBufferUPRef& rhs)
        {
            lcore::swap(buffer_, rhs.buffer_);
        }

    private:
        friend class VertexBufferUP;

        explicit VertexBufferUPRef(VertexBufferUP* buffer)
            :buffer_(buffer)
        {
        }

        VertexBufferUP *buffer_;
    };


    //------------------------------------------------------------
    //---
    //--- VertexBufferUP
    //---
    //------------------------------------------------------------
    class VertexBufferUP : private lcore::NonCopyable<VertexBufferUP>
    {
    public:
        static VertexBufferUPRef create(u32 vertexSize, u32 vertexNum);

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

        u32 getVertexSize() const{ return vertexSize_;}
        u32 getVertexNum() const{ return vertexNum_;}

        void swap(VertexBufferUP& rhs)
        {
            lcore::swap(buffer_, rhs.buffer_);
            lcore::swap(vertexSize_, rhs.vertexSize_);
            lcore::swap(vertexNum_, rhs.vertexNum_);
            lcore::swap(count_, rhs.count_);
        }

    private:
        VertexBufferUP()
            :buffer_(NULL)
            ,vertexSize_(0)
            ,vertexNum_(0)
            ,count_(0)
        {
        }

        VertexBufferUP(u8* buffer, u32 vertexSize, u32 vertexNum)
            :buffer_(buffer)
            ,vertexSize_(vertexSize)
            ,vertexNum_(vertexNum)
            ,count_(0)
        {
        }

        ~VertexBufferUP();

        u8 *buffer_;
        u32 vertexSize_;
        u32 vertexNum_;

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
    inline void swap<lgraphics::VertexBufferRef>(lgraphics::VertexBufferRef& l, lgraphics::VertexBufferRef& r)
    {
        l.swap(r);
    }

    template<>
    inline void swap<lgraphics::VertexBufferUPRef>(lgraphics::VertexBufferUPRef& l, lgraphics::VertexBufferUPRef& r)
    {
        l.swap(r);
    }
}
#endif //INC_LGRAPHICS_DX9_VERTEXBUFFERREF_H__
