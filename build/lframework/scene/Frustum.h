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

        /**
        @brief ビュー空間の視錐台計算
        */
        void calc(const Camera& camera, f32 zfar);

        /**
        @brief ビュー空間の球が視錐台内にあるか
        */
        bool contains(const lmath::Vector3& position, f32 radius)
        {
            contains(position.x_, position.y_, position.z_, radius);
        }

        /**
        @brief ビュー空間の球が視錐台内にあるか
        */
        bool contains(f32 x, f32 y, f32 z, f32 radius);

        /**
        @brief 視錐台の８頂点取得
        */
        void getPoints(lmath::Vector4* points);

        /**
        @brief 視錐台の８頂点取得
        */
        void getPoints(lmath::Vector4* points, f32 znear);

        static f32 calcFitZFar(const lmath::Vector4& aabbMin, const lmath::Vector4& aabbMax, const Camera& camera, f32 minz, f32 maxz);

        lmath::Plane planes_[6]; ///左、右、上、下、近、遠
    };

}
#endif //INC_LSCENE_FRUSTUM_H__
