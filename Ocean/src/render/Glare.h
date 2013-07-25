#ifndef INC_RENDER_GLARE_H__
#define INC_RENDER_GLARE_H__
/**
@file Glare.h
@author t-sakai
@date 2013/07/11 create
*/
#include "render.h"
#include <lmath/lmath.h>
#include <lgraphics/api/ConstantBufferRef.h>
#include <lgraphics/api/TextureRef.h>

namespace render
{
    class FullQuadVS;
    class GlarePS;
    class GaussPS;

    class Glare
    {
    public:
        static const f32 GaussianDeviation;

        //static const u32 FilterWidth = 7;
        //static const u32 FilterHalf = FilterWidth>>1;

        static const u32 FilterWidth = 15;
        static const u32 FilterHalf = FilterWidth>>1;

        Glare();
        ~Glare();

        void destroy();
        bool reset(u32 width, u32 height);

        void run(lgraphics::GraphicsDeviceRef& device, lgraphics::Texture2DRef& texture);

        lgraphics::Texture2DRef& getResult(){ return texture_[0];}

    private:
        struct GaussConstant0
        {
            lmath::Vector2 offset_[FilterHalf+1];
            f32 weight_[FilterHalf+1];
        };

        u32 width_;
        u32 height_;
        f32 invWidth_;
        f32 invHeight_;

        lgraphics::Texture2DRef texture_[2];
        lgraphics::RenderTargetViewRef rtView_[2];

        render::FullQuadVS* fullQuadVS_;
        render::GlarePS* glarePS_;
        render::GaussPS* gaussPS_;

        GaussConstant0 gaussConstant0_;
        lgraphics::ConstantBufferRef gaussConstant0Buffer_;
    };
}
#endif //INC_RENDER_GLARE_H__
