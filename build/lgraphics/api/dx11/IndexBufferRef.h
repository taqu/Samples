#ifndef INC_LGRAPHICS_DX11_INDEXBUFFERREF_H__
#define INC_LGRAPHICS_DX11_INDEXBUFFERREF_H__
/**
@file IndexBufferRef.h
@author t-sakai
@date 2012/07/15 create
*/
#include "BufferRefBase.h"

namespace lgraphics
{
    //------------------------------------------------------------
    //---
    //--- IndexBufferRef
    //---
    //------------------------------------------------------------
    class IndexBufferRef : public BufferRefBase
    {
    public:
        IndexBufferRef()
        {}

        IndexBufferRef(const IndexBufferRef& rhs)
            :BufferRefBase(rhs)
        {}

        ~IndexBufferRef()
        {}

        IndexBufferRef& operator=(const IndexBufferRef& rhs)
        {
            IndexBufferRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void swap(IndexBufferRef& rhs)
        {
            BufferRefBase::swap(rhs);
        }

        void attach(DataFormat format, u32 offset);
    private:
        friend class IndexBuffer;

        IndexBufferRef(ID3D11Buffer* buffer)
            :BufferRefBase(buffer)
        {}
    };

    //------------------------------------------------------------
    //---
    //--- IndexBuffer
    //---
    //------------------------------------------------------------
    class IndexBuffer : public BufferBase
    {
    public:
        static IndexBufferRef create(
            u32 size,
            Usage usage,
            CPUAccessFlag accessFlags,
            ResourceMisc miscFlags,
            u32 structureByteStride =0,
            const void* data = NULL)
        {
            ID3D11Buffer* buffer = BufferBase::create(
                size,
                usage,
                BindFlag_IndexBuffer,
                accessFlags,
                miscFlags,
                structureByteStride,
                data);

            return IndexBufferRef(buffer);
        }
    };
}
#endif //INC_LGRAPHICS_DX11_INDEXBUFFERREF_H__
