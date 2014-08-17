/**
@file Glare.cpp
@author t-sakai
@date 2013/07/11 create
*/
#include "Glare.h"
#include "render/ShaderManager.h"
#include "render/Shader.h"

namespace render
{
namespace
{
    f32 gaussian(f32 x, f32 rho)
    {
        f32 g = 1.0f/(lmath::sqrt(2.0f*PI)*rho);
        g *= lmath::exp(-(x*x)/(2.0f*rho*rho));
        return g;
    }
}

    const f32 Glare::GaussianDeviation = 2.3f;

    Glare::Glare()
        :width_(0)
        ,height_(0)
        ,fullQuadVS_(NULL)
        ,glarePS_(NULL)
        ,gaussPS_(NULL)
    {
    }

    Glare::~Glare()
    {
    }

    void Glare::destroy()
    {
        gaussConstant0Buffer_.destroy();

        for(s32 i=0; i<2; ++i){
            rtView_[i].destroy();
            texture_[i].destroy();
        }
    }

    bool Glare::reset(u32 width, u32 height)
    {
        destroy();
        width_ = width;
        height_ = height;
        invWidth_ = 1.0f/width_;
        invHeight_ = 1.0f/height;

        for(s32 i=0; i<2; ++i){
            lgraphics::ResourceViewDesc desc;
            desc.dimension_ = lgraphics::ViewSRVDimension_Texture2D;
            desc.format_ = lgraphics::Data_R16G16B16A16_Float;
            desc.tex2D_.mipLevels_ = 1;
            desc.tex2D_.mostDetailedMip_ = 0;

            texture_[i] = lgraphics::Texture::create2D(
                width,
                height,
                1,
                1,
                lgraphics::Data_R16G16B16A16_Float,
                lgraphics::Usage_Default,
                lgraphics::BindFlag_ShaderResource | lgraphics::BindFlag_RenderTarget,
                lgraphics::CPUAccessFlag_None,
                lgraphics::ResourceMisc_None,
                lgraphics::TexFilter_MinMagLinearMipPoint,
                lgraphics::TexAddress_Clamp,
                lgraphics::Cmp_Never,
                0.0f,
                NULL,
                &desc);

            lgraphics::RTVDesc rtvDesc;
            rtvDesc.dimension_ = lgraphics::ViewRTVDimension_Texture2D;
            rtvDesc.format_ = lgraphics::Data_R16G16B16A16_Float;
            rtvDesc.tex2D_.mipSlice_ = 0;

            rtView_[i] = texture_[i].createRTView(rtvDesc);
        }


        render::ShaderManager& shaderManager = render::ShaderManager::getInstance();
        render::ShaderVSBase* vs;
        render::ShaderPSBase* ps;

        shaderManager.get(
            render::Shader_FullQuad,
            vs,
            ps);

        fullQuadVS_ = reinterpret_cast<FullQuadVS*>(vs);

        shaderManager.get(
            render::Shader_Glare,
            vs,
            ps);

        glarePS_ = reinterpret_cast<GlarePS*>(ps);

        shaderManager.get(
            render::Shader_GaussianFilter,
            vs,
            ps);

        gaussPS_ = reinterpret_cast<GaussPS*>(ps);

        gaussConstant0Buffer_ = lgraphics::ConstantBuffer::create(
            sizeof(GaussConstant0),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);


        //ガウシアンフィルタのウェイト
        gaussConstant0_.weight_[0] = gaussian(0.0f, GaussianDeviation);

        f32 total = gaussConstant0_.weight_[0];
        for(s32 i=1; i<=FilterHalf; ++i){
            gaussConstant0_.weight_[i] = gaussian(static_cast<f32>(i), GaussianDeviation);
            total += gaussConstant0_.weight_[i];
            total += gaussConstant0_.weight_[i];
        }

        //正規化
        f32 inv = 1.0f/total;
        for(s32 i=0; i<=FilterHalf; ++i){
            gaussConstant0_.weight_[i] *= inv;
        }
        return true;
    }

    void Glare::run(lgraphics::GraphicsDeviceRef& device, lgraphics::Texture2DRef& texture)
    {
        device.setViewport(0, 0, width_, height_);

        ID3D11RenderTargetView* views[1];
        views[0] = rtView_[0].getView();
        device.setRenderTargets(1, views, NULL);

        fullQuadVS_->attach();
        glarePS_->attach();
        texture.attachPS(0,0);
        device.draw(3, 0);

        gaussPS_->attach();

        gaussConstant0Buffer_.setPS(0);

        //水平方向
        device.clearPSResources(1);

        views[0] = rtView_[1].getView();
        device.setRenderTargets(1, views, NULL);
        texture_[0].attachPS(0,0);

        gaussConstant0_.offset_[0].set(0.0f, 0.0f);
        for(s32 i=1; i<+FilterHalf; ++i){
            gaussConstant0_.offset_[i].set(invWidth_*i, 0.0f);
        }
        gaussConstant0Buffer_.update(0, NULL, &gaussConstant0_, 0, 0);

        device.draw(3, 0);

        //垂直方向
        device.clearPSResources(1);

        views[0] = rtView_[0].getView();
        device.setRenderTargets(1, views, NULL);
        texture_[1].attachPS(0,0);

        gaussConstant0_.offset_[0].set(0.0f, 0.0f);
        for(s32 i=1; i<=FilterHalf; ++i){
            gaussConstant0_.offset_[i].set(0.0f, invHeight_*i);
        }
        gaussConstant0Buffer_.update(0, NULL, &gaussConstant0_, 0, 0);

        device.draw(3, 0);
    }
}
