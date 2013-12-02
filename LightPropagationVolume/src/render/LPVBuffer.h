#ifndef INC_RENDER_LPVBUFFER_H__
#define INC_RENDER_LPVBUFFER_H__
/**
@file LPVBuffer.h
@author t-sakai
@date 2012/06/10 create
*/
#include <lgraphics/api/TextureRef.h>
#include "render/render.h"

namespace render
{
    class LPVBuffer
    {
    public:

#ifdef RENDER_ENABLE_INDIPENDENT_ALPHA
        static const u32 NumBuffers = 2;
#else
        static const u32 NumBuffers = 8;
#endif

        LPVBuffer();
        ~LPVBuffer();


        void release();
        bool reset(u32 width, u32 height, u32 depth);

        lgraphics::Texture3DRef texSHCoeffRTemp_[NumBuffers];
        lgraphics::RenderTargetViewRef viewSHCoeffRTemp_[NumBuffers];
        lgraphics::Texture3DRef texSHCoeffGTemp_[NumBuffers];
        lgraphics::RenderTargetViewRef viewSHCoeffGTemp_[NumBuffers];
        lgraphics::Texture3DRef texSHCoeffBTemp_[NumBuffers];
        lgraphics::RenderTargetViewRef viewSHCoeffBTemp_[NumBuffers];

        lgraphics::Texture3DRef texSHCoeffR_;
        lgraphics::RenderTargetViewRef viewSHCoeffR_;
        lgraphics::Texture3DRef texSHCoeffG_;
        lgraphics::RenderTargetViewRef viewSHCoeffG_;
        lgraphics::Texture3DRef texSHCoeffB_;
        lgraphics::RenderTargetViewRef viewSHCoeffB_;


        lgraphics::Texture3DRef texSHCoeffOcclusionTemp_;
        lgraphics::RenderTargetViewRef viewSHCoeffOcclusionTemp_;

        lgraphics::Texture3DRef texSHCoeffOcclusion_;
        lgraphics::RenderTargetViewRef viewSHCoeffOcclusion_;

    private:
        bool create(lgraphics::Texture3DRef& tex, lgraphics::RenderTargetViewRef& view, u32 width, u32 height, u32 depth);
    };
}
#endif //INC_RENDER_LPVBUFFER_H__
