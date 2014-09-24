/**
@file Debug.cpp
@author t-sakai
@date 2014/09/08 create
*/
#include "Debug.h"
#include <lframework/scene/Frustum.h>

#include "System.h"
#include "ShaderManager.h"
#include "Shader.h"

namespace render
{
    Debug::Debug()
        :maxVertices_(0)
        ,maxIndices_(0)
        ,vertices_(NULL)
        ,indices_(NULL)
    {
    }

    Debug::~Debug()
    {
        destroy();
    }

    void Debug::create(s32 maxVertices, s32 maxIndices)
    {
        maxVertices_ = maxVertices;
        maxIndices_ = maxIndices;

        vertices_ = LIME_NEW Vertex[maxVertices_];
        indices_ = LIME_NEW u16[maxIndices_];

        inputLayout_ = fractal::System::getInputLayoutFactory().get(render::Layout_PC);
        vb_ = lgraphics::VertexBuffer::create(
            sizeof(Vertex)*maxVertices_,
            lgraphics::Usage_Dynamic,
            lgraphics::BindFlag_VertexBuffer,
            lgraphics::CPUAccessFlag_Write,
            lgraphics::ResourceMisc_None,
            sizeof(Vertex),
            NULL);

        ib_ = lgraphics::IndexBuffer::create(
            sizeof(u16)*maxIndices_,
            lgraphics::Usage_Dynamic,
            lgraphics::CPUAccessFlag_Write,
            lgraphics::ResourceMisc_None);

        render::ShaderVSBase* vs;
        render::ShaderPSBase* ps;
        ShaderManager::getInstance().get(
            render::Shader_Debug, vs, ps);
        vs_ = reinterpret_cast<DebugVS*>(vs);
        ps_ = reinterpret_cast<DebugPS*>(ps);
    }

    void Debug::destroy()
    {
        ib_.destroy();
        vb_.destroy();
        inputLayout_.destroy();
        LIME_DELETE_ARRAY(indices_);
        LIME_DELETE_ARRAY(vertices_);
    }

    s32 Debug::getMaxVertices() const
    {
        return maxVertices_;
    }

    s32 Debug::getMaxIndices() const
    {
        return maxIndices_;
    }

    Debug::Vertex* Debug::getVertices()
    {
        return vertices_;
    }

    u16* Debug::getIndices()
    {
        return indices_;
    }

    void Debug::update(s32 numVertices, s32 numIndices)
    {
        lgraphics::MappedSubresource mapped;
        if(vb_.map(0, lgraphics::MapType_WriteDiscard, mapped)){
            lcore::memcpy(mapped.data_, vertices_, sizeof(Vertex)*numVertices);
            vb_.unmap(0);
        }

        if(ib_.map(0, lgraphics::MapType_WriteDiscard, mapped)){
            lcore::memcpy(mapped.data_, indices_, sizeof(u16)*numIndices);
            ib_.unmap(0);
        }
    }

    void Debug::draw(const lmath::Matrix44& mwvp)
    {
    }

    void Debug::drawFrustum(
        const lmath::Matrix44& mwvp,
            const lmath::Matrix44& projection,
            f32 znear,
            f32 zfar,
            u32 color)
    {
        lscene::Frustum frustum;
        frustum.calcFromProjection(projection, znear, zfar);

        lmath::Vector4 frustumPoints[8];
        frustum.getPoints(frustumPoints);

        vertices_[0].position_.set(frustumPoints[0]);
        vertices_[0].color_ = color;
        vertices_[1].position_.set(frustumPoints[4]);
        vertices_[1].color_ = color;
        vertices_[2].position_.set(frustumPoints[1]);
        vertices_[2].color_ = color;
        vertices_[3].position_.set(frustumPoints[5]);
        vertices_[3].color_ = color;

        vertices_[4].position_.set(frustumPoints[2]);
        vertices_[4].color_ = color;
        vertices_[5].position_.set(frustumPoints[6]);
        vertices_[5].color_ = color;
        vertices_[6].position_.set(frustumPoints[3]);
        vertices_[6].color_ = color;
        vertices_[7].position_.set(frustumPoints[7]);
        vertices_[7].color_ = color;

        indices_[0] = 0;
        indices_[1] = 1;
        indices_[2] = 2;
        indices_[3] = 3;

        indices_[4] = 4;
        indices_[5] = 5;
        indices_[6] = 6;
        indices_[7] = 7;

        update(8, 8);

        vs_->update(mwvp);
        vs_->attach();
        ps_->attach();

        lgraphics::GraphicsDeviceRef& device = lgraphics::Graphics::getDevice();

        device.setPrimitiveTopology(lgraphics::Primitive_TriangleStrip);
        inputLayout_.attach();
        vb_.attach(0, sizeof(Vertex), 0);
        ib_.attach(lgraphics::Data_R16_UInt, 0);
        device.drawIndexed(8, 0, 0);
    }
}
