/**
@file DebugDraw.cpp
@author t-sakai
@date 2012/05/17 create
*/
#include "DebugDraw.h"

#include <lmath/lmath.h>
#include <lgraphics/lgraphics.h>

#include "../System.h"

#include "Shader.h"
#include "ShaderManager.h"

namespace render
{
    DebugDraw::DebugDraw()
        :vs_(NULL)
        ,ps_(NULL)
        ,maxLines_(0)
        ,numLines_(0)
        ,lines_(NULL)
    {
    }

    DebugDraw::~DebugDraw()
    {
    }


    void DebugDraw::initialize(u32 maxLines)
    {
        terminate();

        ShaderVSBase* vs = NULL;
        ShaderPSBase* ps = NULL;
        ShaderManager::getInstance().get(render::Shader_DebugVC, vs, ps);
        vs_ = reinterpret_cast<DebugVCVS*>(vs);
        ps_ = reinterpret_cast<DebugVCPS*>(ps);

        inputLayout_ = fractal::System::getInputLayoutFactory().get(render::Layout_PC);

        lgraphics::VertexBufferRef vb = lgraphics::VertexBuffer::create(
            maxLines * 2 * sizeof(Vertex),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            0,
            NULL);

        vbLines_ = vb;

        lines_ = reinterpret_cast<Vertex*>(LIME_MALLOC(sizeof(Vertex)*maxLines*2));
        maxLines_ = maxLines;
    }

    void DebugDraw::terminate()
    {
        LIME_FREE(lines_);
        numLines_ = 0;

        {
            lgraphics::InputLayoutRef inputLayout;
            inputLayout.swap(inputLayout_);
        }

        {
            lgraphics::VertexBufferRef vbLines;
            vbLines.swap(vbLines_);
        }

        ps_ = NULL;
        vs_ = NULL;
    }

    void DebugDraw::add(const lmath::Vector3& v0, const lmath::Vector3& v1, u32 color)
    {
        if(maxLines_<=numLines_){
            return;
        }

        u32 index = 2*numLines_;
        lines_[index].x_ = v0.x_;
        lines_[index].y_ = v0.y_;
        lines_[index].z_ = v0.z_;
        lines_[index].color_ = color;

        ++index;
        lines_[index].x_ = v1.x_;
        lines_[index].y_ = v1.y_;
        lines_[index].z_ = v1.z_;
        lines_[index].color_ = color;

        ++numLines_;
    }

    void DebugDraw::add(const lmath::Vector4& v0, const lmath::Vector4& v1, u32 color)
    {
        if(maxLines_<=numLines_){
            return;
        }

        u32 index = 2*numLines_;
        lines_[index].x_ = v0.x_;
        lines_[index].y_ = v0.y_;
        lines_[index].z_ = v0.z_;
        lines_[index].color_ = color;

        ++index;
        lines_[index].x_ = v1.x_;
        lines_[index].y_ = v1.y_;
        lines_[index].z_ = v1.z_;
        lines_[index].color_ = color;

        ++numLines_;
    }

    void DebugDraw::draw(lgraphics::GraphicsDeviceRef& device)
    {
        vbLines_.updateSubresource(
            0,
            NULL,
            reinterpret_cast<const void*>(lines_),
            0,
            0);

        inputLayout_.attach();
        vs_->attach();
        ps_->attach();

        device.setPrimitiveTopology(lgraphics::Primitive_LineList);
        vbLines_.attach(0, sizeof(Vertex), 0);
        device.draw(numLines_*2, 0);

        numLines_ = 0;
    }
}
