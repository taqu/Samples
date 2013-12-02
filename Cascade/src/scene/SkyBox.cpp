/**
@file SkyBox.cpp
@author t-sakai
@date 2013/07/12 create
*/
#include "SkyBox.h"

#include <lframework/scene/Scene.h>

#include "render/ShaderManager.h"
#include "render/Shader.h"

namespace scene
{
    SkyBox::SkyBox()
    {
    }

    SkyBox::~SkyBox()
    {
    }

    void SkyBox::initialize()
    {
        render::ShaderManager& shaderManager = render::ShaderManager::getInstance();
        render::ShaderVSBase* vs;
        render::ShaderPSBase* ps;

        shaderManager.get(
            render::Shader_SkyBox,
            vs,
            ps);

        vs_ = reinterpret_cast<render::SkyBoxVS*>(vs);
        ps_ = reinterpret_cast<render::SkyBoxPS*>(ps);
    }

    void SkyBox::update(const lscene::Camera& camera, u32 screenWidth, u32 screenHeight)
    {
        render::SkyBoxVS::Constant0 constant0;
        constant0.nearAspect_[0] = camera.getZNear();
        constant0.nearAspect_[1] = camera.getAspect();
        constant0.invViewProj_ = camera.getViewMatrix();
        constant0.invViewProj_.setTranslate(0.0f, 0.0f, 0.0f);
        constant0.invViewProj_.mul(camera.getProjMatrix(), constant0.invViewProj_);
        constant0.invViewProj_.invert();


        //lmath::Vector4 v[5] =
        //{
        //    lmath::Vector4(0.0f, 0.0f, camera.getZNear(), 1.0f),
        //    lmath::Vector4(1.0f, 0.0f, camera.getZNear(), 1.0f),
        //    lmath::Vector4(0.0f, 1.0f, camera.getZNear(), 1.0f),
        //    lmath::Vector4(1.0f, 1.0f, camera.getZNear(), 1.0f),
        //    lmath::Vector4(0.0f, 0.0f, camera.getZNear(), 1.0f),
        //};

        //for(s32 i=0; i<5; ++i){
        //    v[i].mul(constant0.invViewProj_, v[i]);
        //    lcore::Log("v(%f, %f, %f, %f)", v[i].x_, v[i].y_, v[i].z_, v[i].w_);
        //}

        //f32 aspect = v[1].x_/v[2].y_;

        vs_->update0(constant0);
        vs_->set();

        vs_->attach();
        ps_->attach();
    }
}
