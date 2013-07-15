#ifndef INC_LGRAPHICS_ES2_VERTEXBUFFERREF_H__
#define INC_LGRAPHICS_ES2_VERTEXBUFFERREF_H__
/**
@file VertexBufferRef.h
@author t-sakai
@date 2010/06/01 create
*/
#include <lcore/NonCopyable.h>
#include "../../lgraphicscore.h"
#include "Enumerations.h"

struct IDirect3DVertexBuffer9;

namespace lgraphics
{

    struct Descriptor;

    struct VertexBufferDesc
    {
        BufferFormat format_;
        ResourceType type_;
        u32 usage_;
        Pool pool_;
        u32 size_;
    };

    class VertexBufferUPRef;


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
            ,vbID_(NULL)
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

        bool valid() const{ return (vbID_ != NULL); }

        void destroy();

        /*
        @brief データ転送。GL的インターフェイス
        */
        void blit(void* data, bool dynamic = false);

        /*
        @brief データ転送。GL的インターフェイス
        @param data
        @param offset ... バイトオフセット
        @param size ... バイトサイズ
        */
        void blit(void* data, u32 offset, u32 size);

        /**
        @brief バッファデータ取得
        */
        //VertexBufferUPRef retrieveAsUP();

        u32 getVertexSize() const{ return vertexSize_;}
        u32 getVertexNum() const{ return vertexNum_;}

        void attach() const;
        void detach() const;

        void swap(VertexBufferRef& rhs)
        {
            lcore::swap(vertexSize_, rhs.vertexSize_);
            lcore::swap(vertexNum_, rhs.vertexNum_);
            lcore::swap(vbID_, rhs.vbID_);
        }

    private:
        friend class VertexBuffer;

        VertexBufferRef(u32 vertexSize, u32 vertexNum, Descriptor* vbID)
            :vertexSize_(vertexSize)
            ,vertexNum_(vertexNum)
            ,vbID_(vbID)
        {
            LASSERT(vbID_ != NULL);
        }

        u32 vertexSize_;
        u32 vertexNum_;
        Descriptor *vbID_;
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
        static VertexBufferRef create(VertexBufferUPRef& vbUP, bool blit, bool dynamic);

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

    //------------------------------------------------------------
    //---
    //--- VertexBufferUPRef
    //---
    //------------------------------------------------------------
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

        u32 getVertexSize() const
        {
            LASSERT(buffer_ != NULL);
            return buffer_->getVertexSize();
        }

        u32 getVertexNum() const
        {
            LASSERT(buffer_ != NULL);
            return buffer_->getVertexNum();
        }

    private:
        friend class VertexBufferUP;

        explicit VertexBufferUPRef(VertexBufferUP* buffer)
            :buffer_(buffer)
        {
        }

        VertexBufferUP *buffer_;
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

#endif //INC_LGRAPHICS_ES2_VERTEXBUFFERREF_H__
