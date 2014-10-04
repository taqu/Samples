/**
@file SAO.cpp
@author t-sakai
@date 2013/12/03 create
*/
#include "SAO.h"
#include "ShaderManager.h"
#include "Shader.h"

namespace render
{
    s32 SAO::getHalfSize(s32 v)
    {
        return ((v&0x01U) != 0)? (v+1) >> 1 : v>>1;
    }

    SAO::SAO()
        :fullQuadVS_(NULL)
        ,reconstructPS_(NULL)
        ,downsample2x2PS_(NULL)
        ,copyPS_(NULL)
        ,saoPS_(NULL)
        ,saoBlurPS_(NULL)
    {
    }

    SAO::~SAO()
    {
    }

    void SAO::destroy()
    {
        saoBlurConstant_.destroy();
        saoConstant_.destroy();
        downsample_.destroy();
        reconstruct_.destroy();

        samplerState_.destroy();

        for(s32 i=MaxMipLevels-2; 0<=i; --i){
            tmpSrView_[i].destroy();
            tmpRtView_[i].destroy();
        }

        tmpDepth_.destroy();

        for(s32 i=MaxMipLevels-1; 0<=i; --i){
            srView_[i].destroy();
            rtView_[i].destroy();
        }
        depth_.destroy();

        rtAOBlur_.destroy();
        texAOBlur_.destroy();

        rtAO_.destroy();
        texAO_.destroy();
    }

