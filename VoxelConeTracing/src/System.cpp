/**
@file System.cpp
@author t-sakai
@date 2011/09/24
*/
#include "System.h"

#include <lcore/liostream.h>

#include <lgraphics/lgraphics.h>

#include <lframework/scene/Scene.h>
#include <lframework/scene/Camera.h>
#include <lframework/scene/LightEnvironment.h>

#include "render/ShaderManager.h"

#include "Input.h"

namespace fractal
{
    System* System::instance_ = NULL;


    bool System::initialize(HWND__* hWnd, u32 width, u32 height)
    {
        if(instance_ == NULL){

            instance_ = LIME_NEW System;
            return instance_->initImpl(hWnd, width, height);
        }
        return true;
    }

    void System::terminate()
    {
        if(instance_ != NULL){
            instance_->termImpl();
            LIME_DELETE(instance_);
        }
    }

    //
    bool System::initImpl(HWND__* hWnd, u32 width, u32 height)
    {
        xorshift_.srand(lcore::getDefaultSeed());

        fractal::Input::initialize(hWnd);

        //lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        {
            lscene::Scene& scene = renderer_.getScene();

            // カメラ設定
            lscene::Camera& camera = scene.getCamera();

            camera.perspectiveFov(45.0f/180.0f*PI, static_cast<lcore::f32>(width)/height, 0.1f, 1000.0f);

            lmath::Matrix44 mat;
            mat.lookAt(lmath::Vector4(0.0f, 0.0f, -3.0f, 0.0f), lmath::Vector4(0.0f, 0.0f, 0.0f, 0.0f), lmath::Vector4(0.0f, 1.0f, 0.0f, 0.0f));

            camera.setViewMatrix(mat);
            camera.updateMatrix();
            //camera_.initialize(lmath::Vector3(0.0f, 0.0f, 2.0f), lmath::Vector3(0.0f, 0.0f, 0.0f));

            // ライト設定
            lscene::LightEnvironment& lightEnv = scene.getLightEnv();
            lightEnv.getDirectionalLight().setColor(lmath::Vector4(6.0f, 6.0f, 6.0f, 1.0f));
            lightEnv.getDirectionalLight().setDirection(lmath::Vector4(0.0f, 1.0f, 0.0f, 0.0f));
            //lightEnv.getDirectionalLight().setDirection(lmath::Vector3(0.0f, -1.0f, 0.0f));
        }

        render::ShaderManager::initialize();
        if(false == inputLayoutFactory_.initialize()){
            return false;
        }

        if(!renderer_.initialize(InitBatchSize, DebugMaxLines, 800, 600)){
            return false;
        }

        if(!renderer_.reset()){
            return false;
        }

        fontManager_.initialize();
        fontManager_.setResolution(width, height);

        lcore::ifstream file("../data/mplus20df.fpak", lcore::ios::binary);
        fontManager_.load(file);


        renderer_.calcZFar(0.1f, 1000.0f);
        
        timer_.reset();
        return true;
    }

    void System::termImpl()
    {
        fontManager_.terminate();
        renderer_.terminate();
        render::ShaderManager::terminate();
        fractal::Input::terminate();
    }

    void System::update()
    {
        timer_.update();
        Input::update();

        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        renderer_.draw();
        fontManager_.draw();
        device.present();
    }

    bool System::reset()
    {
        timer_.reset();

        return renderer_.reset();
    }
}
