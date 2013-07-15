#ifndef INC_LGRAPHICS_IOPNG_H__
#define INC_LGRAPHICS_IOPNG_H__
/**
@file IOPNG.h
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
    class IOPNG
    {
    public:
        static bool read(lcore::istream& is, u8** ppBuffer, u32& width, u32& height, BufferFormat& format);
        static bool read(lcore::istream& is, TextureRef& texture);
    };
}
}
#endif //INC_LGRAPHICS_IOPNG_H__
