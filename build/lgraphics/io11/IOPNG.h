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
    class Texture2DRef;

namespace io
{
    class IOPNG
    {
    public:
        enum SwapRGB
        {
            Swap_RGB,
            Swap_BGR,
        };

        static bool checkSignature(lcore::istream& is);

        /**
        @brief buffer�Ƀ��[�h�Bbuffer��NULL�̏ꍇ�Awidth�Aheight�Aformat�ArowBytes��ݒ肵�ĕԂ�
        */
        static bool read(lcore::istream& is, u8* buffer, u32& width, u32& height, u32& rowBytes, DataFormat& format, SwapRGB swap);

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
            SwapRGB swap);
    };
}
}
#endif //INC_LGRAPHICS_IOPNG_H__
