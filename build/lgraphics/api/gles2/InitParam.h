#ifndef INC_LGRAPHICS_ES2_INITPARAM_H__
#define INC_LGRAPHICS_ES2_INITPARAM_H__
/**
@file InitParam.h
@author t-sakai
@date 2010/05/29 create
@date 2010/12/12 modify for gles2
*/
#include "../../lgraphicscore.h"
#include "Enumerations.h"

namespace lgraphics
{
    struct InitParam
    {
        InitParam()
            :
            format_(Display_R5G6B5),
            backBufferWidth_(1),
            backBufferHeight_(1),
            depthSize_(24),
            stencilSize_(8),
            interval_(PresentInterval_One),
            windowed_(true),
            refreshRate_(60),
            windowHandle_(NULL)
        {
        }

        DisplayFormat format_;
        u32 backBufferWidth_;
        u32 backBufferHeight_;
        s32 depthSize_;
        s32 stencilSize_;
        PresentInterval interval_;
        bool windowed_;
        u32 refreshRate_;
        HWND__ *windowHandle_;
    };
}
#endif //INC_LGRAPHICS_ES2_INITPARAM_H__
