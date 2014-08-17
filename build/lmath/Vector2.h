#ifndef INC_LMATH_VECTOR2_H__
#define INC_LMATH_VECTOR2_H__
/**
@file Vector2.h
@author t-sakai
@date 2009/01/17 create
*/
#include "lmathcore.h"

namespace lmath
{
    //--------------------------------------------
    //---
    //--- Vector2
    //---
    //--------------------------------------------
    class Vector2
    {
    public:
        Vector2()
        {}

        Vector2(f32 x, f32 y)
            :x_(x), y_(y)
        {}

        Vector2(const Vector2& v)
            :x_(v.x_), y_(v.y_)
        {}

        void set(f32 x, f32 y)
        {
            x_ = x;
            y_ = y;
        }

        inline void zero();
        inline void one();

        inline f32 operator[](s32 index) const;
        inline f32& operator[](s32 index);
        inline Vector2 operator-() const;

        inline Vector2& operator+=(const Vector2& v);
        inline Vector2& operator-=(const Vector2& v);

        inline Vector2& operator*=(f32 f);
        inline Vector2& operator/=(f32 f);

        inline bool isEqual(const Vector2& v) const;
        inline bool isEqual(const Vector2& v, f32 epsilon) const;

        inline bool operator==(const Vector2& v) const;
        inline bool operator!=(const Vector2& v) const;

        inline f32 length() const;
        inline f32 lengthSqr() const;
        inline void normalize();

        inline f32 dot(const Vector2& v) const;

        inline f32 distance(const Vector2& v) const;
        inline f32 distanceSqr(const Vector2& v) const;


        Vector2& lerp(const Vector2& v0, const Vector2& v1, f32 t);

        inline bool isNan() const;

        inline void add(const Vector2& v0, const Vector2& v1);
        inline void sub(const Vector2& v0, const Vector2& v1);

        void muladd(f32 t, const Vector2& v0, const Vector2& v1);

        f32 x_, y_;
    };

    //--------------------------------------------
    //---
    //--- 実装
    //---
    //--------------------------------------------
    inline void Vector2::zero()
    {
        x_ = y_ = 0.0f;
    }

    inline void Vector2::one()
    {
        x_ = y_ = 1.0f;
    }

    inline f32 Vector2::operator[](s32 index) const
    {
        LASSERT(0<=index
            && index < 2);
        return (&x_)[index];
    }

    inline f32& Vector2::operator[](s32 index)
    {
        LASSERT(0<=index
            && index < 2);
        return (&x_)[index];
    }

    inline Vector2 Vector2::operator-() const
    {
        return Vector2(-x_, -y_);
    }

    inline Vector2& Vector2::operator+=(const Vector2& v)
    {
        x_ += v.x_;
        y_ += v.y_;
        return *this;
    }

    inline Vector2& Vector2::operator-=(const Vector2& v)
    {
        x_ -= v.x_;
        y_ -= v.y_;
        return *this;
    }

    inline Vector2& Vector2::operator*=(f32 f)
    {
        x_ *= f;
        y_ *= f;
        return *this;
    }

    inline Vector2& Vector2::operator/=(f32 f)
    {
        LASSERT(f != 0.0f);
        f = 1.0f/f;
        x_ *= f;
        y_ *= f;
        return *this;
    }

    inline bool Vector2::isEqual(const Vector2& v) const
    {
        return ( lmath::isEqual(x_, v.x_)
            && lmath::isEqual(y_, v.y_) );
    }

    inline bool Vector2::isEqual(const Vector2& v, f32 epsilon) const
    {
        return ( lmath::isEqual(x_, v.x_, epsilon)
            && lmath::isEqual(y_, v.y_, epsilon) );
    }

    inline bool Vector2::operator==(const Vector2& v) const
    {
        return isEqual(v);
    }

    inline bool Vector2::operator!=(const Vector2& v) const
    {
        return !isEqual(v);
    }

    inline f32 Vector2::length() const
    {
        return lmath::sqrt( lengthSqr() );
    }

    inline f32 Vector2::lengthSqr() const
    {
        return ( x_ * x_ + y_ * y_ );
    }

    inline void Vector2::normalize()
    {
        f32 l = lengthSqr();
        LASSERT( !(lmath::isEqual(l, 0.0f)) );
        l = lmath::sqrt(l);
        *this /= l;
    }

    inline f32 Vector2::dot(const Vector2& v) const
    {
        return ( x_ * v.x_ + y_ * v.y_ );
    }

    inline f32 Vector2::distance(const Vector2& v) const
    {
        return lmath::sqrt(distanceSqr(v));
    }

    inline f32 Vector2::distanceSqr(const Vector2& v) const
    {
        f32 dx = v.x_ - x_;
        f32 dy = v.y_ - y_;
        return (dx*dx + dy*dy); 
    }

    inline bool Vector2::isNan() const
    {
        return (lcore::isNan(x_) || lcore::isNan(y_));
    }

    inline void Vector2::add(const Vector2& v0, const Vector2& v1)
    {
        x_ = v0.x_ + v1.x_;
        y_ = v0.y_ + v1.y_;
    }

    inline void Vector2::sub(const Vector2& v0, const Vector2& v1)
    {
        x_ = v0.x_ - v1.x_;
        y_ = v0.y_ - v1.y_;
    }
}

#endif //INC_LMATH_VECTOR2_H__
