/**
@file LPVBuffer.cpp
@author t-sakai
@date 2012/06/10 create
*/
#include "LPVBuffer.h"

#include <lgraphics/lgraphicsAPIInclude.h>
#include <lgraphics/lgraphics.h>

namespace render
{
    LPVBuffer::LPVBuffer()
    {
    }

    LPVBuffer::~LPVBuffer()
    {
    }

    void LPVBuffer::release()
    {
        for(u32 i=0; i<NumBuffers; ++i){
            viewSHCoeffRTemp_[i].destroy();
            texSHCoeffRTemp_[i].destroy();
            viewSHCoeffGTemp_[i].destroy();
            texSHCoeffGTemp_[i].destroy();
            viewSHCoeffBTemp_[i].destroy();
            texSHCoeffBTemp_[i].destroy();
        }

        viewSHCoeffOcclusionTemp_.destroy();
        texSHCoeffOcclusionTemp_.destroy();

        viewSHCoeffR_.destroy();
        texSHCoeffR_.destroy();
        viewSHCoeffG_.destroy();
        texSHCoeffG_.destroy();
        viewSHCoeffB_.destroy();
        texSHCoeffB_.destroy();

        viewSHCoeffOcclusion_.destroy();
        texSHCoeffOcclusion_.destroy();
    }

    bool LPVBuffer::reset(u32 width, u32 height, u32 depth)
    {
        bool ret = true;

        for(u32 i=0; i<NumBuffers; ++i){
            ret &= create(texSHCoeffRTemp_[i], viewSHCoeffRTemp_[i], width, height, depth);
            ret &= create(texSHCoeffGTemp_[i], viewSHCoeffGTemp_[i], width, height, depth);
            ret &= create(texSHCoeffBTemp_[i], viewSHCoeffBTemp_[i], width, height, depth);
        }
        ret &= create(texSHCoeffOcclusionTemp_, viewSHCoeffOcclusionTemp_, width, height, depth);

        ret &= create(texSHCoeffR_, viewSHCoeffR_, width, height, depth);
        ret &= create(texSHCoeffG_, viewSHCoeffG_, width, height, depth);
        ret &= create(texSHCoeffB_, viewSHCoeffB_, width, height, depth);

        ret &= create(texSHCoeffOcclusion_, viewSHCoeffOcclusion_, width, height, depth);

        return ret;
    }

    bool LPVBuffer::create(lgraphics::Texture3DRef& tex, lgraphics::RenderTargetViewRef& view, u32 width, u32 height, u32 depth)
    {
        lgraphics::ResourceViewDesc desc;
        desc.dimension_ = lgraphics::ViewSRVDimension_Texture3D;
        desc.format_ = lgraphics::Data_R16G16B16A16_Float;
        desc.tex3D_.mipLevels_ = 1;
        desc.tex3D_.mostDetailedMip_ = 0;

        tex = lgraphics::Texture::create3D(
            width,
            height,
            depth,
            1,
            lgraphics::Data_R16G16B16A16_Float,
            lgraphics::Usage_Default,
            lgraphics::BindFlag_ShaderResource|lgraphics::BindFlag_RenderTarget,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None,
            lgraphics::TexFilter_MinMagLinearMipPoint,
            lgraphics::TexAddress_Border,
            lgraphics::Cmp_Never,
            0.0f,
            NULL,
            &desc);

        lgraphics::RTVDesc rtvDesc;
        rtvDesc.dimension_ = lgraphics::ViewRTVDimension_Texture3D;
        rtvDesc.format_ = lgraphics::Data_R16G16B16A16_Float;
        rtvDesc.tex3D_.mipSlice_ = 0;
        rtvDesc.tex3D_.firstWSlice_ = 0;
        rtvDesc.tex3D_.wSize_ = depth;

        view = tex.createRTView(rtvDesc);
        return view.valid();
    }
}
