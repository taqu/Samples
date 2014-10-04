#ifndef INC_LSCENE_FRUSTUM_H__
#define INC_LSCENE_FRUSTUM_H__
/**
@file Frustum.h
@author t-sakai
@date 2011/11/03 create

*/
#include "lscene.h"
#include <lmath/geometry/Plane.h>

namespace lmath
{
    class Vector3;
    class Matrix44;
    class Sphere;
}

namespace lscene
{
    class Camera;

    class Frustum
    {
    public:
        Frustum()
        {}

        ~Frustum()
        {}

        void calcInView(const Camera& camera, f32 znear, f32 zfar);
        void calcInWorld(const Camera& camera);

        /**
        @brief
        */
        void calcFromProjection(const lmath::Matrix44& proj, f32 znear, f32 zfar);

        /**
        @brief 球が視錐台内にあるか
        */
        bool contains(const lmath::Vector3& position, f32 radius) const
        {
            contains(position.x_, position.y_, position.z_, radius);
        }

        /**
        @brief 球が視錐台内にあるか
        */
        bool contains(const lmath::Vector4& sphere) const
        {
            return contains(sphere.x_, sphere.y_, sphere.z_, sphere.w_);
        }

        /**
        @brief 球が視錐台内にあるか
        */
        bool contains(f32 x, f32 y, f32 z, f32 radius) const;

        /**
        @brief AABBが視錐台内にあるか
        */
        bool contains(const lmath::Vector3& bmin, const lmath::Vector3& bmax) const;

        /**
        @brief AABBが視錐台内にあるか
        */
        bool contains(const lmath::Vector4& bmin, const lmath::Vector4& bmax) const;

        /**
        @brief 視錐台の８頂点取得
        */
        void getPoints(lmath::Vector4* points) const;

        /**
        @brief 視錐台の８頂点取得
        */
        void getPoints(lmath::Vector4* points, f32 znear, f32 zfar) const;

        static void calcFitNearFar(f32& znear, f32& zfar, const lmath::Vector4& aabbMin, const lmath::Vector4& aabbMax, const Camera& camera);

        inline lmath::Plane& operator[](s32 index){ return planes_[index];}
        inline const lmath::Plane& operator[](s32 index) const{ return planes_[index];}

        lmath::Plane planes_[6]; ///左、右、上、下、近、遠
    };

}
#endif //INC_LSCENE_FRUSTUM_H__
