#ifndef INC_RENDER_CASCADEDSHADOWMAP_H__
#define INC_RENDER_CASCADEDSHADOWMAP_H__
/**
@file CascadedShadowMap.h
@author t-sakai
@date 2013/09/25 create
*/
#include "render/render.h"
#include <lgraphics/api/TextureRef.h>

namespace render
{
    class DepthVS;
    class DepthGS;
    class DepthPS;
    class DepthBlurVS;
    class DepthBlurGS;
    class DepthBlurPS;

    class CascadedShadowMap
    {
    public:
        static const s32 NumMinCascades = 1;
        static const s32 NumMaxCascades = 4;

        CascadedShadowMap();
        ~CascadedShadowMap();

        void release();
        bool reset(u32 size, s32 numCascades);

        void begin(lgraphics::GraphicsDeviceRef& device);
        void end(lgraphics::GraphicsDeviceRef& device, s32 numCascades);

        lgraphics::Texture2DRef& getTex(){ return texDepth_;}

        DepthVS* getVS(){ return depthVS_;}
    private:
        bool create(u32 size, s32 numCascades);

        u32 size_;
        lgraphics::Texture2DRef texDepth_;
        lgraphics::DepthStencilViewRef viewDepth_;

        DepthVS* depthVS_;
        DepthGS* depthGS_;
        DepthPS* depthPS_;
    };
}

#endif //INC_RENDER_CASCADEDSHADOWMAP_H__
