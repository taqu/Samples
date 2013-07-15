#ifndef INC_LFRAMEWORK_STATIC_H__
#define INC_LFRAMEWORK_STATIC_H__
/**
@file Static.h
@author t-sakai
@date 2010/05/04 create
*/
namespace lframework
{
    static const int ModelFileMagic = 'lm  ';
    static const int ModelFileVersion = 10;

    enum ImageFormat
    {
        Img_BMP =0,
        Img_TGA,
        Img_DDS,
        Img_PNG,
        Img_JPG,

        Img_SPH,
        Img_SPA,

        Img_Num,
        Img_None = 0xFFU,
    };

    extern char const * ImageFormatExt[Img_Num];
}
#endif //INC_LFRAMEWORK_STATIC_H__
