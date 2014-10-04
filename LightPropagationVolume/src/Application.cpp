/**
@file Application.cpp
@author t-sakai
@date 2010/02/14 create
*/
#include "stdafx.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <shellapi.h>

#include "Application.h"

#include <lcore/liostream.h>
#include <lgraphics/api/Enumerations.h>
#include <lgraphics/io11/IOBMP.h>

#include <linput/linput.h>

#include "Input.h"
#include "render/ShaderManager.h"
#include "render/Shader.h"
#include "render/Object.h"
#include "load/ModelLoader.h"
#include "render/render.h"
#include "render/Mesh.h"
#include "render/Geometry.h"
#include "render/Material.h"
#include "render/Node.h"
#include "render/DebugDraw.h"

#include "System.h"

namespace fractal
{
namespace
{
    lgraphics::Texture2DRef loadDDS(const Char* filepath)
    {
        lgraphics::Texture2DRef ret;

        lcore::ifstream in(filepath, lcore::ios::binary);
        if(!in.is_open()){
            return ret;
        }

        u32 size = in.getSize(0);
        u8* buffer = LIME_NEW u8[size];
        lcore::io::read(in, buffer, size);
        lgraphics::io::IODDS::read(ret, buffer, size, lgraphics::Usage_Immutable, lgraphics::TexFilter_MinMagMipLinear, lgraphics::TexAddress_Clamp);
        LIME_DELETE_ARRAY(buffer);
        return ret;
    }


    f32 RGB2Gray(u8 r, u8 g, u8 b)
    {
        const f32 inv = 1.0f/255.0f;
        return inv*r* 0.299f + inv*g*0.587f + inv*b*0.114f;
    }
}

    Application::Application()
        :cameraType_(Camera_Debug)
        ,plane_(NULL)
        ,box_(NULL)
        ,sphere_(NULL)
        ,hdr_(true)
    {
    }

    Application::~Application()
    {
    }

    void Application::initialize()
    {
        u32 viewWidth, viewHeight;
        window_.getViewSize(viewWidth, viewHeight);

        bool ret = System::initialize(window_.getHandle().hWnd_, viewWidth, viewHeight);
        if(!ret){
            setTerminate();
            return;
        }

        //fractal::ParamDialog::show(TRUE);

        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();
        f32 gray[4] = {0.7f, 0.7f, 0.7f, 1.0f};
        device.setClearColor(gray);

        u32 backWidth, backHeight;
        device.getRenderTargetDesc(backWidth, backHeight);

        // ÉJÉÅÉâê›íË
        lscene::Scene& scene = System::getScene();
        lscene::Camera& camera = scene.getCamera();

        camera.perspectiveFovLinearZ(45.0f/180.0f*PI, static_cast<lcore::f32>(viewWidth)/viewHeight, 0.001f, 10000.0f);
        //camera_.initialize(camera, lmath::Vector3(0.0f, 200.0f, -30.0f), lmath::Vector3(0.0f, 190.0f, 0.0f));
        camera_.initialize(camera, lmath::Vector3(0.0f, 10.0f, -30.0f), lmath::Vector3(0.0f, 10.0f, 0.0f));
        scene.getLightEnv().getDirectionalLight().setColor(lmath::Vector4(3.5f, 3.5f, 3.0f, 1.0f));
        lightDirection_ = scene.getLightEnv().getDirectionalLight().getDirection();
        lightDirection_.set(0.454518f, 0.618679f, -0.640678f, 0);
        lightDirection_.normalize();
        scene.getLightEnv().getDirectionalLight().setDirection(lightDirection_);
        System::getRenderer().setFlag(render::Renderer::Flag_HDR, hdr_);


        {
            plane_ = render::DebugDraw::createPlane(100.0f, 2, 0xFF808080U);
        }

        {
            box_ = render::DebugDraw::createBox(2.0f, 0xFFFE0000U);
            box_->setPosition(lmath::Vector4(10.0f, 2.5f, 0.0f, 0.0f));
        }

        {
            sphere_ = render::DebugDraw::createSphere(1.0f, 10, 0xFF00FF00U);
            sphere_->setPosition(lmath::Vector4(-10.0f, 2.5f, 0.0f, 0.0f));
        }

    }

    void Application::update()
    {
        System& system = System::getInstance();
        system.update();

        Input::Keyboard keyboard = Input::getKeyboard();

        font::FontManager& fontManager = fractal::System::getFontManager();


        switch(cameraType_)
        {
        case Camera_Debug:
            camera_.update(system.getScene().getCamera());
            break;
        };

        if(Input::getMouse().isOn(fractal::Input::MouseButton_Right)){
            s32 dx = Input::getMouse().getAxisDuration(Input::MouseAxis_X);
            s32 dy = Input::getMouse().getAxisDuration(Input::MouseAxis_Y);

            lmath::Quaternion rotX;
            rotX.setRotateY(dx * 0.01f);
            lmath::Quaternion rotY;
            rotY.setRotateX(dy * 0.01f);

            rotX *= rotY;
            lightDirection_.rotate(rotX);
            lightDirection_.normalize();

            lscene::Scene& scene = System::getScene();
            scene.getLightEnv().getDirectionalLight().setDirection(lightDirection_);
        }

        if(keyboard.isClick(fractal::Input::Key_H)){
            hdr_ = !hdr_;
            System::getRenderer().setFlag(render::Renderer::Flag_HDR, hdr_);
        }

        if(NULL != plane_){
            system.getRenderer().add(plane_);
        }

        if(NULL != box_){
            system.getRenderer().add(box_);
        }

        if(NULL != sphere_){
            system.getRenderer().add(sphere_);
        }
    }

    void Application::terminate()
    {
        LIME_DELETE(sphere_);
        LIME_DELETE(box_);
        LIME_DELETE(plane_);
        System::terminate();
    }

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch(msg)
        {
        case WM_SYSCOMMAND:
            {
                switch(wParam)
                {
                case SC_SCREENSAVE:
                case SC_MONITORPOWER:
                    return 0;
                }
                return DefWindowProc(hWnd, msg, wParam, lParam);
            }
            break;

        case WM_KILLFOCUS:

            linput::Input::getInstance().clear();
            break;

        case WM_KEYDOWN:
            if(wParam == VK_ESCAPE){
                PostQuitMessage(0);
                return 1;
            }
            break;

        case WM_PAINT:
            {
                ValidateRect(hWnd, NULL);
            }

            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 1;
            break;

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
            break;
        }
        return 0;
    }
}
