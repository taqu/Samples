#ifndef INC_LMATH_PLANE_H__
#define INC_LMATH_PLANE_H__
/**
@file Plane.h
@author t-sakai
@date 2009/12/20 create
*/
#include "../lmathcore.h"
#include "../Vector3.h"
#include "../Vector4.h"

namespace lmath
{
    /// 平面
    class Plane
    {
    public:
        Plane()
        {
        }

        Plane(f32 a, f32 b, f32 c, f32 d)
            :v_(a, b, c, d)
        {
        }


        Plane(const Vector3& point,
            const Vector3& normal)
        {
            f32 d = -normal.dot(point);
            v_.set(normal.x_, normal.y_, normal.z_, d);
        }

        f32 dot(const lmath::Vector3& p) const
        {
            lmath::Vector4 t(p.x_, p.y_, p.z_, 1.0f);

            return v_.dot(t);
        }

        f32 dot(const lmath::Vector4& p) const
        {
            return v_.dot(p);
        }

        void getNormal(lmath::Vector3& normal) const
        {
            normal.set(v_.x_, v_.y_, v_.z_);
        }

        f32 getD() const
        {
            return v_.w_;
        }

        void normalize();


        Vector4 v_; /// 平面の法線, 原点から平面までの距離
    };
}

#endif //INC_LMATH_PLANE_H__
