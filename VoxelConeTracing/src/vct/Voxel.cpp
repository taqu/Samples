/**
@file Voxel.cpp
@author t-sakai
@date 2014/09/08 create
*/
#include "Voxel.h"
#include <lframework/scene/Scene.h>
#include "render/ShaderManager.h"
#include "render/Shader.h"

namespace vct
{
    Voxel::Voxel()
        :resolution_(0)
        ,mipmapLevels_(1)
        ,superSamples_(3)
        ,voxelDiameter_(0.0f)
        ,invVoxelDiameter_(1.0f)
        ,voxelizeVS_(NULL)
        ,voxelizeGS_(NULL)
        ,voxelizePS_(NULL)
        ,voxelRenderVS_(NULL)
        ,voxelRenderPS_(NULL)
        ,reflectiveShadowMapVS_(NULL)
        ,reflectiveShadowMapPS_(NULL)
        ,reflectiveSHadowMapTextureVS_(NULL)
        ,reflectiveSHadowMapTexturePS_(NULL)
    {
        voxelArea_.voxelCenter_.zero();
        voxelArea_.voxelWorldSize_.zero();
        direction_.zero();
        projection_.identity();
        view_.identity();

        lightDirection_.zero();
    }

    Voxel::~Voxel()
    {
        destroy();
    }

