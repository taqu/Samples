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
    //--- ReflectiveShadowMap
    //---
    //----------------------------------------------
    void ReflectiveShadowMapVS::initialize()
    {
        constants0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        constants1_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant1),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    //void ReflectiveShadowMapPS::initialize()
    //{
    //    constants_ = lgraphics::ConstantBuffer::create(
    //        sizeof(Constant),
    //        lgraphics::Usage_Default,
    //        lgraphics::CPUAccessFlag_None,
    //        lgraphics::ResourceMisc_None);
    //}

    //void ReflectiveShadowMapPS::set(const Constant& constants)
    //{
    //    constants_.update(0, NULL, &constants, 0, 0);
    //    constants_.setPS(0);
    //}

    //----------------------------------------------
    //---
    //--- LPVInjection
    //---
    //----------------------------------------------
    void LPVInjectionVS::initialize()
    {
        constants0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        constants1_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant1),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    void LPVInjectionGS::initialize()
    {
        constants_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }


    //----------------------------------------------
    //---
    //--- LPVPropagate
    //---
    //----------------------------------------------
    void LPVOcclusionPropagatePS::initialize()
    {
        constants0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }
#if 0

    //----------------------------------------------
    //---
    //--- LPVPropagate
    //---
    //----------------------------------------------
    void LPVPropagatePS::initialize()
    {
        //setHandle(params_[Param0], "param0");
        setHandle(params_[Param1], "param1");
        //setHandle(params_[GParam1], "gatherParams1");

        setSampler(samplers_[Tex_R], "texR");
        setSampler(samplers_[Tex_G], "texG");
        setSampler(samplers_[Tex_B], "texB");
    }

    void LPVPropagatePS::createGatherParameter(const lmath::Vector4& volumeParameter1)
    {
#if 0
        lmath::Vector3 offsets[6] =
        {
            lmath::Vector3(0.0f, 0.0f, 1.0f),
            lmath::Vector3(0.0f, 0.0f, -1.0f),
            lmath::Vector3(1.0f, 0.0f, 0.0f),
            lmath::Vector3(-1.0f, 0.0f, 0.0f),
            lmath::Vector3(0.0f, 1.0f, 0.0f),
            lmath::Vector3(0.0f, -1.0f, 0.0f),
        };


        //lcore::ofstream out("out.txt", lcore::ios::binary);

        for(s32 neighbor=0; neighbor<6; ++neighbor){
            lmath::Vector3 neighborCellCenter = offsets[neighbor];

            lmath::Vector4 occlusionOffset;
            occlusionOffset.zero();
            if(neighborCellCenter.x_>0.0f)
                occlusionOffset.set(6.0f, 1.0f, 2.0f, 0.0f);
            else if(neighborCellCenter.x_<0.0f)
                occlusionOffset.set(7.0f, 5.0f, 4.0f, 3.0f); 
            else if(neighborCellCenter.y_>0.0f)
                occlusionOffset.set(0.0f, 3.0f, 1.0f, 5.0f);
            else if(neighborCellCenter.y_<0.0f)
                occlusionOffset.set(2.0f, 4.0f, 6.0f, 7.0f);
            else if(neighborCellCenter.z_>0.0f)
                occlusionOffset.set(0.0f, 3.0f, 2.0f, 4.0f);
            else if(neighborCellCenter.z_<0.0f)
                occlusionOffset.set(1.0f, 5.0f, 6.0f, 7.0f);

            lmath::Vector4 multiBounceOffset;
            multiBounceOffset.zero();
            if(neighborCellCenter.x_>0.0f)
                multiBounceOffset.set(7.0f, 5.0f, 4.0f, 3.0f);
            else if(neighborCellCenter.x_<0.0f)
                multiBounceOffset.set(6.0f, 1.0f, 2.0f, 0.0f);
            else if(neighborCellCenter.y_>0.0f)
                multiBounceOffset.set(2.0f, 4.0f, 6.0f, 7.0f);
            else if(neighborCellCenter.y_<0.0f)
                multiBounceOffset.set(0.0f, 3.0f, 1.0f, 5.0f);
            else if(neighborCellCenter.z_>0.0f)
                multiBounceOffset.set(1.0f, 5.0f, 6.0f, 7.0f);
            else if(neighborCellCenter.z_<0.0f)
                multiBounceOffset.set(0.0f, 3.0f, 2.0f, 4.0f);

            //gatherParams0_[neighbor].occlusionOffset_ = occlusionOffset;
            //gatherParams0_[neighbor].multiBounceOffset_ = multiBounceOffset;

            for(s32 face=0; face<6; ++face){
                lmath::Vector3 facePosition = offsets[face];
                facePosition *= 0.5f;

                lmath::Vector3 vecFromNeighborCelCenter(facePosition);
                vecFromNeighborCelCenter += neighborCellCenter;

                f32 length = vecFromNeighborCelCenter.length();
                vecFromNeighborCelCenter /= length;

                s32 index = 6*neighbor + face;

                //lmath::Vector3 center(neighborCellCenter);
                //center *= volumeParameter1.y_;
                //gatherParams1_[index].neighborOffset_.set(center.x_, center.y_, center.z_, 1.0f);
                gatherParams1_[index].x_ = vecFromNeighborCelCenter.x_;
                gatherParams1_[index].y_ = vecFromNeighborCelCenter.y_;
                gatherParams1_[index].z_ = vecFromNeighborCelCenter.z_;

                if(length<=0.5f){
                    gatherParams1_[index].solidAngle_ = 0.0f;
                }else if(length>=1.5f){
                    gatherParams1_[index].solidAngle_ = 0.4006696846f*(INV_PI);
                }else{
                    gatherParams1_[index].solidAngle_ = 0.4234413544f*(INV_PI);
                }

                //out.print("float4(%f, %f, %f, %f),\n",
                //    gatherParams1_[index].x_,
                //    gatherParams1_[index].y_,
                //    gatherParams1_[index].z_,
                //    gatherParams1_[index].solidAngle_);

            }//for(s32 face=0;
        }//for(s32 neighbor=0;
#endif
    }

    void LPVPropagatePS::setConstant(
            const lmath::Vector4& volumeParameter1)
    {
        //ShaderVSBase::set(params_[Param0], volumeParameter0);
        ShaderPSBase::set(params_[Param1], volumeParameter1);
        //ShaderPSBase::set(params_[GParam1], (const lmath::Vector4*)gatherParams1_, NumGatherParam1);
    }

#endif

    //----------------------------------------------
    //---
    //--- LPVRender
    //---
    //----------------------------------------------
    void LPVRenderVS::initialize()
    {
        constants0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        constants1_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant1),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    void LPVRenderPS::initialize()
    {
        constants0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        constants1_ = lgraphics::ConstantBuffer::create(
            sizeof(Constant1),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }
}
