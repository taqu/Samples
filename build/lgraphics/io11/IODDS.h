#ifndef INC_LGRAPHICS_IO11_IODDS_H__
#define INC_LGRAPHICS_IO11_IODDS_H__
/**
@file IODDS.h
@author t-sakai
@date 2012/07/26 create
*/
#include "../lgraphicscore.h"
#include "../api/Enumerations.h"

namespace lgraphics
{
    class Texture2DRef;

namespace io
{
    class IODDS
    {
    public:
        static const u32 DDS_MAGIC = LIME_MAKE_FOURCC('D', 'D', 'S', ' ');
        static const u32 DDS_DX10 = LIME_MAKE_FOURCC( 'D', 'X', '1', '0' );

        struct DDS_PIXELFORMAT
        {
            u32 size_; //==32
            u32 flags_;//
            u32 fourCC_;
            u32 RGBBitCount_;
            u32 RBitMask_;
            u32 GBitMask_;
            u32 BBitMask_;
            u32 ABitMask_;
        };

        static const u32 DDS_HEADER_SIZE = 124; //アライメントチェックも兼ねて固定
        struct DDS_HEADER
        {
            u32 size_;
            u32 flags_;
            u32 height_;
            u32 width_;
            u32 linearSize_; //The number of bytes per scan line in uncompressed.
                             //Total number of bytes in top level in compressed. Pitch must be 4 byte aligned.
            u32 depth_; //depth of volume TextureRef
            u32 mipmap_;
            u32 reserved_[11];
            DDS_PIXELFORMAT ddpf_;
            u32 caps_;
            u32 caps2_;
            u32 caps3_;
            u32 caps4_;
            u32 reserved2_;
        };


        static bool read(Texture2DRef& texture, const s8* data, u32 size, Usage usage, TextureFilterType filter, TextureAddress adress);

    };
}
}
#endif //INC_LGRAPHICS_IO11_IODDS_H__
