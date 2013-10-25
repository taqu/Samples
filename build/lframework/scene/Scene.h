#ifndef INC_LFRAMEWORK_SCENE_H__
#define INC_LFRAMEWORK_SCENE_H__
/**
@file Scene.h
@author t-sakai
@date 2009/10/06 create
*/

#include <lmath/lmath.h>

#include "LightEnvironment.h"
#include "Camera.h"

namespace lscene
{
    class Scene
    {
    public:
        static const s32 NumMinCascades = 1;
        static const s32 NumMaxCascades = 4;

        Scene();
        ~Scene();

        const lmath::Matrix44& getViewMatrix() const
        {
            return camera_.getViewMatrix();
        }

        void setViewMatrix(const lmath::Matrix44& view)
        {
            camera_.setViewMatrix(view);
        }

        const lmath::Matrix44& getProjMatrix() const
        {
            return camera_.getProjMatrix();
        }

        void setProjMatrix(const lmath::Matrix44& proj)
        {
            camera_.setProjMatrix(proj);
        }

        const lmath::Matrix44& getViewProjMatrix() const
        {
            return camera_.getViewProjMatrix();
        }

        const Camera& getCamera() const
        {
            return camera_;
        }

        Camera& getCamera()
        {
            return camera_;
        }

        LightEnvironment& getLightEnv()
        {
            return lightEnv_;
        }

        const LightEnvironment& getLightEnv() const
        {
            return lightEnv_;
        }

        const lmath::Matrix44& getLightViewProjection(s32 cascade) const
        {
            LASSERT(0<=cascade && cascade<numCascades_);

            return lightViewProjection_[cascade];
        }


        //void setSceneAABB(const lmath::Vector4& aabbMin, const lmath::Vector4& aabbMax)
        //{
        //    sceneAABBMin_ = aabbMin;
        //    sceneAABBMax_ = aabbMax;
        //}

        //const lmath::Vector4& getAABBMin() const
        //{
        //    return sceneAABBMin_;
        //}

        //const lmath::Vector4& getAABBMax() const
        //{
        //    return sceneAABBMax_;
        //}

        u32 getShadowMapSize() const
        {
            return shadowMapSize_;
        }

        void setShadowMapSize(u32 size)
        {
            LASSERT(size>2);
            shadowMapSize_ = size;
        }


        f32 getShadowMapZNear() const
        {
            return shadowMapZNear_;
        }

        void setShadowMapZNear(f32 znear)
        {
            shadowMapZNear_ = znear;
        }

        f32 getShadowMapZFar() const
        {
            return shadowMapZFar_;
        }

        void setShadowMapZFar(f32 zfar)
        {
            shadowMapZFar_ = zfar;
        }

        s32 getNumCascades() const
        {
            return numCascades_;
        }

        void setNumCascades(s32 numCascades)
        {
            LASSERT(NumMinCascades<=numCascades && numCascades<=NumMaxCascades);
            numCascades_ = numCascades;
        }

        void calcCascadeSplitDepth();
        void calcLightViewProjection();
        void getShadowMapProjection(lmath::Matrix44& mat, s32 cascade) const;

        void getCascadeSplitDepth(f32* dst);
    private:
        void calcLightViewProjection(s32 cascade, f32 shadowMapZNear, f32 shadowMapZFar);

        Camera camera_;
        LightEnvironment lightEnv_;

        lmath::Matrix44 lightViewProjection_[NumMaxCascades];

        //lmath::Vector4 sceneAABBMin_;
        //lmath::Vector4 sceneAABBMax_;

        u32 shadowMapSize_;
        s32 numCascades_;
        f32 shadowMapZNear_;
        f32 shadowMapZFar_;
        f32 cascadeSplitDepth_[NumMaxCascades];
    };
}

#endif //INC_LFRAMEWORK_SCENE_H__
