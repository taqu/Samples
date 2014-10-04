#ifndef INC_LGRAPHICS_DX11_CONSTANTBUFFERREF_H__
#define INC_LGRAPHICS_DX11_CONSTANTBUFFERREF_H__
/**
@file ConstantBufferRef.h
@author t-sakai
@date 2012/07/16 create
*/
#include "BufferRefBase.h"

namespace lgraphics
{
    struct Box;

    //------------------------------------------------------------
    //---
    //--- ConstantBufferRef
    //---
    //------------------------------------------------------------
    class ConstantBufferRef : public BufferRefBase
    {
    public:
        ConstantBufferRef()
        {}

        ConstantBufferRef(const ConstantBufferRef& rhs)
            :BufferRefBase(rhs)
        {}

        ~ConstantBufferRef()
        {}

        ConstantBufferRef& operator=(const ConstantBufferRef& rhs)
        {
            ConstantBufferRef tmp(rhs);
            tmp.swap(*this);
            return *this;
        }

        void swap(ConstantBufferRef& rhs)
        {
            BufferRefBase::swap(rhs);
        }

        void setVS(u32 start);
        void setGS(u32 start);
        void setPS(u32 start);
        void setCS(u32 start);

        void update(
            u32 dstSubresource,
            const Box* dstBox,
            const void* srcData,
            u32 srcRowPitch,
            u32 srcDepthPitch);
    private:
        friend class ConstantBuffer;

        ConstantBufferRef(ID3D11Buffer* buffer)
            :BufferRefBase(buffer)
        {}
    };

    //------------------------------------------------------------
    //---
    //--- ConstantBuffer
    //---
    //------------------------------------------------------------
    class ConstantBuffer : public BufferBase
    {
    public:
        static ConstantBufferRef create(
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
                BindFlag_ConstantBuffer,
                accessFlags,
                miscFlags,
                structureByteStride,
                data);

            return ConstantBufferRef(buffer);
        }
    };
}
#endif //INC_LGRAPHICS_DX11_CONSTANTBUFFERREF_H__
