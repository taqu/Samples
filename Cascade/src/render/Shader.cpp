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
            sizeof(lmath::Matrix44)*4,
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    void DepthVS::set(const lmath::Matrix44 mat[4])
    {
        constants_.update(0, NULL, mat, 0, 0);
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
    //--- SkyBox
    //---
    //----------------------------------------------
    void SkyBoxVS::initialize()
    {
        constant0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    //----------------------------------------------
    //---
    //--- SkyBox
    //---
    //----------------------------------------------
    void HDAOPS::initialize()
    {
        constant0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    //----------------------------------------------
    //---
    //--- Voxel
    //---
    //----------------------------------------------
    void VoxelGS::initialize()
    {
        constants_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    void VoxelGS::set(const Constant0& constant0)
    {
        constants_.update(0, NULL, &constant0, 0, 0);
        constants_.setGS(0);
    }

    void VoxelPS::initialize()
    {
        constants_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    void VoxelPS::set(const Constant0& constant0)
    {
        constants_.update(0, NULL, &constant0, 0, 0);
        constants_.setPS(0);
    }
}