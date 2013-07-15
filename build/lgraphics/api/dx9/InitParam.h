#ifndef INC_LGRAPHICS_DX9_INITPARAM_H__
#define INC_LGRAPHICS_DX9_INITPARAM_H__
/**
@file InitParam.h
@author t-sakai
@date 2010/03/24 create
*/
#include "../../lgraphicscore.h"
#include "Enumerations.h"

namespace lgraphics
{
    struct InitParam
    {
        InitParam()
            :type_(DriverType_Hardware),
            backBufferWidth_(1),
            backBufferHeight_(1),
            displayFormat_(Buffer_X8R8G8B8),
            refreshRate_(60),
            windowHandle_(NULL),
            windowed_(1),
            interval_(PresentInterval_One),
            swapEffect_(SwapEffect_Discard)
        {
        }

        DriverType type_;
        u32 backBufferWidth_;
        u32 backBufferHeight_;
        u32 displayFormat_;
        u32 refreshRate_;
        HWND__ *windowHandle_;
        s32 windowed_;

        PresentInterval interval_;
        SwapEffect swapEffect_;
    };
}
#endif //INC_LGRAPHICS_DX9_INITPARAM_H__
