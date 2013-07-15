#ifndef INC_LMATH_SPHERE_H__
#define INC_LMATH_SPHERE_H__
/**
@file Sphere.h
@author t-sakai
@date 2009/12/20 create
*/
#include "../lmathcore.h"
#include "../Vector3.h"
#include "../Vector4.h"

namespace lmath
{
    /// 球
    class Sphere
    {
    public:
        Sphere()
        {
        }

        Sphere(const Vector3& position, f32 radius)
            :s_(position, radius)
        {
        }

        Sphere(f32 x, f32 y, f32 z, f32 radius)
            :s_(x, y, z, radius)
        {
        }

        void set(f32 x, f32 y, f32 z, f32 radius)
        {
            s_.set(x, y, z, radius);
        }

        void zero()
        {
            s_.zero();
        }

        void getPosition(lmath::Vector3& position) const
        {
            position.set(s_.x_, s_.y_, s_.z_);
        }

        f32 getRadius() const
        {
            return s_.w_;
        }

        void setPosition(f32 x, f32 y, f32 z)
        {
            s_.x_ = x;
            s_.y_ = y;
            s_.z_ = z;
        }

        /**
        @brief 点に外接する球を計算
        */
        static Sphere circumscribed(const Vector3& p0, const Vector3& p1);

        /**
        @brief 点に外接する球を計算
        */
        static Sphere circumscribed(const Vector3& p0, const Vector3& p1, const Vector3& p2);

        /**
        @brief 点に外接する球を計算
        */
        static Sphere circumscribed(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3);


        static Sphere calcMiniSphere(const Vector3* points, u32 numPoints);

        void combine(const Sphere& s0, const Sphere& s1);
        void add(const Sphere& s1);

        f32 signedDistanceSqr(const Vector3& p) const;

        f32 distance(const Sphere& rhs) const;

        void swap(Sphere& rhs)
        {
            s_.swap(rhs.s_);
        }

        Vector4 s_; ///中心 半径
    };
}
#endif //INC_LMATH_SPHERE_H__
