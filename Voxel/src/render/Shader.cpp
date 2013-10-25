/**
@file Shader.cpp
@author t-sakai
@date 2011/10/16 create
*/
#include "Shader.h"

#include <lframework/scene/scene.h>

#include "render/Material.h"

namespace render
{
    //----------------------------------------------
    //---
    //--- Default
    //---
    //----------------------------------------------
    void DefaultPNVS::initialize()
    {
        constant_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }


    void DefaultPS::initialize()
    {
        constant0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        constant1_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant1),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }


    //----------------------------------------------
    //---
    //--- Depth
    //---
    //----------------------------------------------
    void DepthVS::initialize()
    {
        constants_ = lgraphics::ConstantBuffer::create(
            sizeof(lmath::Matrix44),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    void DepthVS::set(const lmath::Matrix44& mat)
    {
        constants_.update(0, NULL, &mat, 0, 0);
        constants_.setVS(0);
    }

    //----------------------------------------------
    //---
    //--- RectangleVS
    //---
    //----------------------------------------------
    void RectangleVS::initialize()
    {
        constants_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    //----------------------------------------------
    //---
    //--- Voxelize
    //---
    //----------------------------------------------
    void VoxelizePNVS::initialize()
    {
        constant0_ = lgraphics::ConstantBuffer::create(
            sizeof(lmath::Matrix44),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    void VoxelizePNGS::initialize()
    {
        constant0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    void VoxelizePS::initialize()
    {
        constant0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        constant1_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant1),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    //----------------------------------------------
    //---
    //--- VoxelRender
    //---
    //----------------------------------------------
    void VoxelRenderVS::initialize()
    {
        constant0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    void VoxelRenderPS::initialize()
    {
        constant0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    //----------------------------------------------
    //---
    //--- OctreeTagPS
    //---
    //----------------------------------------------
    void OctreeTagPS::initialize()
    {
        constant0_ = lgraphics::ConstantBuffer::create(
            sizeof(OctreeTagPS::Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    //----------------------------------------------
    //---
    //--- OctreePropagatePS
    //---
    //----------------------------------------------
    void OctreePropagatePS::initialize()
    {
        constant0_ = lgraphics::ConstantBuffer::create(
            sizeof(OctreePropagatePS::Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    //----------------------------------------------
    //---
    //--- DefaultAO
    //---
    //----------------------------------------------
    void DefaultAOPS::initialize()
    {
        constant0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        constant1_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant1),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }
}
