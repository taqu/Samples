/**
@file Rectangle.cpp
@author t-sakai
@date 2012/04/24 create

*/
#include "stdafx.h"

#include <lgraphics/lgraphics.h>
#include <lgraphics/api/TextureRef.h>

#include "Rectangle.h"

#include "render/ShaderManager.h"
#include "render/Shader.h"

namespace render
{
    bool Rectangle::create()
    {
        ShaderVSBase* vs = NULL;
        ShaderPSBase* ps = NULL;
        ShaderManager::getInstance().get(render::Shader_Rectangle, vs, ps);
        vs_ = reinterpret_cast<RectangleVS*>(vs);
        ps_ = reinterpret_cast<RectanglePS*>(ps);

        setFullScreenQuad();
        return true;
    }

    void Rectangle::draw()
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        device.setInputLayout(NULL);
        device.setVertexBuffers(0, 0, NULL, NULL, NULL);

        device.setPrimitiveTopology(lgraphics::Primitive_TriangleStrip);

        vs_->attach();
        vs_->set(constants_);
        ps_->attach();

        device.draw(4, 0);
    }

    void Rectangle::draw(lgraphics::Texture2DRef& texture)
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        device.setInputLayout(NULL);
        device.setVertexBuffers(0, 0, NULL, NULL, NULL);

        vs_->attach();
        vs_->set(constants_);
        ps_->attach();
        texture.attachPS(0, 0);

        device.setPrimitiveTopology(lgraphics::Primitive_TriangleStrip);
        device.draw(4, 0);
    }

    void Rectangle::drawNoPS(lgraphics::Texture2DRef& texture)
    {
        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        device.setInputLayout(NULL);
        device.setVertexBuffers(0, 0, NULL, NULL, NULL);

        vs_->attach();
        vs_->set(constants_);
        texture.attachPS(0, 0);

        device.setPrimitiveTopology(lgraphics::Primitive_TriangleStrip);
        device.draw(4, 0);
    }

    void Rectangle::draw(
        f32 minx,
        f32 maxx,
        f32 miny,
        f32 maxy,
        lgraphics::Texture2DRef& texture)
    {
        constants_[0].set(minx, maxy, 0.0f, 0.0f);
        constants_[1].set(maxx, maxy, 1.0f, 0.0f);
        constants_[2].set(minx, miny, 0.0f, 1.0f);
        constants_[3].set(maxx, miny, 1.0f, 1.0f);

        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        device.setInputLayout(NULL);
        device.setVertexBuffers(0, 0, NULL, NULL, NULL);

        vs_->attach();
        vs_->set(constants_);
        ps_->attach();
        texture.attachPS(0, 0);

        device.setPrimitiveTopology(lgraphics::Primitive_TriangleStrip);
        device.draw(4, 0);

        setFullScreenQuad();
    }

    void Rectangle::setFullScreenQuad()
    {
        constants_[0].set(-1.0f,  1.0f, 0.0f, 0.0f);
        constants_[1].set( 1.0f,  1.0f, 1.0f, 0.0f);
        constants_[2].set(-1.0f, -1.0f, 0.0f, 1.0f);
        constants_[3].set( 1.0f, -1.0f, 1.0f, 1.0f);
    }
}
