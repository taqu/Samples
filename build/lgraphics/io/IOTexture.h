#ifndef INC_LGRAPHICS_IOTEXTURE_H__
#define INC_LGRAPHICS_IOTEXTURE_H__
/**
@file IOTexture.h
@author t-sakai
@date 2010/05/06 create

*/
#include <lcore/liostream.h>

namespace lgraphics
{
    class TextureRef;

namespace io
{
    class IOTexture
    {
    public:
        IOTexture();
        ~IOTexture();

        static bool read(lcore::istream& is, TextureRef& texture);
        static bool write(lcore::ostream& os, TextureRef& texture);

    };
}
}
#endif //INC_LGRAPHICS_IOTEXTURE_H__
