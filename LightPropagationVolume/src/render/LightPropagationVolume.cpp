/**
@file LightPropagationVolume.cpp
@author t-sakai
@date 2012/05/29 create

*/
#include "stdafx.h"

#include "LightPropagationVolume.h"

#include <lframework/scene/Scene.h>
#include <lframework/scene/Frustum.h>

#include "render/ShaderManager.h"

namespace render
{
    LightPropagationVolume::LightPropagationVolume()
        :oldColor_(NULL)
        ,oldDepthStencil_(NULL)
        ,volumeResolution_(0)
        ,invVolumeResolution_(1.0f)
        ,zfar_(1.0f)
        ,LPVCellSize_(0.0f)
        ,invLPVCellSize_(1.0f)
        ,invLPVSize_(1.0f)
        ,LPVAttenuation_(1.0f)
        ,numVertices_(0)
        ,enableOcclusion_(false)
    {
        lightViewProjection_.identity();
    }

    LightPropagationVolume::~LightPropagationVolume()
    {
        SAFE_RELEASE(oldColor_);
        SAFE_RELEASE(oldDepthStencil_);
    }

    bool LightPropagationVolume::initialize(
        u32 reflectiveMapSize,
        u32 volumeResolution,
        f32 zfar,
        f32 attenuation)
    {
        LASSERT(reflectiveMapSize>0);
        LASSERT(volumeResolution>0);

        volumeResolution_ = volumeResolution;
        invVolumeResolution_ = 1.0f/volumeResolution;
        zfar_ = zfar;
        LPVCellSize_ = zfar_*invVolumeResolution_;
        invLPVCellSize_ = 1.0f/LPVCellSize_;
        invLPVSize_ = 1.0f/(LPVCellSize_ * volumeResolution_);
        setLPVAttenuation(attenuation);
        numVertices_ = reflectiveMapSize*reflectiveMapSize*reflectiveMapSize;

        LPVInjectionVS::Constant0 injectionConstants0;
        {
            f32 p = lmath::ceil( lmath::log(static_cast<f32>(reflectiveMapSize))*INV_LOG2 );
            injectionConstants0.size_ = static_cast<s32>(reflectiveMapSize);
            injectionConstants0.mask_ = static_cast<s32>(reflectiveMapSize - 1);
            injectionConstants0.shift_ = static_cast<s32>(p);

            injectionConstants0.invWidth_ = 1.0f/(reflectiveMapSize-1);
            injectionConstants0.invHeight_ = 1.0f/(reflectiveMapSize-1);


            f32 invRMS = 1.0f/reflectiveMapSize;
            injectionConstants0.occlusionAttenuation_ = 4.0f * invRMS * invRMS * invLPVCellSize_ * invLPVCellSize_;
        }


        ShaderManager& manager = ShaderManager::getInstance();
        ShaderVSBase* vs = NULL;
        ShaderGSBase* gs = NULL;
        ShaderPSBase* ps = NULL;

        manager.get(Shader_LPVInjection, vs, ps);
        injectionVS_ = reinterpret_cast<LPVInjectionVS*>(vs);
        injectionPS_ = reinterpret_cast<LPVInjectionPS*>(ps);

        injectionVS_->setConstant(injectionConstants0);

        manager.get(ShaderGS_LPVInjection, gs);
        injectionGS_ = reinterpret_cast<LPVInjectionGS*>(gs);
        injectionGS_->set(invVolumeResolution_);

        manager.get(Shader_LPVOcclusionInjection, vs, ps);
        occlusionInjectionVS_ = reinterpret_cast<LPVOcclusionInjectionVS*>(vs);
        occlusionInjectionPS_ = reinterpret_cast<LPVOcclusionInjectionPS*>(ps);

        manager.get(Shader_LPVPropagate, vs, ps);
        propagateVS_ = reinterpret_cast<LPVPropagateVS*>(vs);
        propagatePS_ = reinterpret_cast<LPVPropagatePS*>(ps);

        manager.get(Shader_LPVOcclusionPropagate, vs, ps);
        occlusionPropagatePS_ = reinterpret_cast<LPVOcclusionPropagatePS*>(ps);
        occlusionPropagatePS_->set(invLPVSize_);

        manager.get(ShaderGS_LPVPropagate, gs);
        propagateGS_ = reinterpret_cast<LPVPropagateGS*>(gs);

        manager.get(Shader_LPVAccumulate, vs, ps);
        accumulatePS_ = reinterpret_cast<LPVAccumulatePS*>(ps);

        manager.get(Shader_LPVRender, vs, ps);
        renderVS_ = reinterpret_cast<LPVRenderVS*>(vs);
        renderPS_ = reinterpret_cast<LPVRenderPS*>(ps);

        if(false == reflectiveShadowMap_.initialize(reflectiveMapSize)){
            return false;
        }

        if(false == LPVBuffer_.reset(volumeResolution_, volumeResolution_, volumeResolution_)){
            return false;
        }

        {
#ifdef RENDER_ENABLE_INDIPENDENT_ALPHA
            s32 blendEnable[lgraphics::MaxRenderTargets] =
            {
                FALSE, FALSE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE,
            };

            lgraphics::BlendType blendType[lgraphics::MaxRenderTargets] =
            {
                lgraphics::Blend_One, lgraphics::Blend_One, lgraphics::Blend_One, lgraphics::Blend_One, lgraphics::Blend_One, lgraphics::Blend_One, lgraphics::Blend_One, lgraphics::Blend_One,
            };

            lgraphics::BlendOp blendOp[lgraphics::MaxRenderTargets] =
            {
                lgraphics::BlendOp_Add, lgraphics::BlendOp_Add, lgraphics::BlendOp_Add, lgraphics::BlendOp_Add, lgraphics::BlendOp_Add, lgraphics::BlendOp_Add, lgraphics::BlendOp_Add, lgraphics::BlendOp_Add,
            };

            u8 writeMask[lgraphics::MaxRenderTargets] =
            {
                0x0FU, 0x0FU, 0x0FU, 0x0FU, 0x0FU, 0x0FU, 0x0FU, 0x0FU,
            };

            propagateBlendState_ = lgraphics::BlendState::createIndipendent(
                FALSE,
                blendEnable,
                blendType, blendType, blendOp,
                blendType, blendType, blendOp,
                writeMask);
            return propagateBlendState_.valid();
#else
            //propagateBlendState_ = lgraphics::BlendState::create(
            //    FALSE,
            //    TRUE,
            //    lgraphics::Blend_One, lgraphics::Blend_One, lgraphics::BlendOp_Add,
            //    lgraphics::Blend_One, lgraphics::Blend_One, lgraphics::BlendOp_Add,
            //    0x0FU);
            return true;
#endif
        }
    }

