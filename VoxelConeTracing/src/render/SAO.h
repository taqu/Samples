#ifndef INC_RENDER_SAO_H__
#define INC_RENDER_SAO_H__
/**
@file SAO.h
@author t-sakai
@date 2013/12/03 create
*/
#include "render/render.h"
#include <lmath/lmath.h>
#include <lgraphics/api/ConstantBufferRef.h>
#include <lgraphics/api/TextureRef.h>


namespace render
{
    class FullQuadVS;
    class ReconstructZPS;
    class Downsample2x2PS;
    class CopyPS;
    class SAOPS;
    class SAOBlurPS;

    class SAO
    {
    public:
        static const s32 MaxMipLevels = 5;

        SAO();
        ~SAO();

        void destroy();
        bool reset(u32 width, u32 height, const lmath::Matrix44& proj, f32 znear, f32 zfar);

        void clearTarget(lgraphics::GraphicsDeviceRef& device);
        void run(lgraphics::GraphicsDeviceRef& device, f32 znear, f32 zfar);

        lgraphics::Texture2DRef& get(){ return texAO_;}
        //lgraphics::Texture2DRef& get(){ return depth_;}
       
        void resetProjectionParam(f32 znear, f32 zfar, bool perspective);
    private:
        static s32 getHalfSize(s32 v);

        struct SAOParam
        {
            s32 size_[2];
            f32 radius_;
            f32 radius2_;
            f32 proj_[4];
            f32 scale_;
            f32 bias_;
            f32 intensity_;
            f32 padding0_;
        };

        struct SAOBlurParam
        {
            s32 size_[2];
            f32 axis_[2];
        };

        s32 width_[MaxMipLevels];
        s32 height_[MaxMipLevels];

        lgraphics::Texture2DRef texAO_;
        lgraphics::RenderTargetViewRef rtAO_;

        lgraphics::Texture2DRef texAOBlur_;
        lgraphics::RenderTargetViewRef rtAOBlur_;

        lgraphics::Texture2DRef depth_;
        lgraphics::ShaderResourceViewRef srView_[MaxMipLevels];
        lgraphics::RenderTargetViewRef rtView_[MaxMipLevels];

        lgraphics::Texture2DRef tmpDepth_;
        lgraphics::ShaderResourceViewRef tmpSrView_[MaxMipLevels-1];
        lgraphics::RenderTargetViewRef tmpRtView_[MaxMipLevels-1];

        lgraphics::SamplerStateRef samplerState_;

        render::FullQuadVS* fullQuadVS_;
        render::ReconstructZPS* reconstructPS_;
        render::Downsample2x2PS* downsample2x2PS_;
        render::CopyPS* copyPS_;
        render::SAOPS* saoPS_;
        render::SAOBlurPS* saoBlurPS_;

        lgraphics::ConstantBufferRef reconstruct_;
        lgraphics::ConstantBufferRef downsample_;
        lgraphics::ConstantBufferRef saoConstant_;
        lgraphics::ConstantBufferRef saoBlurConstant_;

        SAOParam saoParam_;
    };
}

#endif //INC_RENDER_SAO_H__
