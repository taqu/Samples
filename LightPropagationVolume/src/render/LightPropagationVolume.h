#ifndef INC_RENDER_LIGHTPROPAGATIONVOLUME_H__
#define INC_RENDER_LIGHTPROPAGATIONVOLUME_H__
/**
@file LightPropagationVolume.h
@author t-sakai
@date 2012/05/29 create

*/
#include <lgraphics/api/VertexBufferRef.h>
#include <lgraphics/api/TextureRef.h>
#include <lgraphics/api/BlendStateRef.h>

#include "render/render.h"
#include "ReflectiveShadowMap.h"
#include "LPVBuffer.h"
#include "render/Shader.h"

namespace lgraphics
{
    class GraphicsDeviceRef;
}

namespace lscene
{
    class Scene;
}

namespace render
{
    class LightPropagationVolume
    {
    public:
        static const u32 NumIterations = 8;

        LightPropagationVolume();
        ~LightPropagationVolume();


        bool initialize(
            u32 reflectiveMapSize,
            u32 volumeResolution,
            f32 zfar,
            f32 attenuation);

        void release();

        ReflectiveShadowMap& getReflectiveShadowMap(){ return reflectiveShadowMap_;}
        const lmath::Matrix44& getLightViewProjection() const{ return lightViewProjection_;}

        const lmath::Vector4& getLPVMin() const{ return LPVMin_;}
        f32 getInvLPVSize() const{ return invLPVSize_;}
        f32 getInvVolumeResolution() const{ return invVolumeResolution_;}
        f32 getLPVAttenuation() const{ return LPVAttenuation_;}
        void setLPVAttenuation(f32 v)
        {
            LPVAttenuation_ = v * 4.0f * invLPVCellSize_ * invLPVCellSize_;
        }

        void beginReflectiveShadowMap(
            lgraphics::GraphicsDeviceRef& device,
            const lscene::Scene& scene);

        void beginLPVInjection(
            lgraphics::GraphicsDeviceRef& device,
            const lscene::Scene& scene);

        void beginLPVPropagate(
            lgraphics::GraphicsDeviceRef& device);

        void end(lgraphics::GraphicsDeviceRef& device);

        // LPVテクスチャ
        lgraphics::Texture3DRef& getTexLPVR(){ return LPVBuffer_.texSHCoeffR_;}
        lgraphics::Texture3DRef& getTexLPVG(){ return LPVBuffer_.texSHCoeffG_;}
        lgraphics::Texture3DRef& getTexLPVB(){ return LPVBuffer_.texSHCoeffB_;}

        LPVRenderVS* getRenderVS(){ return renderVS_;}
        LPVRenderPS* getRenderPS(){ return renderPS_;}

        void toggleOcclusion(){ enableOcclusion_ = !enableOcclusion_;}
    private:

        void calcLightViewProjectionForVolume(const lscene::Scene& scene);

        ID3D11RenderTargetView* oldColor_;
        ID3D11DepthStencilView* oldDepthStencil_;

        ReflectiveShadowMap reflectiveShadowMap_;

        u32 volumeResolution_;
        f32 invVolumeResolution_;

        f32 zfar_;

        f32 LPVCellSize_;
        f32 invLPVCellSize_;
        f32 invLPVSize_;
        f32 LPVAttenuation_;

        u32 numVertices_;

        lmath::Matrix44 lightViewProjection_;
        lmath::Vector4 LPVMin_; //４番目がセルサイズの逆数
        lmath::Vector4 viewZ_;

        LPVBuffer LPVBuffer_;
        lgraphics::BlendStateRef propagateBlendState_;

        LPVInjectionVS* injectionVS_;
        LPVInjectionGS* injectionGS_;
        LPVInjectionPS* injectionPS_;

        LPVOcclusionInjectionVS* occlusionInjectionVS_;
        LPVOcclusionInjectionPS* occlusionInjectionPS_;

        LPVPropagateVS* propagateVS_;
        LPVPropagateGS* propagateGS_;
        LPVPropagatePS* propagatePS_;
        LPVOcclusionPropagatePS* occlusionPropagatePS_;
        LPVAccumulatePS* accumulatePS_;

        LPVRenderVS* renderVS_;
        LPVRenderPS* renderPS_;

        bool enableOcclusion_;
    };
}
#endif //INC_RENDER_LIGHTPROPAGATIONVOLUME_H__