    void Voxel::create(s32 resolution, s32 mipmapLevels)
    {
        destroy();

        resolution_ = resolution;
        mipmapLevels_ = mipmapLevels;

#if 0
        f32 resolution2 = resolution_*resolution_;
        invVoxelDiameter_ = lmath::sqrt(resolution2 + resolution2 + resolution2);
        voxelDiameter_ = 1.0f/invVoxelDiameter_;
#else
        invVoxelDiameter_ = resolution_;
        voxelDiameter_ = 1.0f/resolution_;
#endif
        render::ShaderManager& shaderManager = render::ShaderManager::getInstance();

        render::ShaderVSBase* vs;
        render::ShaderPSBase* ps;
        render::ShaderGSBase* gs;
        shaderManager.get(render::Shader_Voxelize, vs, ps);
        shaderManager.get(render::ShaderGS_Voxelize, gs);
        voxelizeVS_ = reinterpret_cast<render::VoxelizeVS*>(vs);
        voxelizePS_ = reinterpret_cast<render::VoxelizePS*>(ps);
        voxelizeGS_ = reinterpret_cast<render::VoxelizeGS*>(gs);

        shaderManager.get(render::Shader_VoxelRender, vs, ps);
        shaderManager.get(render::ShaderGS_VoxelRender, gs);
        voxelRenderVS_ = reinterpret_cast<render::VoxelRenderVS*>(vs);
        voxelRenderPS_ = reinterpret_cast<render::VoxelRenderPS*>(ps);
        voxelRenderGS_ = reinterpret_cast<render::VoxelRenderGS*>(gs);

        shaderManager.get(render::Shader_ReflectiveShadowMap, vs, ps);
        reflectiveShadowMapVS_ = reinterpret_cast<render::ReflectiveShadowMapVS*>(vs);
        reflectiveShadowMapPS_ = reinterpret_cast<render::ReflectiveShadowMapPS*>(ps);

        shaderManager.get(render::Shader_ReflectiveShadowMapTexture, vs, ps);
        reflectiveSHadowMapTextureVS_ = reinterpret_cast<render::ReflectiveShadowMapTextureVS*>(vs);
        reflectiveSHadowMapTexturePS_ = reinterpret_cast<render::ReflectiveShadowMapTexturePS*>(ps);

        shaderManager.get(render::Shader_VoxelInjection, vs, ps);
        voxelInjectionVS_ = reinterpret_cast<render::VoxelInjectionVS*>(vs);
        voxelInjectionPS_ = reinterpret_cast<render::VoxelInjectionPS*>(ps);

        shaderManager.get(render::Shader_VoxelMipmap, vs, ps);
        voxelMipmapVS_ = reinterpret_cast<render::VoxelMipmapVS*>(vs);
        voxelMipmapPS_ = reinterpret_cast<render::VoxelMipmapPS*>(ps);


        {
            lgraphics::SRVDesc srvDesc;
            srvDesc.dimension_ = lgraphics::ViewSRVDimension_Buffer;
            srvDesc.format_ = lgraphics::Data_R32_UInt;
            srvDesc.buffer_.firstElement_ = 0;
            srvDesc.buffer_.numElements_ = resolution*resolution*resolution;

            lgraphics::UAVDesc uavDesc;
            uavDesc.dimension_ = lgraphics::UAVDimension_Buffer;
            uavDesc.format_ = lgraphics::Data_R32_TypeLess;
            uavDesc.buffer_.firstElement_ = 0;
            uavDesc.buffer_.numElements_ = resolution*resolution*resolution;
            uavDesc.buffer_.flags_ = lgraphics::UAVFlag_RAW;

            for(s32 i=0; i<6; ++i){
                voxelBuffers_[i] = lgraphics::Texture::createBuffer(
                    resolution*resolution*resolution*4,
                    lgraphics::Usage_Default,
                    lgraphics::BindFlag_ShaderResource | lgraphics::BindFlag_UnorderedAccess,
                    lgraphics::CPUAccessFlag_None,
                    lgraphics::ResourceMisc_BufferAllowRawView,
                    4,
                    NULL,
                    &srvDesc);

                voxelBufferUAVViews_[i] = voxelBuffers_[i].createUAView(uavDesc);
            }
        }

        {
            lgraphics::SRVDesc srvDesc;
            srvDesc.dimension_ = lgraphics::ViewSRVDimension_Texture3D;
            srvDesc.format_ = lgraphics::Data_R8G8B8A8_UNorm;
            srvDesc.tex3D_.mostDetailedMip_ = 0;
            srvDesc.tex3D_.mipLevels_ = mipmapLevels_;

            lgraphics::UAVDesc uavDesc;
            uavDesc.dimension_ = lgraphics::UAVDimension_Texture3D;
            uavDesc.format_ = lgraphics::Data_R8G8B8A8_UNorm;
            uavDesc.tex3D_.mipSlice_ = 0;
            uavDesc.tex3D_.firstWSlice_ = 0;
            uavDesc.tex3D_.wsize_ = -1;

            for(s32 i=0; i<6; ++i){
                srvDesc.tex3D_.mostDetailedMip_ = 0;
                srvDesc.tex3D_.mipLevels_ = mipmapLevels_;

                voxelTextures_[i] = lgraphics::Texture::create3D(
                    resolution_,
                    resolution_,
                    resolution_,
                    mipmapLevels_,
                    lgraphics::Data_R8G8B8A8_UNorm,
                    lgraphics::Usage_Default,
                    lgraphics::BindFlag_ShaderResource | lgraphics::BindFlag_UnorderedAccess,
                    lgraphics::CPUAccessFlag_None,
                    lgraphics::ResourceMisc_None,
                    lgraphics::TexFilter_MinMagMipLinear,
                    lgraphics::TexAddress_Border,
                    lgraphics::Cmp_Less,
                    0.0f,
                    NULL,
                    &srvDesc);

                for(s32 j=0; j<mipmapLevels_; ++j){
                    srvDesc.tex3D_.mostDetailedMip_ = j;
                    srvDesc.tex3D_.mipLevels_ = 1;
                    voxelSRVViews_[i][j] = voxelTextures_[i].createSRView(srvDesc);

                    uavDesc.tex3D_.mipSlice_ = j;
                    voxelUAVViews_[i][j] = voxelTextures_[i].createUAView(uavDesc);
                }
            }
        }

        Constants0 tmpConstants0;
        tmpConstants0.voxelSize_[0] = resolution_;
        tmpConstants0.voxelSize_[1] = resolution_*resolution_;
        tmpConstants0.voxelSize_[2] = resolution_*superSamples_;
        tmpConstants0.voxelSize_[3] = superSamples_;
        tmpConstants0.voxelInvSize_.set(1.0f/resolution_, 1.0f/(resolution_*resolution_), 0.5f/resolution_, 1.0f/(resolution_-1));
        constants0_ = lgraphics::ConstantBuffer::create(
            sizeof(Constants0),
            lgraphics::Usage_Immutable,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            sizeof(Constants0),
            &tmpConstants0);


        constants1_ = lgraphics::ConstantBuffer::create(
            sizeof(Constants1),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        constants2_ = lgraphics::ConstantBuffer::create(
            sizeof(Constants2),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        constants3_ = lgraphics::ConstantBuffer::create(
            sizeof(Constants3),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        constants4_ = lgraphics::ConstantBuffer::create(
            sizeof(Constants4),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);
    }

    void Voxel::destroy()
    {
        constants4_.destroy();
        constants3_.destroy();
        constants2_.destroy();
        constants1_.destroy();
        constants0_.destroy();

        for(s32 i=5; 0<=i; --i){
            for(s32 j=MaxMipLevels-1; 0<=j; --j){
                voxelUAVViews_[i][j].destroy();
                voxelSRVViews_[i][j].destroy();
            }
            voxelTextures_[i].destroy();
        }

        for(s32 i=5; 0<=i; --i){
            voxelBufferUAVViews_[i].destroy();
            voxelBuffers_[i].destroy();
        }
    }

    void Voxel::setView(const lmath::Vector4& center, const lmath::Vector4& direction, const lmath::Vector4& size, s32 reflectiveResolution)
    {
        voxelArea_.voxelCenter_ = center;
        direction_ = direction;
        lmath::Vector4 at;
        at.add(voxelArea_.voxelCenter_, direction_);
        view_.lookAt(voxelArea_.voxelCenter_, at, lmath::Vector4(0.0f, 1.0f, 0.0f, 0.0f));

        voxelArea_.voxelWorldSize_ = size;
        f32 halfz = voxelArea_.voxelWorldSize_.z_ * 0.5f;
        projection_.ortho(voxelArea_.voxelWorldSize_.x_, voxelArea_.voxelWorldSize_.y_, -halfz, halfz);

        constants1_.update(0, NULL, &voxelArea_, 0, 0);


        //-----------------------------------------------------
        if(reflectiveShadowMap_.getResolution() != reflectiveResolution){
            lgraphics::SRVDesc srvDesc;
            srvDesc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
            srvDesc.format_ = lgraphics::Data_R10G10B10A2_UNorm;
            srvDesc.tex2D_.mostDetailedMip_ = 0;
            srvDesc.tex2D_.mipLevels_ = 1;

            texReflectiveShadowMap_ = lgraphics::Texture::create2D(
                reflectiveResolution,
                reflectiveResolution,
                1,
                1,
                lgraphics::Data_R10G10B10A2_UNorm,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource | lgraphics::BindFlag_RenderTarget,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagLinearMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Less,
                0.0f,
                NULL,
                &srvDesc);

            lgraphics::RTVDesc rtvDesc;
            rtvDesc.dimension_ = lgraphics::ViewRTVDimension_Texture2D;
            rtvDesc.format_ = lgraphics::Data_R10G10B10A2_UNorm;
            rtvDesc.tex2D_.mipSlice_ = 0;
            rtReflectiveShadowMap_ = texReflectiveShadowMap_.createRTView(rtvDesc);


            srvDesc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
            srvDesc.format_ = lgraphics::Data_R32_Float;
            srvDesc.tex2D_.mostDetailedMip_ = 0;
            srvDesc.tex2D_.mipLevels_ = 1;

            texReflectiveDepth_ = lgraphics::Texture::create2D(
                reflectiveResolution,
                reflectiveResolution,
                1,
                1,
                lgraphics::Data_R32_TypeLess,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource | lgraphics::BindFlag_DepthStencil,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Less,
                0.0f,
                NULL,
                &srvDesc);

            lgraphics::DSVDesc dsvDesc;
            dsvDesc.dimension_ = lgraphics::ViewDSVDimension_Texture2D;
            dsvDesc.format_ = lgraphics::Data_D32_Float;
            dsvDesc.tex2D_.mipSlice_ = 0;
            dsReflectiveDepth_ = texReflectiveDepth_.createDSView(dsvDesc);
        }

        lightDirection_.zero();
        reflectiveShadowMap_.initialize(reflectiveResolution);

        {//ボクセルエリアのAABB
            lmath::Vector4 half = size;
            half *= 0.5f;
            lmath::Vector4 aabbMin;
            aabbMin.sub(center, half);
            lmath::Vector4 aabbMax;
            aabbMax.add(center, half);

            lmath::Vector4 worldPos[8];
            worldPos[0].set(aabbMin.x_, aabbMin.y_, aabbMin.z_, 0.0f);
            worldPos[1].set(aabbMax.x_, aabbMin.y_, aabbMin.z_, 0.0f);
            worldPos[2].set(aabbMin.x_, aabbMax.y_, aabbMin.z_, 0.0f);
            worldPos[3].set(aabbMax.x_, aabbMax.y_, aabbMin.z_, 0.0f);
            worldPos[4].set(aabbMin.x_, aabbMin.y_, aabbMax.z_, 0.0f);
            worldPos[5].set(aabbMax.x_, aabbMin.y_, aabbMax.z_, 0.0f);
            worldPos[6].set(aabbMin.x_, aabbMax.y_, aabbMax.z_, 0.0f);
            worldPos[7].set(aabbMax.x_, aabbMax.y_, aabbMax.z_, 0.0f);

            aabbMin.set(FLT_MAX, FLT_MAX, FLT_MAX, 0.0f);
            aabbMax.set(-FLT_MAX, -FLT_MAX, -FLT_MAX, 0.0f);

            lmath::Matrix44 invView = view_;
            invView.invert();
            for(s32 i=0; i<8; ++i){
                worldPos[i].mul(invView, worldPos[i]);
                aabbMin.minimum(aabbMin, worldPos[i]);
                aabbMax.maximum(aabbMax, worldPos[i]);
            }
            reflectiveShadowMap_.setSceneAABB(aabbMin, aabbMax);
        }
    }

    void Voxel::getViewProjection(lmath::Matrix44& viewProj)
    {
        viewProj.mul(projection_, view_);
    }

    void Voxel::clearVoxels(lgraphics::GraphicsDeviceRef& device)
    {
        for(s32 i=0; i<6; ++i){
            for(s32 j=0; j<mipmapLevels_; ++j){
                device.clearUnorderedAccessView(voxelUAVViews_[i][j].getView(), lgraphics::GraphicsDeviceRef::UIZero);
            }
        }
    }

    void Voxel::begin(lgraphics::GraphicsDeviceRef& device)
    {
        ID3D11UnorderedAccessView* const uavViews[] =
        {
            voxelBufferUAVViews_[0].getView(),
            voxelBufferUAVViews_[1].getView(),
            voxelBufferUAVViews_[2].getView(),
            voxelBufferUAVViews_[3].getView(),
            voxelBufferUAVViews_[4].getView(),
            voxelBufferUAVViews_[5].getView(),
        };


        device.setRenderTargetsAndUAV(0, NULL, NULL, 0, 6, uavViews, NULL);
        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolidNoCull);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        device.setViewport(0, 0, resolution_*superSamples_, resolution_*superSamples_);

        for(s32 i=0; i<6; ++i){
            device.clearUnorderedAccessView(voxelBufferUAVViews_[i].getView(), lgraphics::GraphicsDeviceRef::UIZero);
        }

        constants0_.setPS(0);
        voxelizeVS_->attach();
        voxelizeGS_->attach();
        voxelizePS_->attach();
    }

    void Voxel::end(lgraphics::GraphicsDeviceRef& device)
    {
        //インジェクション
        ID3D11UnorderedAccessView* uavViews[] =
        {
            voxelUAVViews_[0][0].getView(),
            voxelUAVViews_[1][0].getView(),
            voxelUAVViews_[2][0].getView(),
            voxelUAVViews_[3][0].getView(),
            voxelUAVViews_[4][0].getView(),
            voxelUAVViews_[5][0].getView(),
        };


        device.setRenderTargetsAndUAV(0, NULL, NULL, 0, 6, uavViews, NULL);
        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolidNoCull);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);
        device.setViewport(0, 0, resolution_, resolution_);
        device.setPrimitiveTopology(lgraphics::Primitive_PointList);
        device.setInputLayout(NULL);
        device.setVertexBuffers(0, 0, NULL, NULL, NULL);

        constants0_.setVS(0);
        constants1_.setVS(1);
        constants2_.setVS(2);

        constants3_.setPS(0);
        texReflectiveShadowMap_.attachPS(0, 0);
        texReflectiveDepth_.attachPS(1, 1);

        ID3D11ShaderResourceView* srvViews[] =
        {
            *voxelBuffers_[0].getView(),
            *voxelBuffers_[1].getView(),
            *voxelBuffers_[2].getView(),
            *voxelBuffers_[3].getView(),
            *voxelBuffers_[4].getView(),
            *voxelBuffers_[5].getView(),
        };
        device.setPSResources(2, 6, srvViews);

        voxelInjectionVS_->attach();
        device.setGeometryShader(NULL);
        voxelInjectionPS_->attach();

        device.draw(resolution_*resolution_*resolution_, 0);


        //
        voxelMipmapVS_->attach();
        voxelMipmapPS_->attach();

        constants4_.setVS(0);

        Constants4 tmpConstants4;
        s32 resolution = resolution_;
        f32 invResolution;
        for(s32 i=1; i<mipmapLevels_; ++i){
            for(s32 j=0; j<6; ++j){
                srvViews[j] = voxelSRVViews_[j][i-1].getView();
                uavViews[j] = voxelUAVViews_[j][i].getView();
            }
            device.setRenderTargetsAndUAV(0, NULL, NULL, 0, 6, uavViews, NULL);
            device.setPSResources(0, 6, srvViews);

            resolution >>= 1;
            invResolution = 1.0f/resolution;

            tmpConstants4.resolution_ = resolution;
            tmpConstants4.resolution2_ = resolution*resolution;
            tmpConstants4.invResolution_ = invResolution;
            tmpConstants4.invResolution2_ = invResolution * invResolution; 

            constants4_.update(0, NULL, &tmpConstants4, 0, 0);
            device.draw(resolution*resolution*resolution, 0);
        }

        device.clearPSResources(8);
        device.setVertexShader(NULL);
        device.setPixelShader(NULL);
        device.restoreDefaultRenderTargets();
    }

