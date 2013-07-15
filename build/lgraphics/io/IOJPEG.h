#ifndef INC_LGRAPHICS_IOJPEG_H__
#define INC_LGRAPHICS_IOJPEG_H__
/**
@file IOJPEG.h
@author t-sakai
@date 2011/01/08 create
*/
#include "../lgraphicscore.h"
#include "../api/Enumerations.h"
#include <lcore/liostream.h>

namespace lgraphics
{
    class TextureRef;

namespace io
{
    class IOJPEG
    {
    public:
        static bool read(const Char* filepath, u8** ppBuffer, u32& width, u32& height, BufferFormat& format);
        static bool read(const Char* filepath, TextureRef& texture);
    };
}
}
#endif //INC_LGRAPHICS_IOJPEG_H__
