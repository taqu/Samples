/**
@file CascadedShadowMap.cpp
@author t-sakai
@date 2013/09/25 create
*/
#include "stdafx.h"

#include "CascadedShadowMap.h"

#include <lframework/scene/Scene.h>

#include "ShaderManager.h"
#include "Shader.h"
#include "System.h"

namespace render
{
    CascadedShadowMap::CascadedShadowMap()
        :size_(0)
        ,depthVS_(NULL)
        ,depthGS_(NULL)
        ,depthPS_(NULL)
    {
    }

    CascadedShadowMap::~CascadedShadowMap()
    {
    }

    void CascadedShadowMap::release()
    {
        viewDepth_.destroy();
        texDepth_.destroy();
    }

    bool CascadedShadowMap::reset(u32 size, s32 numCascades)
    {
        lscene::Scene& scene = fractal::System::getScene();

        ShaderManager& manager = ShaderManager::getInstance();
        ShaderVSBase* vs = NULL;
        ShaderPSBase* ps = NULL;

        size_ = size;
        manager.get(Shader_Depth, vs, ps);
        depthVS_ = reinterpret_cast<DepthVS*>(vs);
        depthPS_ = reinterpret_cast<DepthPS*>(ps);

        ShaderGSBase* gs = NULL;
        manager.get(ShaderGS_Depth, gs);
        depthGS_ = reinterpret_cast<DepthGS*>(gs);

        return create(size, numCascades);
    }

    void CascadedShadowMap::begin(lgraphics::GraphicsDeviceRef& device)
    {
        lscene::Scene& scene = fractal::System::getScene();

        ID3D11DepthStencilView* viewDepth = viewDepth_.getView();

        device.setRenderTargets(0, NULL, viewDepth);
        device.setViewport(0, 0, size_, size_);

        const f32 color[] = {1.0f, 1.0f, 1.0f, 1.0f};
        //device.clearRenderTargetView(viewBlur_.getView(), color);
        device.clearDepthStencilView(viewDepth_.getView(), lgraphics::ClearFlag_Depth, 1.0f, 0);

        device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_NoAlpha);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWEnable);

        depthVS_->attach();
        depthGS_->attach();
        device.setPixelShader(NULL);
    }

    void CascadedShadowMap::end(lgraphics::GraphicsDeviceRef& device, s32 numCascades)
    {
        device.setVertexShader(NULL);
        device.setGeometryShader(NULL);
        device.setPixelShader(NULL);

        device.clearPSResources(1);
        device.restoreDefaultRenderTargets();
    }

    bool CascadedShadowMap::create(u32 size, s32 numCascades)
    {
        {
            lgraphics::ResourceViewDesc desc;

            desc.dimension_ = lgraphics::ViewSRVDimension_Texture2DArray;
            desc.format_ = lgraphics::Data_R16_UNorm;
            desc.tex2DArray_.mostDetailedMip_ = 0;
            desc.tex2DArray_.mipLevels_ = 1;
            desc.tex2DArray_.firstArraySlice_ = 0;
            desc.tex2DArray_.arraySize_ = numCascades;

            texDepth_ = lgraphics::Texture::create2D(
                size,
                size,
                1,
                numCascades,
                lgraphics::Data_R16_TypeLess,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_DepthStencil,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_CompMinMagLinearMipPoint,
                lgraphics::TexAddress_Border,
                lgraphics::Cmp_Less,
                1.0f,
                NULL,
                &desc);

            lgraphics::DSVDesc dsvDesc;
            dsvDesc.format_ = lgraphics::Data_D16_UNorm;
            dsvDesc.dimension_ = lgraphics::ViewDSVDimension_Texture2DArray;
            dsvDesc.tex2DArray_.mipSlice_ = 0;
            dsvDesc.tex2DArray_.firstArraySlice_ = 0;
            dsvDesc.tex2DArray_.arraySize_ = numCascades;

            viewDepth_ = texDepth_.createDSView(dsvDesc);
        }

        return viewDepth_.valid();
    }
}
