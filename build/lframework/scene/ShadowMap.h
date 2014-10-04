#ifndef INC_LFRAMEWORK_SHADOWMAP_H__
#define INC_LFRAMEWORK_SHADOWMAP_H__
/**
@file ShadowMap.h
@author t-sakai
@date 2014/09/04 create
*/
#include <lmath/lmath.h>
#include "lscene.h"

namespace lscene
{
    class Scene;

    class ShadowMap
    {
    public:
        static const s32 MaxCascades = 4;
        static const u8 FitType_ToCascades = 0;
        static const u8 FitType_ToScene = 1;

        static const u8 FitNearFar_None = 0;
        static const u8 FitNearFar_AABB = 1;
        static const u8 FitNearFar_SceneAABB = 2;

        ShadowMap();
        ~ShadowMap();

        void initialize(s32 cascadeLevels, s32 resolution, f32 znear, f32 zfar, f32 logRatio=0.5f);
        void update(const Scene& scene);

        s32 getCascadeLevels() const
        {
            return cascadeLevels_;
        }

        s32 getResolution() const
        {
            return resolution_;
        }

        s32 getPCFBlurSize() const
        {
            return pcfBlurSize_;
        }

        void setPCFBlurSize(s32 size)
        {
            pcfBlurSize_ = size;
        }

        u8 getFitType() const
        {
            return fitType_;
        }

        void setFitType(u8 type)
        {
            fitType_ = type;
        }

        u8 getMoveLightTexelSize() const
        {
            return moveLightTexelSize_;
        }

        void setMoveLightTexelSize(u8 flag)
        {
            moveLightTexelSize_ = flag;
        }

        s8 getFitNearFar()
        {
            return fitNearFar_;
        }

        void setFitNearFar(u8 type)
        {
            fitNearFar_ = type;
        }

        void setSceneAABB(const lmath::Vector4& aabbMin, const lmath::Vector4& aabbMax)
        {
            sceneAABBMin_ = aabbMin;
            sceneAABBMax_ = aabbMax;
        }

        const lmath::Matrix44& getLightProjection(s32 cascade) const
        {
            LASSERT(0<=cascade && cascade<cascadeLevels_);
            return lightProjection_[cascade];
        }

        f32 getCascadePartition(s32 cascade) const
        {
            LASSERT(0<=cascade && cascade<cascadeLevels_);
            return cascadePartitionsFrustum_[cascade];
        }

        void calcCascadePartitions(f32 logRatio);
    private:
        s32 cascadeLevels_;
        s32 resolution_;
        f32 znear_;
        f32 zfar_;
        s32 pcfBlurSize_;
        u8 fitType_;
        u8 moveLightTexelSize_;
        u8 fitNearFar_;
        u8 reserved3_;

        lmath::Vector4 sceneAABBMin_;
        lmath::Vector4 sceneAABBMax_;

        void createSceneAABBPoints(lmath::Vector4 dst[8]);
        void getNearFar(f32& nearPlane, f32& farPlane, lmath::Vector4& lightViewOrthoMin, lmath::Vector4& lightViewOrthoMax, lmath::Vector4* viewPoints);

        f32 cascadePartitions_[MaxCascades+1];
        f32 cascadePartitionsFrustum_[MaxCascades+1];
        lmath::Matrix44 lightProjection_[MaxCascades];
    };
}

#endif //INC_LFRAMEWORK_SHADOWMAP_H__
