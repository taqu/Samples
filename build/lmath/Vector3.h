#ifndef INC_LMATH_VECTOR3_H__
#define INC_LMATH_VECTOR3_H__
/**
@file Vector3.h
@author t-sakai
@date 2009/01/17 create
*/
#include "lmathcore.h"
#include <lcore/utility.h>

namespace lmath
{
    class Vector4;
    class Matrix34;
    class Matrix44;
    class Quaternion;

    //--------------------------------------------
    //---
    //--- Vector3
    //---
    //--------------------------------------------
    class Vector3
    {
    public:
        Vector3()
        {}

        Vector3(f32 x, f32 y, f32 z)
            :x_(x)
            ,y_(y)
            ,z_(z)
        {}

        explicit Vector3(const lmath::Vector4& v);

        inline void set(f32 x, f32 y, f32 z)
        {
            x_ = x; y_ = y; z_ = z;
        }

        void set(const lmath::Vector4& v);

        inline void zero();
        inline void one();

        inline f32 operator[](s32 index) const;
        inline f32& operator[](s32 index);
        inline Vector3 operator-() const;


        inline Vector3& operator+=(const Vector3& v);
        inline Vector3& operator-=(const Vector3& v);

        inline Vector3& operator*=(f32 f);
        Vector3& operator/=(f32 f);

        Vector3& operator*=(const Vector3& v);
        Vector3& operator/=(const Vector3& v);

        inline bool isEqual(const Vector3& v) const;
        inline bool isEqual(const Vector3& v, f32 epsilon) const;

        inline bool operator==(const Vector3& v) const;
        inline bool operator!=(const Vector3& v) const;

        inline f32 length() const;
        inline f32 lengthSqr() const;
        void normalize();
        inline f32 dot(const Vector3& v) const;
        f32 distanceSqr(const Vector3& v) const;
        inline f32 distance(const Vector3& v) const;

        void cross(const Vector3& v1, const Vector3& v2);

        /**
        @brief 線形補間。v = (1-t)*v0 + t*v1
        @param v0 ... 
        @param v1 ... 
        */
        Vector3& lerp(const Vector3& v0, const Vector3& v1, f32 f);

        /**
        @brief 線形補間。v = f1*v0 + f0*v1
        @param v0 ... 
        @param v1 ... 
        */
        Vector3& lerp(const Vector3& v0, const Vector3& v1, f32 f0, f32 f1);

        void mul(const Matrix34& m, const Vector3& v);
        void mul(const Vector3& v, const Matrix34& m);

        void mul33(const Matrix34& m, const Vector3& v);
        void mul33(const Vector3& v, const Matrix34& m);

        void mul33(const Matrix44& m, const Vector3& v);
        void mul33(const Vector3& v, const Matrix44& m);

        void rotate(const Quaternion& rotation);
        void rotate(const Quaternion& rotation, const Vector3& v);

        inline void swap(Vector3& rhs);

        inline bool isNan() const;

        inline void add(const Vector3& v0, const Vector3& v1);
        inline void sub(const Vector3& v0, const Vector3& v1);

        inline void min(const Vector3& v0, const Vector3& v1);
        inline void max(const Vector3& v0, const Vector3& v1);

        /**
        @brief v0*v1 + v2
        */
        inline void muladd(const Vector3& v0, const Vector3& v1, const Vector3& v2);

        /**
        @brief a*v1 + v2
        */
        inline void muladd(f32 a, const Vector3& v0, const Vector3& v1);

#if defined(LMATH_USE_SSE)
        //SSEセット・ストア命令

        inline static lm128 load(const Vector3& v)
        {
            lm128 t = _mm_load_ss(&v.z_);
            t = _mm_movelh_ps(t, t);
            t = _mm_loadl_pi(t, reinterpret_cast<const __m64*>(&v.x_));
            return t;
        }

        inline static lm128 load(f32 x, f32 y, f32 z)
        {
            return set_m128(x, y, z, 1.0f);
        }

        inline static lm128 load(f32 v)
        {
            return _mm_load1_ps(&v);
        }

        inline static void store(Vector3& v, const lm128& r)
        {
            _mm_storel_pi(reinterpret_cast<__m64*>(&v.x_), r);

            static const u32 Shuffle = 170;
            lm128 t = _mm_shuffle_ps(r, r, Shuffle);
            _mm_store_ss(&v.z_, t);
        }
#endif

        f32 x_;
        f32 y_;
        f32 z_;
    };

    //--------------------------------------------
    //---
    //--- 実装
    //---
    //--------------------------------------------
    inline void Vector3::zero()
    {
        x_ = y_ = z_ = 0.0f;
    }

    inline void Vector3::one()
    {
        x_ = y_ = z_ = 1.0f;
    }

