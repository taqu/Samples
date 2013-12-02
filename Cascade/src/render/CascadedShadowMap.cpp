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
        :depthVS_(NULL)
        ,depthGS_(NULL)
        ,depthPS_(NULL)
        ,depthBlurVS_(NULL)
        ,depthBlurGS_(NULL)
        ,depthBlurPS_(NULL)
    {
    }

    CascadedShadowMap::~CascadedShadowMap()
    {
    }

    void CascadedShadowMap::release()
    {
        viewBlur_.destroy();
        texBlur_.destroy();

        viewDepth_.destroy();
        texDepth_.destroy();
    }

    bool CascadedShadowMap::reset(u32 size, s32 numCascades)
    {
        lscene::Scene& scene = fractal::System::getScene();

        scene.setShadowMapSize(size);
        scene.setNumCascades(numCascades);

        ShaderManager& manager = ShaderManager::getInstance();
        ShaderVSBase* vs = NULL;
        ShaderPSBase* ps = NULL;

        manager.get(Shader_Depth, vs, ps);
        depthVS_ = reinterpret_cast<DepthVS*>(vs);
        depthPS_ = reinterpret_cast<DepthPS*>(ps);

        manager.get(Shader_DepthBlur, vs, ps);
        depthBlurVS_ = reinterpret_cast<DepthBlurVS*>(vs);
        depthBlurPS_ = reinterpret_cast<DepthBlurPS*>(ps);

        ShaderGSBase* gs = NULL;
        manager.get(ShaderGS_Depth, gs);
        depthGS_ = reinterpret_cast<DepthGS*>(gs);

        manager.get(ShaderGS_DepthBlur, gs);
        depthBlurGS_ = reinterpret_cast<DepthBlurGS*>(gs);

        return create(size, numCascades);
    }

    void CascadedShadowMap::begin(lgraphics::GraphicsDeviceRef& device)
    {
        lscene::Scene& scene = fractal::System::getScene();

        ID3D11RenderTargetView* viewDepthArray = viewBlur_.getView();
        ID3D11DepthStencilView* viewDepth = viewDepth_.getView();

        device.setRenderTargets(1, &viewDepthArray, viewDepth);

        device.setViewport(0, 0, scene.getShadowMapSize(), scene.getShadowMapSize());

        const f32 color[] = {1.0f, 1.0f, 1.0f, 1.0f};
        device.clearRenderTargetView(viewBlur_.getView(), color);
        device.clearDepthStencilView(viewDepth_.getView(), lgraphics::ClearFlag_Depth, 1.0f, 0);

        device.setBlendState(lgraphics::GraphicsDeviceRef::BlendState_NoAlpha);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DEnableWEnable);

        depthVS_->attach();
        depthGS_->attach();
        depthPS_->attach();
    }

    void CascadedShadowMap::end(lgraphics::GraphicsDeviceRef& device, s32 numCascades)
    {
#if 0
        ID3D11RenderTargetView* viewBlurArray = viewBlur_.getView();
        ID3D11DepthStencilView* viewDepth = NULL;

        device.setRenderTargets(1, &viewBlurArray, viewDepth);
        device.setDepthStencilState(lgraphics::GraphicsDeviceRef::DepthStencil_DDisableWDisable);

        device.setInputLayout(NULL);
        device.setVertexBuffers(0, 0, NULL, NULL, NULL);
        device.setPrimitiveTopology(lgraphics::Primitive_TriangleList);

        depthBlurVS_->attach();
        depthBlurGS_->attach();
        depthBlurPS_->attach();

        texDepth_.attachPS(0, 0);

        device.drawInstanced(3, numCascades, 0, 0);
#endif
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
                lgraphics::TexFilter_MinMagMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Never,
                0.0f,
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

        {
            lgraphics::ResourceViewDesc desc;

            desc.dimension_ = lgraphics::ViewSRVDimension_Texture2DArray;
            desc.format_ = lgraphics::Data_R16G16_Float;
            desc.tex2DArray_.mostDetailedMip_ = 0;
            desc.tex2DArray_.mipLevels_ = 1;
            desc.tex2DArray_.firstArraySlice_ = 0;
            desc.tex2DArray_.arraySize_ = numCascades;

            texBlur_ = lgraphics::Texture::create2D(
                size,
                size,
                1,
                numCascades,
                lgraphics::Data_R16G16_Float,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_RenderTarget,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagLinearMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Never,
                0.0f,
                NULL,
                &desc);

            lgraphics::RTVDesc rtvDesc;
            rtvDesc.format_ = lgraphics::Data_R16G16_Float;
            rtvDesc.dimension_ = lgraphics::ViewRTVDimension_Texture2DArray;
            rtvDesc.tex2DArray_.mipSlice_ = 0;
            rtvDesc.tex2DArray_.firstArraySlice_ = 0;
            rtvDesc.tex2DArray_.arraySize_ = numCascades;

            viewBlur_ = texBlur_.createRTView(rtvDesc);
        }

        return viewDepth_.valid();
    }
}
