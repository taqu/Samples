#ifndef INC_RENDER_SHADOWMAP_H__
#define INC_RENDER_SHADOWMAP_H__
/**
@file ShadowMap.h
@author t-sakai
@date 2012/08/27 create
*/
#include <lgraphics/api/TextureRef.h>
#include "render.h"

namespace render
{
    class DepthVS;
    class DepthPS;

    class ShadowMap
    {
    public:
        ShadowMap();
        ~ShadowMap();

        void release();
        bool reset(u32 size);

        void begin(lgraphics::GraphicsDeviceRef& device);
        void end(lgraphics::GraphicsDeviceRef& device);

        lgraphics::Texture2DRef& getTex(){ return texDepth_;}

        DepthVS* getVS(){ return depthVS_;}

    private:
        bool create(lgraphics::Texture2DRef& tex, lgraphics::DepthStencilViewRef& view, u32 size);

        u32 size_;

        lgraphics::Texture2DRef texDepth_;
        lgraphics::DepthStencilViewRef viewDepth_;

        DepthVS* depthVS_;
        DepthPS* depthPS_;
    };
}
#endif //INC_RENDER_SHADOWMAP_H__
