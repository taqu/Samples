#ifndef INC_LGRAPHICS_DX11_VERTEXBUFFERREF_H__
#define INC_LGRAPHICS_DX11_VERTEXBUFFERREF_H__
/**
@file VertexBufferRef.h
@author t-sakai
@date 2012/07/15 create
*/
#include "BufferRefBase.h"

namespace lgraphics
{
    //------------------------------------------------------------
    //---
    //--- VertexBufferRef
    //---
    //------------------------------------------------------------
    class VertexBufferRef : public BufferRefBase
    {
    public:
        VertexBufferRef()
        {}

        VertexBufferRef(const VertexBufferRef& rhs)
            :BufferRefBase(rhs)
        {}

        ~VertexBufferRef()
        {}

        VertexBufferRef& operator=(const VertexBufferRef& rhs)
        {
            VertexBufferRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void swap(VertexBufferRef& rhs)
        {
            BufferRefBase::swap(rhs);
        }

        void attach(u32 startSlot, u32 stride, u32 offsetInBytes);

        void attachStreamOut(u32 offsetInBytes);
    private:
        friend class VertexBuffer;

        VertexBufferRef(ID3D11Buffer* buffer)
            :BufferRefBase(buffer)
        {}
    };

    //------------------------------------------------------------
    //---
    //--- VertexBuffer
    //---
    //------------------------------------------------------------
    class VertexBuffer : public BufferBase
    {
    public:
        static VertexBufferRef create(
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
                BindFlag_VertexBuffer,
                accessFlags,
                miscFlags,
                structureByteStride,
                data);

            return VertexBufferRef(buffer);
        }

        static VertexBufferRef create(
            u32 size,
            Usage usage,
            u32 bindFlag,
            CPUAccessFlag accessFlags,
            ResourceMisc miscFlags,
            u32 structureByteStride,
            const void* data,
            u32 pitch = 0,
            u32 slicePitch = 0)
        {
            ID3D11Buffer* buffer = BufferBase::create(
                size,
                usage,
                bindFlag,
                accessFlags,
                miscFlags,
                structureByteStride,
                data,
                pitch,
                slicePitch);

            return VertexBufferRef(buffer);
        }
    };
}
#endif //INC_LGRAPHICS_DX11_VERTEXBUFFERREF_H__
