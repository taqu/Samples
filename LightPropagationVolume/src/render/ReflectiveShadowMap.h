#ifndef INC_RENDER_REFLECTIVESHADOWMAP_H__
#define INC_RENDER_REFLECTIVESHADOWMAP_H__
/**
@file ReflectiveShadowMap.h
@author t-sakai
@date 2012/05/29 create
*/
#include <lgraphics/api/TextureRef.h>

#include "render/render.h"

namespace lgraphics
{
    class GraphicsDeviceRef;
}

namespace render
{
    class ReflectiveShadowMapVS;
    class ReflectiveShadowMapPS;

    class ReflectiveShadowMap
    {
    public:
        ReflectiveShadowMap();
        ~ReflectiveShadowMap();

        /**
        @brief 初期化
        @param size ... サイズ
        */
        bool initialize(u32 size);

        void release();

        u32 getSize() const{ return size_;}
        f32 getInvSize() const{ return invSize_;}

        /// Colorテクスチャ
        lgraphics::Texture2DRef&  getTexColor(){ return texColor_;}

        /// Colorビュー
        lgraphics::RenderTargetViewRef& getViewColor(){ return viewColor_;}

        /// 法線テクスチャ
        lgraphics::Texture2DRef& getTexNormal(){ return texNormal_;}

        /// 法線ビュー
        lgraphics::RenderTargetViewRef& getViewNormal(){ return viewNormal_;}

        /// デプステクスチャ
        lgraphics::Texture2DRef& getTexDepth(){ return texDepth_;}

        /// デプスビュー
        lgraphics::DepthStencilViewRef& getViewDepth(){ return viewDepth_;}

        ReflectiveShadowMapVS* getVS(){ return vs_;}
        ReflectiveShadowMapPS* getPS(){ return ps_;}

        void begin(lgraphics::GraphicsDeviceRef& device);
    private:
        u32 size_;
        f32 invSize_;

        lgraphics::Texture2DRef texColor_;
        lgraphics::RenderTargetViewRef viewColor_;

        lgraphics::Texture2DRef texNormal_;
        lgraphics::RenderTargetViewRef viewNormal_;

        lgraphics::Texture2DRef texDepth_;
        lgraphics::DepthStencilViewRef viewDepth_;

        ReflectiveShadowMapVS* vs_;
        ReflectiveShadowMapPS* ps_;
    };
}
#endif //INC_RENDER_REFLECTIVESHADOWMAP_H__
