/**
@file ReflectiveShadowMap.cpp
@author t-sakai
@date 2012/05/29 create
*/
#include "stdafx.h"

#include "ReflectiveShadowMap.h"

#include "render/ShaderManager.h"
#include "render/Shader.h"

namespace render
{
    ReflectiveShadowMap::ReflectiveShadowMap()
        :size_(0)
        ,invSize_(1.0f)
    {
    }

    ReflectiveShadowMap::~ReflectiveShadowMap()
    {
    }

    bool ReflectiveShadowMap::initialize(u32 size)
    {
        LASSERT(size>0);
        size_ = size;
        invSize_ = 1.0f/size;

        ShaderManager& manager = ShaderManager::getInstance();
        ShaderVSBase* vs = NULL;
        ShaderPSBase* ps = NULL;
        manager.get(Shader_ReflectiveShadowMap, vs, ps);
        vs_ = reinterpret_cast<ReflectiveShadowMapVS*>(vs);
        ps_ = reinterpret_cast<ReflectiveShadowMapPS*>(ps);

        lgraphics::ResourceViewDesc viewDesc;
        lgraphics::RTVDesc rtvDesc;

        {
            viewDesc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
            viewDesc.format_ = lgraphics::Data_B8G8R8A8_UNorm_SRGB;
            viewDesc.tex2D_.mipLevels_ = 1;
            viewDesc.tex2D_.mostDetailedMip_ = 0;

            rtvDesc.format_ = lgraphics::Data_B8G8R8A8_UNorm_SRGB;
            rtvDesc.dimension_ = lgraphics::ViewRTVDimension_Texture2D;
            rtvDesc.tex2D_.mipSlice_ = 0;

            texColor_ = lgraphics::Texture::create2D(
                size_,
                size_,
                1,
                1,
                lgraphics::Data_B8G8R8A8_UNorm_SRGB,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_RenderTarget,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Never,
                0.0f,
                NULL,
                &viewDesc);

            viewColor_ = texColor_.createRTView(rtvDesc);
        }

        {
            viewDesc.format_ = lgraphics::Data_B8G8R8A8_UNorm;
            viewDesc.tex2D_.mipLevels_ = 1;
            viewDesc.tex2D_.mostDetailedMip_ = 0;

            rtvDesc.format_ = lgraphics::Data_B8G8R8A8_UNorm;
            rtvDesc.dimension_ = lgraphics::ViewRTVDimension_Texture2D;
            rtvDesc.tex2D_.mipSlice_ = 0;

            texNormal_ = lgraphics::Texture::create2D(
                size_,
                size_,
                1,
                1,
                lgraphics::Data_B8G8R8A8_UNorm,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_RenderTarget,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Never,
                0.0f,
                NULL,
                &viewDesc);

            viewNormal_ = texNormal_.createRTView(rtvDesc);
        }

        {
            viewDesc.format_ = lgraphics::Data_R32_Float;
            viewDesc.tex2D_.mostDetailedMip_ = 0;
            viewDesc.tex2D_.mipLevels_ = 1;

            lgraphics::DSVDesc dsvDesc;
            dsvDesc.format_ = lgraphics::Data_D32_Float;
            dsvDesc.dimension_ = lgraphics::ViewDSVDimension_Texture2D;
            dsvDesc.tex2D_.mipSlice_ = 0;

            texDepth_ = lgraphics::Texture::create2D(
                size_,
                size_,
                1,
                1,
                lgraphics::Data_R32_TypeLess,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_DepthStencil,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Never,
                0.0f,
                NULL,
                &viewDesc);

            viewDepth_ = texDepth_.createDSView(dsvDesc);
        }

        return (texColor_.valid()
            && texNormal_.valid()
            && texDepth_.valid());
    }

    void ReflectiveShadowMap::release()
    {
        viewDepth_.destroy();
        texDepth_.destroy();

        viewNormal_.destroy();
        texNormal_.destroy();

        viewColor_.destroy();
        texColor_.destroy();
    }

    void ReflectiveShadowMap::begin(lgraphics::GraphicsDeviceRef& device)
    {
        ID3D11RenderTargetView * const targets[] =
        {
            viewColor_.getView(),
            viewNormal_.getView(),
        };

        device.setRenderTargets(2, targets, viewDepth_.getView());
        device.setViewport(0, 0, size_, size_);
        
        const f32 color[] = {0.0f, 0.0f, 0.0f, 0.0f};
        device.clearRenderTargetView(viewColor_.getView(), color);
        device.clearRenderTargetView(viewNormal_.getView(), color);
        device.clearDepthStencilView(viewDepth_.getView(), lgraphics::ClearFlag_Depth, 1.0f, 0);

        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWEnable);

        vs_->attach();
        ps_->attach();
    }

}
