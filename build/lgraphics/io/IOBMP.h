#ifndef INC_LGRAPHICS_IOBMP_H__
#define INC_LGRAPHICS_IOBMP_H__
/**
@file IOBMP.h
@author t-sakai
@date 2010/05/13 create

*/
#include "../lgraphicscore.h"
#include "../api/Enumerations.h"
#include <lcore/liostream.h>

namespace lgraphics
{
    class TextureRef;

namespace io
{
    class IOBMP
    {
    public:
        enum Compression
        {
            Compression_RGB =0,
            Compression_RLE8,
            Compression_RLE4,
            Compression_BitFields,
        };

        static const u16 BMP_MAGIC;// = 'MB';
        static const u32 INFO_SIZE = 40;

        struct BMP_HEADER
        {
            //u16 type_; /// マジック
            u32 size_; /// ファイルサイズ
            u16 reserve1_;
            u16 reserve2_;
            u32 offset_; /// ファイル先頭からデータまでのオフセット

            u32 infoSize_;
            s32 width_;
            s32 height_;
            u16 planes_;
            u16 bitCount_;
            u32 compression_;
            u32 sizeImage_;
            s32 xPixPerMeter_;
            s32 yPixPerMeter_;
            u32 clrUsed_;
            u32 cirImportant_;

        };

        static bool read(lcore::istream& is, u8** ppBuffer, u32& width, u32& height, BufferFormat& format, bool transpose =false);
        static bool read(lcore::istream& is, TextureRef& texture, bool transpose =false);


        static bool write(lcore::ostream& os, const u8* buffer, u32 width, u32 height, BufferFormat format);
    };
}
}
#endif //INC_LGRAPHICS_IOBMP_H__
