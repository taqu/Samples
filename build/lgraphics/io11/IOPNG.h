#ifndef INC_LGRAPHICS_IOPNG_H__
#define INC_LGRAPHICS_IOPNG_H__
/**
@file IOPNG.h
@author t-sakai
@date 2013/07/24 create
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
        /**
        @brief buffer�Ƀ��[�h�Bbuffer��NULL�̏ꍇ�Awidth�Aheight�Aformat�ArowBytes��ݒ肵�ĕԂ�
        */
        static bool read(lcore::istream& is, u8* buffer, u32& width, u32& height, u32& rowBytes, DataFormat& format);
    };
}
}
#endif //INC_LGRAPHICS_IOPNG_H__
