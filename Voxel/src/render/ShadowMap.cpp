/**
@file ShadowMap.cpp
@author t-sakai
@date 2012/08/27 create
*/
#include "stdafx.h"

#include "ShadowMap.h"

#include <lframework/scene/Scene.h>

#include "ShaderManager.h"
#include "Shader.h"

namespace render
{
    ShadowMap::ShadowMap()
        :size_(0)
        ,depthVS_(NULL)
        ,depthPS_(NULL)
    {
    }

    ShadowMap::~ShadowMap()
    {
    }

    void ShadowMap::release()
    {
        viewDepth_.destroy();
        texDepth_.destroy();
    }

    bool ShadowMap::reset(u32 size)
    {
        size_ = size;

        ShaderManager& manager = ShaderManager::getInstance();
        ShaderVSBase* vs = NULL;
        ShaderPSBase* ps = NULL;

        manager.get(Shader_Depth, vs, ps);
        depthVS_ = reinterpret_cast<DepthVS*>(vs);
        depthPS_ = reinterpret_cast<DepthPS*>(ps);

        return create(texDepth_, viewDepth_, size);
    }

    void ShadowMap::begin(lgraphics::GraphicsDeviceRef& device)
    {
        ID3D11RenderTargetView * const targets[] =
        {
            NULL,
        };

        device.setRenderTargets(1, targets, viewDepth_.getView());
        device.setViewport(0, 0, size_, size_);

        device.clearDepthStencilView(viewDepth_.getView(), lgraphics::ClearFlag_Depth, 1.0f, 0);

        device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_NoAlpha);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWEnable);

        depthVS_->attach();
        depthPS_->attach();
    }

    void ShadowMap::end(lgraphics::GraphicsDeviceRef& device)
    {
        device.restoreDefaultRenderTargets();
    }

    bool ShadowMap::create(lgraphics::Texture2DRef& tex, lgraphics::DepthStencilViewRef& view, u32 size)
    {
        lgraphics::ResourceViewDesc desc;
        desc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
        desc.format_ = lgraphics::Data_R32_Float;
        desc.tex2D_.mostDetailedMip_ = 0;
        desc.tex2D_.mipLevels_ = 1;

        tex = lgraphics::Texture::create2D(
            size,
            size,
            1,
            1,
            lgraphics::Data_R32_TypeLess,
            lgraphics::Usage_Default,
            lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_DepthStencil,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            lgraphics::TexFilter_MinMagLinearMipPoint,
            lgraphics::TexAddress_Border,
            lgraphics::Cmp_Never,
            255.0f,
            NULL,
            &desc);

        lgraphics::DSVDesc dsvDesc;
        dsvDesc.format_ = lgraphics::Data_D32_Float;
        dsvDesc.dimension_ = lgraphics::ViewDSVDimension_Texture2D;
        dsvDesc.tex2D_.mipSlice_ = 0;

        view = tex.createDSView(dsvDesc);
        return view.valid();
    }
}