    inline f32 Vector3::operator[](s32 index) const
    {
        LASSERT(0<=index
            && index < 3);
        return (&x_)[index];
    }

    inline f32& Vector3::operator[](s32 index)
    {
        LASSERT(0<=index
            && index < 3);
        return (&x_)[index];
    }

    inline Vector3 Vector3::operator-() const
    {
        return Vector3(-x_, -y_, -z_);
    }

    //inline Vector3 Vector3::operator*(f32 f) const
    //{
    //    return Vector3(f*x_, f*y_, f*z_);
    //}

    //inline Vector3 Vector3::operator/(f32 f) const
    //{
    //    f32 inv = 1.0f/f;
    //    return Vector3(inv*x_, inv*y_, inv*z_);
    //}

    inline f32 Vector3::dot(const Vector3& v) const
    {
        return ( x_ * v.x_ + y_ * v.y_ + z_ * v.z_);
    }

    inline Vector3& Vector3::operator+=(const Vector3& v)
    {
        x_ += v.x_;
        y_ += v.y_;
        z_ += v.z_;
        return *this;
    }

    inline Vector3& Vector3::operator-=(const Vector3& v)
    {
        x_ -= v.x_;
        y_ -= v.y_;
        z_ -= v.z_;
        return *this;
    }

    inline Vector3& Vector3::operator*=(f32 f)
    {
        x_ *= f;
        y_ *= f;
        z_ *= f;
        return *this;
    }

    inline bool Vector3::isEqual(const Vector3& v) const
    {
        return ( lmath::isEqual(x_, v.x_)
            && lmath::isEqual(y_, v.y_)
            && lmath::isEqual(z_, v.z_) );
    }

    inline bool Vector3::isEqual(const Vector3& v, f32 epsilon) const
    {
        return ( lmath::isEqual(x_, v.x_, epsilon)
            && lmath::isEqual(y_, v.y_, epsilon)
            && lmath::isEqual(z_, v.z_, epsilon) );
    }

    inline bool Vector3::operator==(const Vector3& v) const
    {
        return isEqual(v);
    }

    inline bool Vector3::operator!=(const Vector3& v) const
    {
        return !isEqual(v);
    }

    inline f32 Vector3::length() const
    {
        return lmath::sqrt( lengthSqr() );
    }

    inline f32 Vector3::lengthSqr() const
    {
        return ( x_ * x_ + y_ * y_ + z_ * z_);
    }

    inline f32 Vector3::distance(const Vector3& v) const
    {
        return lmath::sqrt( distanceSqr(v) );
    }

    inline void Vector3::swap(Vector3& rhs)
    {
        lcore::swap(x_, rhs.x_);
        lcore::swap(y_, rhs.y_);
        lcore::swap(z_, rhs.z_);
    }

    inline bool Vector3::isNan() const
    {
        return (lcore::isNan(x_) || lcore::isNan(y_) || lcore::isNan(z_));
    }

    inline void Vector3::add(const Vector3& v0, const Vector3& v1)
    {
        x_ = v0.x_ + v1.x_;
        y_ = v0.y_ + v1.y_;
        z_ = v0.z_ + v1.z_;
    }

    inline void Vector3::sub(const Vector3& v0, const Vector3& v1)
    {
        x_ = v0.x_ - v1.x_;
        y_ = v0.y_ - v1.y_;
        z_ = v0.z_ - v1.z_;
    }


    inline void Vector3::min(const Vector3& v0, const Vector3& v1)
    {
        x_ = lcore::minimum(v0.x_, v1.x_);
        y_ = lcore::minimum(v0.y_, v1.y_);
        z_ = lcore::minimum(v0.z_, v1.z_);
    }

    inline void Vector3::max(const Vector3& v0, const Vector3& v1)
    {
        x_ = lcore::maximum(v0.x_, v1.x_);
        y_ = lcore::maximum(v0.y_, v1.y_);
        z_ = lcore::maximum(v0.z_, v1.z_);
    }

    // v0*v1 + v2
    inline void Vector3::muladd(const Vector3& v0, const Vector3& v1, const Vector3& v2)
    {
        x_ = v0.x_ * v1.x_ + v2.x_;
        y_ = v0.y_ * v1.y_ + v2.y_;
        z_ = v0.z_ * v1.z_ + v2.z_;
    }

    // a*v1 + v2
    inline void Vector3::muladd(f32 a, const Vector3& v0, const Vector3& v1)
    {
        x_ = a * v0.x_ + v1.x_;
        y_ = a * v0.y_ + v1.y_;
        z_ = a * v0.z_ + v1.z_;
    }
}

#endif //INC_LMATH_VECTOR3_H__
