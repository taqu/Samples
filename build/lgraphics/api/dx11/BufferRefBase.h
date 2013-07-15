#ifndef INC_LGRAPHICS_DX11_BUFFERREFBASE_H__
#define INC_LGRAPHICS_DX11_BUFFERREFBASE_H__
/**
@file BufferRefBase.h
@author t-sakai
@date 2012/07/15 create
*/
#include "../../lgraphicscore.h"
#include "Enumerations.h"

struct ID3D11Buffer;

namespace lgraphics
{
    struct Box;

    struct MappedSubresource
    {
        void* data_;
        u32 rowPitch_;
        u32 depthPitch_;
    };


    //------------------------------------------------------------
    //---
    //--- BufferRefBase
    //---
    //------------------------------------------------------------
    class BufferRefBase
    {
    public:
        void destroy();

        bool map(u32 subresource, MapType type, MappedSubresource& mapped);
        void unmap(u32 subresource);
        void updateSubresource(u32 index, const Box* box, const void* data, u32 rowPitch, u32 depthPitch);

        bool valid() const{ return buffer_ != NULL;}
    protected:
        BufferRefBase()
            :buffer_(NULL)
        {}

        BufferRefBase(const BufferRefBase& rhs);

        explicit BufferRefBase(ID3D11Buffer* buffer)
            :buffer_(buffer)
        {}

        ~BufferRefBase()
        {
            destroy();
        }

        void swap(BufferRefBase& rhs)
        {
            lcore::swap(buffer_, rhs.buffer_);
        }

        ID3D11Buffer* buffer_;
    };

    //------------------------------------------------------------
    //---
    //--- BufferBase
    //---
    //------------------------------------------------------------
    class BufferBase
    {
    protected:
        static ID3D11Buffer* create(
            u32 size,
            Usage usage,
            BindFlag bindFlags,
            CPUAccessFlag accessFlags,
            ResourceMisc miscFlags,
            u32 structureByteStride =0,
            const void* data = NULL,
            u32 pitch =0,
            u32 slicePitch =0);
    };
}
#endif //INC_LGRAPHICS_DX11_BUFFERREFBASE_H__
