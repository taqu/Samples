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
#include <lframework/anim/AnimationClip.h>

#include <linput/linput.h>

#include "Input.h"
#include "render/ShaderManager.h"
#include "render/Shader.h"
#include "render/Object.h"
#include "load/ModelLoader.h"
#include "render/DebugDraw.h"

#include "System.h"

namespace fractal
{
namespace
{

    f32 RGB2Gray(u8 r, u8 g, u8 b)
    {
        const f32 inv = 1.0f/255.0f;
        return inv*r* 0.299f + inv*g*0.587f + inv*b*0.114f;
    }
}

    Application::Application()
        :cameraType_(Camera_Debug)
        ,count_(0.0f)
        ,object_(NULL)
        ,plane_(NULL)
        ,box0_(NULL)
        ,sphere0_(NULL)
        ,box1_(NULL)
        ,box2_(NULL)
        ,sponza_(NULL)
        ,drawSponza_(true)
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
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();
        f32 gray[4] = {0.7f, 0.7f, 0.7f, 1.0f};
        device.setClearColor(gray);

        u32 backWidth, backHeight;
        device.getRenderTargetDesc(backWidth, backHeight);

        // ÉJÉÅÉâê›íË
        lscene::Scene& scene = System::getScene();
        lscene::Camera& camera = scene.getCamera();

        camera.perspectiveFov(45.0f/180.0f*PI, static_cast<lcore::f32>(viewWidth)/viewHeight, 0.1f, 1000.0f);
        camera_.initialize(camera, lmath::Vector3(-30.0f, 10.0f, 0.0f), lmath::Vector3(0.0f, 10.0f, 0.0f));

        //ÉâÉCÉgê›íË
        scene.getLightEnv().getDirectionalLight().setColor(lmath::Vector4(7.5f, 7.5f, 7.0f, 1.0f));
        lightDirection_ = scene.getLightEnv().getDirectionalLight().getDirection();
        lightDirection_.set(0.5f, 2.0f, 0.1f, 0);
        lightDirection_.normalize();
        scene.getLightEnv().getDirectionalLight().setDirection(lightDirection_);
        System::getRenderer().setFlag(render::Renderer::Flag_HDR, true);
        System::getRenderer().setFlag(render::Renderer::Flag_SAO, true);
        System::getRenderer().setFlag(render::Renderer::Flag_GI, true);
        {
            
            load::ModelLoader loader;
            if(loader.open("../data/model/1052.lm")){

                render::Object* obj = LIME_NEW render::Object();
                if(loader.load(*obj)){
                    lcore::swap(object_, obj);
                }
                LIME_DELETE(obj);

                object_->setPosition(lmath::Vector4(-40.0f, 0.0f, 0.0f, 0.0f));
            }
        }

        {
            //plane_ = render::DebugDraw::createPlane(100.0f, 2, 0xFFFFFFFFU);
            plane_ = render::DebugDraw::createPlane(100.0f, 2, 0xFF808080U);
            plane_->setPosition(lmath::Vector4(-40.0f, -0.5f, 0.0f, 0.0f));
        }

        {
            box0_ = render::DebugDraw::createBox(8.0f, 0xFFFF0000U, 1.0f);
            box0_->setPosition(lmath::Vector4(-25.0f, 4.0f, 0.0f, 0.0f));

            box1_ = render::DebugDraw::createBox(8.0f, 0xFF0000FFU, 1.0f);
            box1_->setPosition(lmath::Vector4(-40.0f, 6.0f, 15.0f, 0.0f));

            box2_ = render::DebugDraw::createBox(8.0f, 0xFFFFFF00U, 1.0f);
            box2_->setPosition(lmath::Vector4(-40.0f, 4.0f, -15.0f, 0.0f));
        }

        {
            sphere0_ = render::DebugDraw::createSphere(4.0f, 10, 0xFF00FF00U, 1.0f);
            sphere0_->setPosition(lmath::Vector4(-55.0f, 4.0f, 0.0f, 0.0f));
        }

        {
            
            load::ModelLoader loader;
            if(loader.open("../data/model/sponza.lm")){
                loader.setTextureAddress(lgraphics::TexAddress_Wrap);
                render::Object* obj = LIME_NEW render::Object();
                if(loader.load(*obj)){
                    lcore::swap(sponza_, obj);
                }
                LIME_DELETE(obj);
            }
        }

    }

    void Application::update()
    {
        System& system = System::getInstance();
        system.update();

        Input::Keyboard keyboard = Input::getKeyboard();
        //Input::Joystick joystick = Input::getJoystick();

        //s32 axisX = joystick.getAxisPosition(fractal::Input::JoystickAxis_X);
        //s32 axisY = joystick.getAxisPosition(fractal::Input::JoystickAxis_Y);

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
            bool hdr = System::getRenderer().getFlag(render::Renderer::Flag_HDR);
            System::getRenderer().setFlag(render::Renderer::Flag_HDR, !hdr);
        }

        if(keyboard.isClick(fractal::Input::Key_O)){
            bool sao = System::getRenderer().getFlag(render::Renderer::Flag_SAO);
            System::getRenderer().setFlag(render::Renderer::Flag_SAO, !sao);
        }

        if(keyboard.isClick(fractal::Input::Key_V)){
            bool voxel = System::getRenderer().getFlag(render::Renderer::Flag_DrawVoxel);
            System::getRenderer().setFlag(render::Renderer::Flag_DrawVoxel, !voxel);
        }


        if(keyboard.isClick(fractal::Input::Key_G)){
            bool gi = System::getRenderer().getFlag(render::Renderer::Flag_GI);
            System::getRenderer().setFlag(render::Renderer::Flag_GI, !gi);
        }

        if(keyboard.isClick(fractal::Input::Key_M)){
            drawSponza_ = !drawSponza_;
        }

        if(NULL != object_){
            system.getRenderer().add(object_);
        }

        if(NULL != plane_){
            system.getRenderer().add(plane_);
        }

        f32 radian = count_ * DEG_TO_RAD;
        if(NULL != box0_){
            system.getRenderer().add(box0_);
        }

        if(NULL != box1_){
            lmath::Quaternion rot;
            rot.setRotateX(radian);
            box1_->setRotation(rot);

            system.getRenderer().add(box1_);
        }

        if(NULL != box2_){
            lmath::Quaternion rot;
            rot.setRotateY(radian);
            box2_->setRotation(rot);

            system.getRenderer().add(box2_);
        }

        if(NULL != sphere0_){
            system.getRenderer().add(sphere0_);
        }

        if(drawSponza_ && NULL != sponza_){
            system.getRenderer().add(sponza_);
        }

        count_ += System::getTimer().getDeltaTime() * 90.0f;
        if(360.0f<count_){
            count_ -= 360.0f;
        }
    }

    void Application::terminate()
    {
        LIME_DELETE(sponza_);
        LIME_DELETE(sphere0_);
        LIME_DELETE(box0_);
        LIME_DELETE(box1_);
        LIME_DELETE(box2_);
        LIME_DELETE(plane_);
        LIME_DELETE(object_);
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
