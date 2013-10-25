#include "stdafx.h"
#include "Application.h"

#include <lcore/lcore.h>

#include <lcore/ldll.h>
#include <lgraphics/lgraphics.h>
#include <lgraphics/api/InitParam.h>

void create()
{
    lcore::f32 Theta[] =
    {
        0.0f/180.0f*PI, 10.0f/180.0f*PI, 10.0f/180.0f*PI, 10.0f/180.0f*PI,
        20.0f/180.0f*PI, 20.0f/180.0f*PI, 20.0f/180.0f*PI, 20.0f/180.0f*PI,
    };

    lcore::f32 Phi[] =
    {
        0.0f/180.0f*PI,   0.0f/180.0f*PI, 120.0f/180.0f*PI, 240.0f/180.0f*PI,
        45.0f/180.0f*PI, 135.0f/180.0f*PI, 215.0f/180.0f*PI, 315.0f/180.0f*PI,
    };

    for(int i=0; i<8; ++i){
        lcore::f32 cs0 = lmath::cos(Theta[i]);
        lcore::f32 sn0 = lmath::sin(Theta[i]);

        lcore::f32 cs1 = lmath::cos(Phi[i]);
        lcore::f32 sn1 = lmath::sin(Phi[i]);

        lcore::f32 x = cs0;
        lcore::f32 y = sn0 * cs1;
        lcore::f32 z = sn0 * sn1;
        lcore::f32 w = cs0 / (2.0f*PI*8.0);
        lcore::Log("float4(%f, %f, %f, %f),", x, y, z, w);
    }
}

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
        param.format_ = lgraphics::Data_R8G8B8A8_UNorm;
        param.refreshRate_ = 60;
        param.windowed_ = 1;
        param.allowSoftwareDevice_ = 1;
        param.supportHardwareLevel_ = lgraphics::InitParam::HardwareLevel_11;

        if(false == myApp.create(param, "Voxel", fractal::WndProc)){
            return 0;
        }
    }

    myApp.run();
    return 0;
}