    void LightPropagationVolume::release()
    {
        reflectiveShadowMap_.release();
        LPVBuffer_.release();
    }


    void LightPropagationVolume::beginReflectiveShadowMap(
        lgraphics::GraphicsDeviceRef& device,
        const lscene::Scene& scene)
    {
        calcLightViewProjectionForVolume(scene);

        device.getRenderTargets(1, &oldColor_, &oldDepthStencil_);

        reflectiveShadowMap_.begin(device);
    }

    void LightPropagationVolume::beginLPVInjection(
            lgraphics::GraphicsDeviceRef& device,
            const lscene::Scene& /*scene*/)
    {
        ID3D11RenderTargetView* const views[] =
        {
            LPVBuffer_.viewSHCoeffRTemp_[0].getView(),
            LPVBuffer_.viewSHCoeffGTemp_[0].getView(),
            LPVBuffer_.viewSHCoeffBTemp_[0].getView(),
        };

        device.setRenderTargets(3, views, NULL);
        device.setViewport(0, 0, volumeResolution_, volumeResolution_);

        const f32 color[] = {0.0f, 0.0f, 0.0f, 0.0f};
        device.clearRenderTargetView(LPVBuffer_.viewSHCoeffRTemp_[0].getView(), color);
        device.clearRenderTargetView(LPVBuffer_.viewSHCoeffGTemp_[0].getView(), color);
        device.clearRenderTargetView(LPVBuffer_.viewSHCoeffBTemp_[0].getView(), color);

        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        device.setInputLayout(NULL);
        device.setVertexBuffers(0, 0, NULL, NULL, NULL);
        device.setPrimitiveTopology(lgraphics::Primitive_PointList);

        injectionVS_->attach();
        injectionGS_->attach();
        injectionPS_->attach();

        LPVInjectionVS::Constant1 injectionConstants1;
        injectionConstants1.mivp_ = lightViewProjection_;
        injectionConstants1.mivp_.invert();

        injectionConstants1.LPVMin_ = LPVMin_;

        injectionVS_->updateConstant(injectionConstants1);

        injectionGS_->update();

        ID3D11ShaderResourceView* const resViews[] =
        {
            *reflectiveShadowMap_.getTexColor().getView(),
            *reflectiveShadowMap_.getTexNormal().getView(),
            *reflectiveShadowMap_.getTexDepth().getView(),
        };
        device.setVSResources(0, 3, resViews);

        device.draw(numVertices_, 0);

        //Occlusion Injection
        //----------------------------------
        if(enableOcclusion_){
            ID3D11RenderTargetView* const occViews[] =
            {
                LPVBuffer_.viewSHCoeffOcclusionTemp_.getView(),
            };
            device.setRenderTargets(1, occViews, NULL);

            occlusionInjectionVS_->attach();
            occlusionInjectionPS_->attach();

            device.draw(numVertices_, 0);
        }

        //
        device.setGeometryShader(NULL);

        device.clearVSResources(3);
        //device.clearPSResources(3);
    }

