#include "stdafx.h"
#include "Application.h"

#include <lcore/lcore.h>
#include <lcore/ldll.h>
#include <lgraphics/lgraphics.h>
#include <lgraphics/api/InitParam.h>

int APIENTRY WinMain(HINSTANCE hInstance,
                       HINSTANCE /*hPrevInstance*/,
                       LPTSTR    /*lpCmdLine*/,
                       int       /*nCmdShow*/)
{
    lcore::System system;

    fractal::Application myApp;

    {
        lgraphics::InitParam param;

        param.swapEffect_ = lgraphics::DXGISwapEffect_Discard;
        param.interval_ = lgraphics::PresentInterval_One;

        param.backBufferWidth_ = 800;
        param.backBufferHeight_ = 600;
        param.format_ = lgraphics::Data_R8G8B8A8_UNorm_SRGB;
        param.refreshRate_ = 60;
        param.windowed_ = 1;
        param.allowSoftwareDevice_ = 0;
        param.supportHardwareLevel_ = lgraphics::InitParam::HardwareLevel_11;

        if(false == myApp.create(param, "Ocean", fractal::WndProc)){
            return 0;
        }
    }

    myApp.run();
    return 0;
}
