#ifndef INC_LFRAMEWORK_REFLECTIVESHADOWMAP_H__
#define INC_LFRAMEWORK_REFLECTIVESHADOWMAP_H__
/**
@file ReflectiveShadowMap.h
@author t-sakai
@date 2014/09/15 create
*/
#include <lmath/lmath.h>
#include "lscene.h"

namespace lscene
{
    class Scene;

    class ReflectiveShadowMap
    {
    public:
        static const u8 FitNearFar_None = 0;
        static const s8 FitNearFar_AABB = 1;
        static const s8 FitNearFar_SceneAABB = 2;

        ReflectiveShadowMap();
        ~ReflectiveShadowMap();

        void initialize(s32 resolution);
        void update(const Scene& scene);


        s32 getResolution() const
        {
            return resolution_;
        }

        void setSceneAABB(const lmath::Vector4& aabbMin, const lmath::Vector4& aabbMax)
        {
            sceneAABBMin_ = aabbMin;
            sceneAABBMax_ = aabbMax;
        }

        const lmath::Matrix44& getLightProjection() const
        {
            return lightProjection_;
        }

    private:
        s32 resolution_;

        lmath::Vector4 sceneAABBMin_;
        lmath::Vector4 sceneAABBMax_;

        void createSceneAABBPoints(lmath::Vector4 dst[8]);

        lmath::Matrix44 lightProjection_;
    };
}

#endif //INC_LFRAMEWORK_REFLECTIVESHADOWMAP_H__