    void Voxel::beginReflective(lgraphics::GraphicsDeviceRef& device, const lscene::Scene& scene)
    {
        //ライトの方向が変わったら更新
        const lscene::LightEnvironment& lightEnv = scene.getLightEnv();
        const lmath::Vector4& lightDirection = lightEnv.getDirectionalLight().getDirection();
        if(false == lightDirection.isEqual(direction_)){
            direction_ = lightDirection;
            reflectiveShadowMap_.update(scene);

            const lmath::Matrix44& lightView = lightEnv.getDirectionalLight().getLightView();


            lmath::Matrix44 world = view_;
            world.invert();

            lmath::Matrix44 wv;
            wv.mul(lightView, world);
            lmath::Matrix44 mlwvp;
            mlwvp.mul(reflectiveShadowMap_.getLightProjection(), wv);
            constants2_.update(0, NULL, &mlwvp, 0, 0);

            lmath::Vector4 size2 = voxelArea_.voxelWorldSize_;
            size2 /= resolution_;
            size2 *= size2;

            f32 diag = lmath::sqrt(size2.x_+size2.y_+size2.z_);
            f32 invZExtent = reflectiveShadowMap_.getLightProjection().m_[2][2];

            Constants3 tmpConstant3;
            tmpConstant3.depthBias_ = diag*invZExtent*2.0f;
            constants3_.update(0, NULL, &tmpConstant3, 0, 0);
        }

        device.setRenderTargets(1, rtReflectiveShadowMap_.get(), dsReflectiveDepth_.getView());
        device.clearRenderTargetView(rtReflectiveShadowMap_.getView(), lgraphics::GraphicsDeviceRef::Zero);
        device.clearDepthStencilView(dsReflectiveDepth_.getView(), lgraphics::ClearFlag_Depth, 1.0f, 0);

        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolid);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWEnable);
        device.setViewport(0, 0, reflectiveShadowMap_.getResolution(), reflectiveShadowMap_.getResolution());
    }

    void Voxel::endReflective(lgraphics::GraphicsDeviceRef& device)
    {
    }

    void Voxel::setVoxelTextureToPS(lgraphics::GraphicsDeviceRef& device, s32 startIndex, s32 samplerIndex)
    {
        ID3D11ShaderResourceView* const srvViews[] =
        {
            *voxelTextures_[0].getView(),
            *voxelTextures_[1].getView(),
            *voxelTextures_[2].getView(),
            *voxelTextures_[3].getView(),
            *voxelTextures_[4].getView(),
            *voxelTextures_[5].getView(),
        };
        device.setPSResources(startIndex, 6, srvViews);
        device.setPSSamplers(samplerIndex, 1, voxelTextures_[0].getSampler());
    }

    void Voxel::getVoxelParam(
        f32& voxelDiameter,
        f32& invVoxelDiameter,
        lmath::Matrix44& worldToVoxelSpace,
        lmath::Vector4& voxelScale)
    {
        voxelDiameter = voxelDiameter_;
        invVoxelDiameter = invVoxelDiameter_;
        worldToVoxelSpace = view_;

        lmath::Vector4 invSize;
        invSize.one();
        invSize /= voxelArea_.voxelWorldSize_;

        voxelScale.set(invSize.x_, invSize.y_, invSize.z_, 0.0f);
    }

    void Voxel::draw(lgraphics::GraphicsDeviceRef& device, const lscene::Scene& scene)
    {
        device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillSolid);
        //device.setRasterizerState(lgraphics::GraphicsDeviceRef::Rasterizer_FillWireFrameNoCull);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWEnable);
        device.setPrimitiveTopology(lgraphics::Primitive_PointList);
        device.setInputLayout(NULL);
        device.setVertexBuffers(0, 0, NULL, NULL, NULL);

        lmath::Matrix44 invView = view_;
        invView.invert();

        lmath::Matrix44 mwvp;
        mwvp.mul(scene.getViewProjMatrix(), invView);

        constants0_.setVS(0);
        constants1_.setVS(1);

        voxelizeVS_->setGS(0, mwvp);
        constants0_.setGS(1);
        constants1_.setGS(2);

        voxelRenderVS_->attach();
        voxelRenderGS_->attach();
        voxelRenderPS_->attach();

#if 0
        ID3D11ShaderResourceView* const uavViews[] =
        {
            *voxelBuffers_[0].getView(),
            *voxelBuffers_[1].getView(),
            *voxelBuffers_[2].getView(),
            *voxelBuffers_[3].getView(),
            *voxelBuffers_[4].getView(),
            *voxelBuffers_[5].getView(),
        };
#else
        ID3D11ShaderResourceView* const srcViews[] =
        {
            *voxelTextures_[0].getView(),
            *voxelTextures_[1].getView(),
            *voxelTextures_[2].getView(),
            *voxelTextures_[3].getView(),
            *voxelTextures_[4].getView(),
            *voxelTextures_[5].getView(),
        };
#endif
        device.setGSResources(0, 6, srcViews);

        device.draw(resolution_*resolution_*resolution_, 0);
        device.clearGSResources(6);

        device.setVertexShader(NULL);
        device.setGeometryShader(NULL);
        device.setPixelShader(NULL);
    }
}
