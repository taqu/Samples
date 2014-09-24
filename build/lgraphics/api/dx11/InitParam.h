#ifndef INC_LGRAPHICS_DX11_INITPARAM_H__
#define INC_LGRAPHICS_DX11_INITPARAM_H__
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
        enum HardwareLevel
        {
            HardwareLevel_11 = 1,
            HardwareLevel_10_1,
            HardwareLevel_10,
        };

        enum Flag
        {
            Flag_DepthStencilShaderResource = (0x01<<0),
        };

        InitParam()
            :type_(DriverType_Hardware)
            ,backBufferWidth_(1)
            ,backBufferHeight_(1)
            ,format_(Data_R8G8B8A8_UNorm_SRGB)
            ,depthStencilFormat_(Data_D24_UNorm_S8_UInt)
            ,shadowMapFormat_(Data_D16_UNorm)
            ,refreshRate_(60)
            ,windowHandle_(NULL)
            ,windowed_(1)
            ,allowSoftwareDevice_(0)
            ,interval_(PresentInterval_One)
            ,swapEffect_(DXGISwapEffect_Discard)
            ,supportHardwareLevel_(HardwareLevel_10)
            ,flags_(0)
        {
        }

        DriverType type_;
        u32 backBufferWidth_;
        u32 backBufferHeight_;
        u32 format_;
        u32 depthStencilFormat_;
        u32 shadowMapFormat_;
        u32 refreshRate_;
        HWND__ *windowHandle_;
        s16 windowed_;
        s16 allowSoftwareDevice_;
        s32 supportHardwareLevel_;
        u32 flags_;

        PresentInterval interval_;
        DXGISwapEffect swapEffect_;
    };
}

#endif //INC_LGRAPHICS_DX11_INITPARAM_H__