    void LightPropagationVolume::beginLPVPropagate(
        lgraphics::GraphicsDeviceRef& device)
    {
        device.setViewport(0, 0, volumeResolution_, volumeResolution_);

        const f32 color[] = {0.0f, 0.0f, 0.0f, 0.0f};
        device.clearRenderTargetView(LPVBuffer_.viewSHCoeffR_.getView(), color);
        device.clearRenderTargetView(LPVBuffer_.viewSHCoeffG_.getView(), color);
        device.clearRenderTargetView(LPVBuffer_.viewSHCoeffB_.getView(), color);

        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        device.setInputLayout(NULL);
        device.setVertexBuffers(0, 0, NULL, NULL, NULL);
        device.setPrimitiveTopology(lgraphics::Primitive_TriangleStrip);

        propagateVS_->attach();
        propagateGS_->attach();

#ifdef RENDER_ENABLE_INDIPENDENT_ALPHA

        device.setBlendState(propagateBlendState_.get());

        static const s32 NumPSResources = 4;
        ID3D11ShaderResourceView* resources[NumPSResources];
        if(enableOcclusion_){
            occlusionPropagatePS_->attach();
            occlusionPropagatePS_->update();

            device.setPSSamplers(0, 1, LPVBuffer_.texSHCoeffOcclusionTemp_.getSampler());

            resources[3] = *LPVBuffer_.texSHCoeffOcclusionTemp_.getView();

        }else{
            propagatePS_->attach();
            resources[3] = NULL;
        }

        ID3D11RenderTargetView* targets[6];
        targets[3] = LPVBuffer_.viewSHCoeffR_.getView();
        targets[4] = LPVBuffer_.viewSHCoeffG_.getView();
        targets[5] = LPVBuffer_.viewSHCoeffB_.getView();

        u32 index = 0;
        for(u32 i=1; i<NumIterations; ++i){
            device.clearRenderTargets(3);

            resources[0] = *LPVBuffer_.texSHCoeffRTemp_[index].getView();
            resources[1] = *LPVBuffer_.texSHCoeffGTemp_[index].getView();
            resources[2] = *LPVBuffer_.texSHCoeffBTemp_[index].getView();
            device.setPSResources(0, NumPSResources, resources);

            index = (index + 1) & 0x01U;

            targets[0] = LPVBuffer_.viewSHCoeffRTemp_[index].getView();
            targets[1] = LPVBuffer_.viewSHCoeffGTemp_[index].getView();
            targets[2] = LPVBuffer_.viewSHCoeffBTemp_[index].getView();
            device.setRenderTargets(6, targets, NULL);

            device.drawInstanced(4, volumeResolution_, 0, 0);
        }

#else
        device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_NoAlpha);

        static const s32 NumPSResources = 3*NumIterations;
        ID3D11ShaderResourceView* resources[NumPSResources];
        if(enableOcclusion_){
            occlusionPropagatePS_->attach();
            occlusionPropagatePS_->update();

            device.setPSSamplers(0, 1, LPVBuffer_.texSHCoeffOcclusionTemp_.getSampler());

            resources[3] = *LPVBuffer_.texSHCoeffOcclusionTemp_.getView();

        }else{
            propagatePS_->attach();
            resources[3] = NULL;
        }

        ID3D11RenderTargetView* targets[3];
        for(u32 i=1; i<NumIterations; ++i){
            targets[0] = LPVBuffer_.viewSHCoeffRTemp_[i].getView();
            targets[1] = LPVBuffer_.viewSHCoeffGTemp_[i].getView();
            targets[2] = LPVBuffer_.viewSHCoeffBTemp_[i].getView();
            device.setRenderTargets(3, targets, NULL);

            resources[0] = *LPVBuffer_.texSHCoeffRTemp_[i-1].getView();
            resources[1] = *LPVBuffer_.texSHCoeffGTemp_[i-1].getView();
            resources[2] = *LPVBuffer_.texSHCoeffBTemp_[i-1].getView();
            device.setPSResources(0, 3, resources);

            device.drawInstanced(4, volumeResolution_, 0, 0);
        }

        accumulatePS_->attach();

        targets[0] = LPVBuffer_.viewSHCoeffR_.getView();
        targets[1] = LPVBuffer_.viewSHCoeffG_.getView();
        targets[2] = LPVBuffer_.viewSHCoeffB_.getView();
        device.setRenderTargets(3, targets, NULL);