    bool SAO::reset(u32 width, u32 height, const lmath::Matrix44& proj, f32 znear, f32 zfar)
    {
        lgraphics::DataFormat saoFormat = lgraphics::Data_R8G8B8A8_UNorm;
        lgraphics::DataFormat mipmapFormat = lgraphics::Data_R32_Float;

        destroy();
        width_[0] = width;
        height_[0] = height;
        for(s32 i=1; i<MaxMipLevels; ++i){
            width_[i] = getHalfSize(width_[i-1]);
            height_[i] = getHalfSize(height_[i-1]);
        }

        lgraphics::ResourceViewDesc srvDesc;
        srvDesc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
        srvDesc.format_ = saoFormat;
        srvDesc.tex2D_.mipLevels_ = 1;
        srvDesc.tex2D_.mostDetailedMip_ = 0;

        lgraphics::RTVDesc rtvDesc;
        rtvDesc.dimension_ = lgraphics::ViewRTVDimension_Texture2D;
        rtvDesc.format_ = saoFormat;
        rtvDesc.tex2D_.mipSlice_ = 0;

        texAO_ = lgraphics::Texture::create2D(
            width,
            height,
            1,
            1,
            saoFormat,
            lgraphics::Usage_Default,
            lgraphics::BindFlag_ShaderResource | lgraphics::BindFlag_RenderTarget,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            lgraphics::TexFilter_MinMagLinearMipPoint,
            lgraphics::TexAddress_Clamp,
            lgraphics::Cmp_Never,
            1.0f,
            NULL,
            &srvDesc);

        rtAO_ = texAO_.createRTView(rtvDesc);

        texAOBlur_ = lgraphics::Texture::create2D(
            width,
            height,
            1,
            1,
            saoFormat,
            lgraphics::Usage_Default,
            lgraphics::BindFlag_ShaderResource | lgraphics::BindFlag_RenderTarget,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            lgraphics::TexFilter_MinMagLinearMipPoint,
            lgraphics::TexAddress_Clamp,
            lgraphics::Cmp_Never,
            1.0f,
            NULL,
            &srvDesc);

        rtAOBlur_ = texAOBlur_.createRTView(rtvDesc);

        srvDesc.format_ = mipmapFormat;
        srvDesc.tex2D_.mipLevels_ = MaxMipLevels;
        srvDesc.tex2D_.mostDetailedMip_ = 0;

        depth_ = lgraphics::Texture::create2D(
            width,
            height,
            MaxMipLevels,
            1,
            mipmapFormat,
            lgraphics::Usage_Default,
            lgraphics::BindFlag_ShaderResource | lgraphics::BindFlag_RenderTarget,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            lgraphics::TexFilter_MinMagMipLinear,
            lgraphics::TexAddress_Clamp,
            lgraphics::Cmp_Never,
            1.0f,
            NULL,
            &srvDesc);


        srvDesc.tex2D_.mipLevels_ = 1;

        rtvDesc.format_ = mipmapFormat;

        for(s32 i=0; i<MaxMipLevels; ++i){
            rtvDesc.tex2D_.mipSlice_ = i;
            rtView_[i] = depth_.createRTView(rtvDesc);

            srvDesc.tex2D_.mostDetailedMip_ = i;
            srView_[i] = depth_.createSRView(srvDesc);
        }

        tmpDepth_ = lgraphics::Texture::create2D(
            width_[1],
            height_[1],
            MaxMipLevels-1,
            1,
            mipmapFormat,
            lgraphics::Usage_Default,
            lgraphics::BindFlag_ShaderResource | lgraphics::BindFlag_RenderTarget,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            lgraphics::TexFilter_MinMagMipPoint,
            lgraphics::TexAddress_Clamp,
            lgraphics::Cmp_Never,
            1.0f,
            NULL,
            NULL);

        for(s32 i=0; i<MaxMipLevels-1; ++i){
            rtvDesc.tex2D_.mipSlice_ = i;
            tmpRtView_[i] = tmpDepth_.createRTView(rtvDesc);

            srvDesc.tex2D_.mostDetailedMip_ = i;
            tmpSrView_[i] = tmpDepth_.createSRView(srvDesc);
        }

        samplerState_ = lgraphics::SamplerState::create(
            lgraphics::TexFilter_MinMagMipLinear,
            lgraphics::TexAddress_Clamp,
            lgraphics::TexAddress_Clamp,
            lgraphics::TexAddress_Clamp,
            lgraphics::Cmp_Never,
            1.0f);

        render::ShaderManager& shaderManager = render::ShaderManager::getInstance();
        render::ShaderVSBase* vs;
        render::ShaderPSBase* ps;

        shaderManager.get(
            render::Shader_FullQuad,
            vs,
            ps);

        fullQuadVS_ = reinterpret_cast<FullQuadVS*>(vs);

        shaderManager.get(
            render::Shader_ReconstructZ,
            vs,
            ps);

        reconstructPS_ = reinterpret_cast<ReconstructZPS*>(ps);

        shaderManager.get(
            render::Shader_Downsample2x2,
            vs,
            ps);

        downsample2x2PS_ = reinterpret_cast<Downsample2x2PS*>(ps);

        shaderManager.get(
            render::Shader_Copy,
            vs,
            ps);

        copyPS_ = reinterpret_cast<CopyPS*>(ps);

        shaderManager.get(
            render::Shader_SAO,
            vs,
            ps);

        saoPS_ = reinterpret_cast<SAOPS*>(ps);

        shaderManager.get(
            render::Shader_SAOBlur,
            vs,
            ps);

        saoBlurPS_ = reinterpret_cast<SAOBlurPS*>(ps);

        reconstruct_ = lgraphics::ConstantBuffer::create(
            sizeof(lmath::Vector4),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        downsample_ = lgraphics::ConstantBuffer::create(
            sizeof(u32)*4,
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        saoConstant_ = lgraphics::ConstantBuffer::create(
            sizeof(SAOParam),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        saoBlurConstant_ = lgraphics::ConstantBuffer::create(
            sizeof(SAOBlurParam),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

        saoParam_.size_[0] = width;
        saoParam_.size_[1] = height;
        saoParam_.radius_ = 0.2f;
        saoParam_.radius2_ = saoParam_.radius_*saoParam_.radius_;

        saoParam_.proj_[0] = -2.0f/(proj.m_[0][0] * width);
        saoParam_.proj_[1] = -2.0f/(proj.m_[1][1] * height);
        saoParam_.proj_[2] = (1.0f - proj.m_[0][2]) / proj.m_[0][0];
        saoParam_.proj_[3] = (1.0f + proj.m_[1][2]) / proj.m_[1][1];

        saoParam_.scale_ = -proj.m_[1][1]*0.5f*height;
        saoParam_.bias_ = 0.012f;
        saoParam_.intensity_ = 0.9f /lmath::pow(saoParam_.radius_, 6.0f);

        saoConstant_.update(0, NULL, &saoParam_, 0, 0);

        bool perspective = lmath::isZero(proj.m_[3][3]);
        resetProjectionParam(znear, zfar, perspective);
        return true;
    }

    void SAO::resetProjectionParam(f32 znear, f32 zfar, bool perspective)
    {
        lmath::Vector4 clipInfo;

        if(perspective){
            clipInfo.x_ = znear * zfar;
            clipInfo.y_ = -(znear - zfar);
            clipInfo.z_ = -zfar;
            clipInfo.w_ = 0.0f;
        }else{
            clipInfo.x_ = -znear;
            clipInfo.y_ = -1.0f;
            clipInfo.z_ = 1.0f;
            clipInfo.w_ = 0.0f;
        }

        reconstruct_.update(0, NULL, &clipInfo, 0, 0);
    }

    void SAO::clearTarget(lgraphics::GraphicsDeviceRef& device)
    {
        device.clearRenderTargetView(rtAO_.getView(), lgraphics::GraphicsDeviceRef::One);
    }

    void SAO::run(lgraphics::GraphicsDeviceRef& device, f32 znear, f32 zfar)
    {
        device.setViewport(0, 0, width_[0], height_[0]);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);

        device.setRenderTargets(1, rtView_[0].get(), NULL);

        ID3D11ShaderResourceView* resourceView[1] = { device.getDepthStencilShaderResourceView() };
        device.setPSResources(0, 1, resourceView);

        device.setPSSamplers(0, 1, samplerState_.get());

        reconstruct_.setPS(0);

        fullQuadVS_->attach();
        reconstructPS_->attach();
        device.draw(3, 0);

        device.clearPSResources(1);
        s32 level[4];

        for(s32 i=1; i<MaxMipLevels; ++i){
            downsample2x2PS_->attach();

            device.setViewport(0, 0, width_[i], height_[i]);

            device.setRenderTargets(1, tmpRtView_[i-1].get(), NULL);

            device.setPSResources(0, 1, srView_[i-1].get());

            level[0] = i;
            level[1] = width_[i];
            level[2] = height_[i];
            downsample_.update(0, NULL, level, 0, 0);

            downsample_.setPS(0);
            device.draw(3, 0);

            copyPS_->attach();

            device.setRenderTargets(1, rtView_[i].get(), NULL);
            device.setPSResources(0, 1, tmpSrView_[i-1].get());
            device.draw(3, 0);
        }

        device.setViewport(0, 0, width_[0], height_[0]);
        device.setRenderTargets(1, rtAO_.get(), NULL);

        saoConstant_.setPS(0);
        saoPS_->attach();
        depth_.attachPS(0);
        device.draw(3, 0);

        //Bilateral filtering
        //------------------------------------------------------
        SAOBlurParam saoBlurParam;
        saoBlurParam.size_[0] = width_[0];
        saoBlurParam.size_[1] = height_[0];

        saoBlurConstant_.setPS(0);
        saoBlurPS_->attach();

        //Horizontal
        device.setRenderTargets(1, rtAOBlur_.get(), NULL);

        saoBlurParam.axis_[0] = 0.0f;
        saoBlurParam.axis_[1] = 1.0f;
        saoBlurConstant_.update(0, NULL, &saoBlurParam, 0, 0);

        texAO_.attachPS(0);
        device.draw(3, 0);
        device.clearPSResources(1);

        //Vertical
        device.setRenderTargets(1, rtAO_.get(), NULL);
        saoBlurParam.axis_[0] = 1.0f;
        saoBlurParam.axis_[1] = 0.0f;
        saoBlurConstant_.update(0, NULL, &saoBlurParam, 0, 0);

        texAOBlur_.attachPS(0);
        device.draw(3, 0);

        device.clearPSResources(1);
        device.restoreDefaultRenderTargets();
    }
}
