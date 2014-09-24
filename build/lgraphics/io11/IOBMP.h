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
    class Texture2DRef;
    struct SRVDesc;

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

        enum WriteFormat
        {
            WriteFormat_RGB,
            WriteFormat_RGBA,
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

        /**
        @brief bufferにロード。bufferがNULLの場合、width、height、formatを設定して返る
        */
        static bool read(lcore::istream& is, u8* buffer, u32& width, u32& height, DataFormat& format, bool transpose = false);

        static bool read(
            Texture2DRef& texture,
            lcore::istream& is,
            Usage usage,
            BindFlag bindFlag,
            CPUAccessFlag access,
            ResourceMisc misc,
            TextureFilterType filter,
            TextureAddress adress,
            CmpFunc compFunc,
            f32 borderColor,
            bool transpose = false);

        static bool write(lcore::ostream& os, const u8* buffer, u32 width, u32 height, WriteFormat format);
    };
}
}
#endif //INC_LGRAPHICS_IOBMP_H__