        for(u32 i=0; i<NumIterations; ++i){
            resources[3*i + 0] = *LPVBuffer_.texSHCoeffRTemp_[i].getView();
            resources[3*i + 1] = *LPVBuffer_.texSHCoeffGTemp_[i].getView();
            resources[3*i + 2] = *LPVBuffer_.texSHCoeffBTemp_[i].getView();
        }
        device.setPSResources(0, NumPSResources, resources);
        device.drawInstanced(4, volumeResolution_, 0, 0);
#endif
    }

    void LightPropagationVolume::end(lgraphics::GraphicsDeviceRef& device)
    {
#ifdef RENDER_ENABLE_INDIPENDENT_ALPHA
        device.clearPSResources(4);
#else
        device.clearPSResources(3*NumIterations);
#endif
        device.setPixelShader(NULL);
        device.setGeometryShader(NULL);
        device.setVertexShader(NULL);

        device.setRenderTargets(1, &oldColor_, oldDepthStencil_);
        SAFE_RELEASE(oldDepthStencil_);
        SAFE_RELEASE(oldColor_);
    }


    void LightPropagationVolume::calcLightViewProjectionForVolume(const lscene::Scene& scene)
    {
        const lmath::Matrix44& view = scene.getViewMatrix();
        const lmath::Matrix44& lightView = scene.getLightEnv().getDirectionalLight().getLightView();

        lmath::Matrix44 invView(view);
        invView.invert();

        lmath::Vector4 frustumPoints[8];
        lscene::Frustum frustum;
        frustum.calcInView(scene.getCamera(), scene.getCamera().getZNear(), zfar_);
        frustum.getPoints(frustumPoints, scene.getCamera().getZNear(), -LPVCellSize_*1.7320508f * 3.0f);

        lmath::Vector4 lightCameraOrthoMin(FLT_MAX, FLT_MAX, FLT_MAX, 1.0f);
        lmath::Vector4 lightCameraOrthoMax(-FLT_MAX, -FLT_MAX, -FLT_MAX, -1.0f);
        LPVMin_ = lightCameraOrthoMin;
        lmath::Vector4 LPVMax = lightCameraOrthoMax;

        lmath::Vector4 worldPoint;
        for(s32 i=0; i<8; ++i){
            //ワールド座標へ変換
            frustumPoints[i].mul(invView, frustumPoints[i]);

            LPVMin_.minimum(LPVMin_, frustumPoints[i]);
            LPVMax.maximum(LPVMax, frustumPoints[i]);
        }

        LPVMin_ += LPVMax;
        LPVMin_ *= 0.5f;

        f32 size = LPVCellSize_ * volumeResolution_;
        LPVMin_.sub(0.5f*size);

        LPVMin_ *= invLPVCellSize_;
        LPVMin_.floor();
        LPVMin_ *= LPVCellSize_;

        LPVMax = LPVMin_;
        LPVMax.add(size);

        frustumPoints[0].set(LPVMin_.x_, LPVMin_.y_, LPVMin_.z_, 0.0f);
        frustumPoints[1].set(LPVMax.x_, LPVMin_.y_, LPVMin_.z_, 0.0f);
        frustumPoints[2].set(LPVMax.x_, LPVMax.y_, LPVMin_.z_, 0.0f);
        frustumPoints[3].set(LPVMin_.x_, LPVMax.y_, LPVMin_.z_, 0.0f);

        frustumPoints[4].set(LPVMin_.x_, LPVMin_.y_, LPVMax.z_, 0.0f);
        frustumPoints[5].set(LPVMax.x_, LPVMin_.y_, LPVMax.z_, 0.0f);
        frustumPoints[6].set(LPVMax.x_, LPVMax.y_, LPVMax.z_, 0.0f);
        frustumPoints[7].set(LPVMin_.x_, LPVMax.y_, LPVMax.z_, 0.0f);

        for(s32 i=0; i<8; ++i){

            //ライトの座標へ変換
            worldPoint.mul(lightView, frustumPoints[i]);
            lightCameraOrthoMin.minimum(lightCameraOrthoMin, worldPoint);
            lightCameraOrthoMax.maximum(lightCameraOrthoMax, worldPoint);
        }
        lightViewProjection_.orthoOffsetCenter(
            lightCameraOrthoMin.x_,
            lightCameraOrthoMax.x_,
            lightCameraOrthoMax.y_,
            lightCameraOrthoMin.y_,
            lightCameraOrthoMin.z_,
            lightCameraOrthoMax.z_);

        lightViewProjection_.mul(lightViewProjection_, lightView);
        
        LPVMin_.w_ = invLPVCellSize_;
        viewZ_.set(0.0f, 0.0f, lightCameraOrthoMax.z_ - lightCameraOrthoMin.z_, lightCameraOrthoMin.z_);

    }
}
