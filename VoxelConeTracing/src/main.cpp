#include "stdafx.h"
#include "Application.h"

#include <lcore/lcore.h>
#include <lcore/ldll.h>
#include <lgraphics/lgraphics.h>
#include <lgraphics/api/InitParam.h>

#include <lmath/lmath.h>


int APIENTRY WinMain(HINSTANCE hInstance,
                       HINSTANCE /*hPrevInstance*/,
                       LPTSTR    /*lpCmdLine*/,
                       int       /*nCmdShow*/)
{
    lcore::f32 theta = 10.0f*DEG_TO_RAD;
    lcore::f32 phi = 0.0f;
    for(int i=0; i<3; ++i){
        phi = 120.0f*i*DEG_TO_RAD;
        lmath::Vector3 v;
        v.z_ = lmath::cosf(theta);
        v.x_ = lmath::cosf(phi) * v.z_;
        v.y_ = lmath::sinf(phi) * v.z_;
        v.normalize();

        lcore::Log("float4(%f, %f, %f, %f * SWEIGHT),", v.x_, v.y_, v.z_, lmath::cosf(theta)/PI);
    }

    lcore::System system;

    fractal::Application myApp;

    {
        lgraphics::InitParam param;

        param.swapEffect_ = lgraphics::DXGISwapEffect_Discard;
        param.interval_ = lgraphics::PresentInterval_One;

        param.backBufferWidth_ = 1280;
        param.backBufferHeight_ = 1024;
        param.format_ = lgraphics::Data_R8G8B8A8_UNorm_SRGB;
        param.refreshRate_ = 60;
        param.windowed_ = 1;
        param.allowSoftwareDevice_ = 0;
        param.supportHardwareLevel_ = lgraphics::InitParam::HardwareLevel_11;
        param.flags_ = lgraphics::InitParam::Flag_DepthStencilShaderResource;

        if(false == myApp.create(param, "VoxelConeTracing", fractal::WndProc)){
            return 0;
        }
    }

    myApp.run();
    return 0;
}
