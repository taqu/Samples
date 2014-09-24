#ifndef INC_VCT_VOXEL_H__
#define INC_VCT_VOXEL_H__
/**
@file Voxel.h
@author t-sakai
@date 2014/09/08 create
*/
#include "vct.h"
#include <lmath/lmath.h>
#include <lgraphics/api/TextureRef.h>
#include <lgraphics/api/ConstantBufferRef.h>
#include <lframework/scene/ReflectiveShadowMap.h>

namespace render
{
    class VoxelizeVS;
    class VoxelizeGS;
    class VoxelizePS;

    class VoxelRenderVS;
    class VoxelRenderGS;
    class VoxelRenderPS;

    class ReflectiveShadowMapVS;
    class ReflectiveShadowMapPS;
    class ReflectiveShadowMapTextureVS;
    class ReflectiveShadowMapTexturePS;

    class VoxelInjectionVS;
    class VoxelInjectionPS;

    class VoxelMipmapVS;
    class VoxelMipmapPS;
}

namespace lscene
{
    class Scene;
}

namespace vct
{
    class Voxel
    {
    public:
        static const s32 MaxMipLevels = 5;

        Voxel();
        ~Voxel();

        void create(s32 resolution, s32 mipmapLevels);
        void destroy();

        s32 getResolution() const{ return resolution_;}
        render::VoxelizeVS* getVoxelizeVS() { return voxelizeVS_;}
        render::VoxelizeGS* getVoxelizeGS() { return voxelizeGS_;}
        render::VoxelizePS* getVoxelizePS() { return voxelizePS_;}

        void setView(const lmath::Vector4& center, const lmath::Vector4& direction, const lmath::Vector4& size, s32 reflectiveResolution);
        void getViewProjection(lmath::Matrix44& viewProj);

        void begin(lgraphics::GraphicsDeviceRef& device);
        void end(lgraphics::GraphicsDeviceRef& device);


        render::ReflectiveShadowMapVS* getReflectiveShadowMapVS() { return reflectiveShadowMapVS_;}
        render::ReflectiveShadowMapPS* getReflectiveShadowMapPS() { return reflectiveShadowMapPS_;}
        render::ReflectiveShadowMapTextureVS* getReflectiveSHadowMapTextureVS() { return reflectiveSHadowMapTextureVS_;}
        render::ReflectiveShadowMapTexturePS* getReflectiveSHadowMapTexturePS() { return reflectiveSHadowMapTexturePS_;}
        const lmath::Matrix44& getLightProjection() const{ return reflectiveShadowMap_.getLightProjection();}

        void beginReflective(lgraphics::GraphicsDeviceRef& device, const lscene::Scene& scene);
        void endReflective(lgraphics::GraphicsDeviceRef& device);

        void setVoxelTextureToPS(lgraphics::GraphicsDeviceRef& device, s32 startIndex, s32 samplerIndex);
        void getVoxelParam(
            f32& voxelDiameter,
            f32& invVoxelDiameter,
            lmath::Matrix44& worldToVoxelSpace,
            lmath::Vector4& voxelScale);

        lgraphics::Texture2DRef& getReflectiveShadowMap() { return texReflectiveShadowMap_;}

        void draw(lgraphics::GraphicsDeviceRef& device, const lscene::Scene& scene);
    private:
        struct Constants0
        {
            s32 voxelSize_[4]; //voxel resolution, (voxel resolution)^2, (super sample)
            lmath::Vector4 voxelInvSize_; //1/(voxel resolution), 1/(voxel resolution)^2, 1/(super sample)
        };

        struct Constants1
        {
            lmath::Vector4 voxelCenter_;
            lmath::Vector4 voxelWorldSize_;
        };

        struct Constants2
        {
            lmath::Matrix44 mlwvp_;
        };

        struct Constants3
        {
            f32 depthBias_;
            s32 reserved0_;
            s32 reserved1_;
            s32 reserved2_;
        };

        struct Constants4
        {
            s32 resolution_;
            s32 resolution2_;
            f32 invResolution_;
            f32 invResolution2_;
        };

        s32 resolution_;
        s32 mipmapLevels_;
        s32 superSamples_;
        f32 voxelDiameter_;
        f32 invVoxelDiameter_;
        Constants1 voxelArea_;
        lmath::Vector4 direction_;
        lmath::Matrix44 projection_;
        lmath::Matrix44 view_;

        render::VoxelizeVS* voxelizeVS_;
        render::VoxelizeGS* voxelizeGS_;
        render::VoxelizePS* voxelizePS_;

        render::VoxelInjectionVS* voxelInjectionVS_;
        render::VoxelInjectionPS* voxelInjectionPS_;

        render::VoxelMipmapVS* voxelMipmapVS_;
        render::VoxelMipmapPS* voxelMipmapPS_;

        render::VoxelRenderVS* voxelRenderVS_;
        render::VoxelRenderGS* voxelRenderGS_;
        render::VoxelRenderPS* voxelRenderPS_;

        lgraphics::BufferRef voxelBuffers_[6];
        lgraphics::UnorderedAccessViewRef voxelBufferUAVViews_[6];

        lgraphics::Texture3DRef voxelTextures_[6];
        lgraphics::ShaderResourceViewRef voxelSRVViews_[6][MaxMipLevels];
        lgraphics::UnorderedAccessViewRef voxelUAVViews_[6][MaxMipLevels];

        lgraphics::ConstantBufferRef constants0_;
        lgraphics::ConstantBufferRef constants1_;
        lgraphics::ConstantBufferRef constants2_;
        lgraphics::ConstantBufferRef constants3_;
        lgraphics::ConstantBufferRef constants4_;

        render::ReflectiveShadowMapVS* reflectiveShadowMapVS_;
        render::ReflectiveShadowMapPS* reflectiveShadowMapPS_;
        render::ReflectiveShadowMapTextureVS* reflectiveSHadowMapTextureVS_;
        render::ReflectiveShadowMapTexturePS* reflectiveSHadowMapTexturePS_;

        lmath::Vector4 lightDirection_;
        lscene::ReflectiveShadowMap reflectiveShadowMap_;
        lgraphics::Texture2DRef texReflectiveShadowMap_;
        lgraphics::RenderTargetViewRef rtReflectiveShadowMap_;
        lgraphics::Texture2DRef texReflectiveDepth_;
        lgraphics::DepthStencilViewRef dsReflectiveDepth_;

    };
}
#endif //INC_VCT_VOXEL_H__
