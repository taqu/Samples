#ifndef INC_LMATH_VECTOR4_H__
#define INC_LMATH_VECTOR4_H__
/**
@file Vector4.h
@author t-sakai
@date 2009/01/17 create
*/
#include "lmathcore.h"
#include <lcore/utility.h>
#include "Vector3.h"

namespace lmath
{
    class Matrix34;
    class Matrix44;
    class Quaternion;

    //--------------------------------------------
    //---
    //--- Vector4
    //---
    //--------------------------------------------
    class Vector4
    {
    public:
        Vector4()
        {}

        inline Vector4(f32 x, f32 y, f32 z, f32 w);
        inline Vector4(const Vector4& rhs);
        inline Vector4& operator=(const Vector4& rhs);
        inline Vector4& operator=(const Vector3& rhs);

        inline explicit Vector4(const Vector3& v);
        inline Vector4(const Vector3& v, f32 w);

        inline void set(f32 x, f32 y, f32 z, f32 w);

        inline void set(const Vector3& v);
        inline void set(const Vector3& v, f32 w);

        inline void zero();
        inline void identity();
        inline void one();

        inline f32 operator[](s32 index) const;
        inline f32& operator[](s32 index);
        inline Vector4 operator-() const;

        inline Vector4& operator+=(const Vector4& v);
        inline Vector4& operator-=(const Vector4& v);

        inline Vector4& operator*=(f32 f);
        inline Vector4& operator/=(f32 f);

        inline Vector4& operator*=(const Vector4& v);
        inline Vector4& operator/=(const Vector4& v);

        bool isEqual(const Vector4& v) const;
        bool isEqual(const Vector4& v, f32 epsilon) const;

        inline bool operator==(const Vector4& v) const;
        inline bool operator!=(const Vector4& v) const;

        void setLength();
        f32 length() const;
        f32 lengthSqr() const;
        void normalize();
        void normalize(f32 lengthSqr);

        f32 dot(const Vector4& v) const;
        void cross3(const Vector4& v0, const Vector4& v1);
        f32 distance(const Vector4& v) const;
        f32 distanceSqr(const Vector4& v) const;

        f32 distance3(const Vector4& v) const;

        void setLerp(const Vector4& v1, const Vector4& v2, f32 f);

        void mul(f32 a, const Vector4& v);
        void mul(const Vector4& v, f32 a){ mul(a, v);}

        void mul(const Matrix34& m, const Vector4& v);
        void mul(const Vector4& v, const Matrix34& m);

        void mul(const Matrix44& m, const Vector4& v);
        void mul(const Vector4& v, const Matrix44& m);

        inline void mulPoint(const Matrix44& m, const Vector4& v);
        inline void mulVector(const Matrix44& m, const Vector4& v);

        void rotate(const Quaternion& rotation);
        void rotate(const Quaternion& rotation, const Vector4& v);

        inline void swap(Vector4& rhs);

        inline bool isNan() const;
        inline bool isZero() const;

        inline void add(const Vector4& v0, const Vector4& v1);
        inline void sub(const Vector4& v0, const Vector4& v1);

        inline void add(f32 v);
        inline void sub(f32 v);

        inline void min(const Vector4& v0, const Vector4& v1);
        inline void max(const Vector4& v0, const Vector4& v1);

        /**
        @brief v0*v1 + v2
        */
        inline void muladd(const Vector4& v0, const Vector4& v1, const Vector4& v2);

        /**
        @brief a*v1 + v2
        */
        inline void muladd(f32 a, const Vector4& v0, const Vector4& v1);

        inline void floor();
        inline void invert();

        inline void sqrt();

        inline void getParallelComponent(Vector4& dst, const Vector4& basis) const
        {
            const f32 cs = dot(basis);
            dst = basis;
            dst *= cs;
        }

        inline void getPerpendicularComponent(Vector4& dst, const Vector4& basis) const
        {
            Vector4 parallel;
            getParallelComponent(parallel, basis);
            dst = *this;
            dst -= parallel;
        }

#if defined(LMATH_USE_SSE)
        inline static lm128 load(const Vector4& v)
        {
            return _mm_loadu_ps(&v.x_);
        }

        inline static void store(Vector4& v, lm128& r)
        {
            _mm_storeu_ps(&v.x_, r);
        }

        //inline lm128 rcp(lm128& r)
        //{
        //    //Newton-Raphson法で、行列式の逆数を計算
        //    lm128 tmp = _mm_rcp_ss(r);
        //    r = _mm_mul_ss(r, tmp);
        //    r = _mm_mul_ss(r, tmp);
        //    tmp = _mm_add_ss(tmp, tmp);
        //    r = _mm_sub_ss(tmp, r);
        //    r = _mm_shuffle_ps(r, r, 0);
        //    return r;
        //}
#endif

        f32 x_, y_, z_, w_;
    };

    //--------------------------------------------
    //---
    //--- Vector4
    //---
    //--------------------------------------------
    inline Vector4::Vector4(f32 x, f32 y, f32 z, f32 w)
        :x_(x), y_(y), z_(z), w_(w)
    {}

    inline Vector4::Vector4(const Vector4& rhs)
    {
#if defined(LMATH_USE_SSE)
        lm128 t = load(rhs);
        store(*this, t);
#else
        set(rhs.x_, rhs.y_, rhs.z_, rhs.w_);
#endif
    }

    inline Vector4& Vector4::operator=(const Vector4& rhs)
    {
#if defined(LMATH_USE_SSE)
        lm128 t = load(rhs);
        store(*this, t);
#else
        set(rhs.x_, rhs.y_, rhs.z_, rhs.w_);
#endif
        return *this;
    }

    inline Vector4& Vector4::operator=(const Vector3& rhs)
    {
        set(rhs.x_, rhs.y_, rhs.z_, 0.0f);
        return *this;
    }


    inline Vector4::Vector4(const Vector3& v)
        :x_(v.x_)
        ,y_(v.y_)
        ,z_(v.z_)
        ,w_(0.0f)
    {
    }

    inline Vector4::Vector4(const Vector3& v, f32 w)
        :x_(v.x_)
        ,y_(v.y_)
        ,z_(v.z_)
        ,w_(w)
    {
    }

    inline void Vector4::set(f32 x, f32 y, f32 z, f32 w)
    {
        x_ = x; y_ = y; z_ = z; w_ = w;
    }

    inline void Vector4::set(const Vector3& v)
    {
        x_ = v.x_;
        y_ = v.y_;
        z_ = v.z_;
        w_ = 0.0f;
    }

    inline void Vector4::set(const Vector3& v, f32 w)
    {
        x_ = v.x_;
        y_ = v.y_;
        z_ = v.z_;
        w_ = w;
    }

    inline void Vector4::zero()
    {
#if defined(LMATH_USE_SSE)
        lm128 zero = _mm_setzero_ps();
        store(*this, zero);
#else
        x_ = y_ = z_ = w_ = 0.0f;
#endif
    }

    inline void Vector4::identity()
    {
#if defined(LMATH_USE_SSE)
        static const LIME_ALIGN16 f32 Init[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        lm128 r = _mm_load_ps(Init);
        store(*this, r);
#else
        x_ = y_ = z_ = 0.0f;
        w_ = 1.0f;
#endif
    }

    inline void Vector4::one()
    {
#if defined(LMATH_USE_SSE)
        static const LIME_ALIGN16 f32 Init[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        lm128 r = _mm_load_ps(Init);
        store(*this, r);
#else
        x_ = y_ = z_ = w_ = 1.0f;
#endif
    }


    inline f32 Vector4::operator[](s32 index) const
    {
        LASSERT(0<=index
            && index < 4);

        return (&x_)[index];
    }

    inline f32& Vector4::operator[](s32 index)
    {
        LASSERT(0<=index
            && index < 4);

        return (&x_)[index];
    }

    inline Vector4 Vector4::operator-() const
    {
#if defined(LMATH_USE_SSE)
        f32 f;
        *((u32*)&f) = 0x80000000U;
        lm128 mask = _mm_load1_ps(&f);
        lm128 r0 = load(*this);
        r0 = _mm_xor_ps(r0, mask);

        Vector4 ret;
        store(ret, r0);
        return ret;
#else
        return Vector4(-x_, -y_, -z_, -w_);
#endif
    }

    inline Vector4& Vector4::operator+=(const Vector4& v)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(*this);
        lm128 r1 = load(v);
        r0 = _mm_add_ps(r0, r1);
        store(*this, r0);

#else
        x_ += v.x_;
        y_ += v.y_;
        z_ += v.z_;
        w_ += v.w_;
#endif
        return *this;
    }

    inline Vector4& Vector4::operator-=(const Vector4& v)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(*this);
        lm128 r1 = load(v);
        r0 = _mm_sub_ps(r0, r1);
        store(*this, r0);

#else
        x_ -= v.x_;
        y_ -= v.y_;
        z_ -= v.z_;
        w_ -= v.w_;
#endif
        return *this;
    }

    inline Vector4& Vector4::operator*=(f32 f)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(*this);
        lm128 r1 = _mm_load1_ps(&f);
        r0 = _mm_mul_ps(r0, r1);
        store(*this, r0);

#else
        x_ *= f;
        y_ *= f;
        z_ *= f;
        w_ *= f;
#endif
        return *this;
    }

    inline Vector4& Vector4::operator/=(f32 f)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(*this);
        lm128 r1 = _mm_load1_ps(&f);
        r0 = _mm_div_ps(r0, r1);
        store(*this, r0);

#else
        LASSERT(!lmath::isEqual(f, 0.0f));
        f = 1.0f / f;
        x_ *= f;
        y_ *= f;
        z_ *= f;
        w_ *= f;
#endif
        return *this;
    }

    inline Vector4& Vector4::operator*=(const Vector4& v)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(*this);
        lm128 r1 = load(v);
        r0 = _mm_mul_ps(r0, r1);
        store(*this, r0);
#else
        x_ *= v.x_;
        y_ *= v.y_;
        z_ *= v.z_;
        w_ *= v.w_;
#endif
        return *this;
    }

    inline Vector4& Vector4::operator/=(const Vector4& v)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(*this);
        lm128 r1 = load(v);
        r0 = _mm_div_ps(r0, r1);
        store(*this, r0);
#else
        x_ /= v.x_;
        y_ /= v.y_;
        z_ /= v.z_;
        w_ /= v.w_;
#endif
        return *this;
    }

    inline bool Vector4::operator==(const Vector4& v) const
    {
        return isEqual(v);
    }

    inline bool Vector4::operator!=(const Vector4& v) const
    {
        return !isEqual(v);
    }

    inline void Vector4::mulPoint(const Matrix44& m, const Vector4& v)
    {
        *this = v;
        f32 w = w_;
        w_ = 1.0f;
        mul(m, *this);
        *this /= w_;
        w_ = w;
    }

    inline void Vector4::mulVector(const Matrix44& m, const Vector4& v)
    {
        *this = v;
        f32 w = w_;
        w_ = 0.0f;
        mul(m, *this);
        w_ = w;
    }

    inline void Vector4::swap(Vector4& rhs)
    {
#if defined(LMATH_USE_SSE)
        lm128 t0 = load(*this);
        lm128 t1 = load(rhs);

        store(*this, t1);
        store(rhs, t0);
#else
        lcore::swap(x_, rhs.x_);
        lcore::swap(y_, rhs.y_);
        lcore::swap(z_, rhs.z_);
        lcore::swap(w_, rhs.w_);
#endif
    }

    inline bool Vector4::isNan() const
    {
        return (lcore::isNan(x_) || lcore::isNan(y_) || lcore::isNan(z_) || lcore::isNan(w_));
    }

    inline bool Vector4::isZero() const
    {
        return (lmath::isZero(x_) && lmath::isZero(y_) && lmath::isZero(z_) && lmath::isZero(w_));
    }

    inline void Vector4::add(const Vector4& v0, const Vector4& v1)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(v0);
        lm128 r1 = load(v1);
        r0 = _mm_add_ps(r0, r1);
        store(*this, r0);

#else
        x_ = v0.x_ + v1.x_;
        y_ = v0.y_ + v1.y_;
        z_ = v0.z_ + v1.z_;
        w_ = v0.w_ + v1.w_;
#endif
    }

    inline void Vector4::sub(const Vector4& v0, const Vector4& v1)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(v0);
        lm128 r1 = load(v1);
        r0 = _mm_sub_ps(r0, r1);
        store(*this, r0);

#else
        x_ = v0.x_ - v1.x_;
        y_ = v0.y_ - v1.y_;
        z_ = v0.z_ - v1.z_;
        w_ = v0.w_ - v1.w_;
#endif
    }

    inline void Vector4::add(f32 v)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(*this);
        lm128 r1 = _mm_load1_ps(&v);
        r0 = _mm_add_ps(r0, r1);
        store(*this, r0);

#else
        x_ += v;
        y_ += v;
        z_ += v;
        w_ += v;
#endif
    }

    inline void Vector4::sub(f32 v)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(*this);
        lm128 r1 = _mm_load1_ps(&v);
        r0 = _mm_sub_ps(r0, r1);
        store(*this, r0);

#else
        x_ -= v;
        y_ -= v;
        z_ -= v;
        w_ -= v;
#endif
    }

    inline void Vector4::min(const Vector4& v0, const Vector4& v1)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(v0);
        lm128 r1 = load(v1);
        r0 = _mm_min_ps(r0, r1);
        store(*this, r0);

#else
        x_ = lcore::minimum(v0.x_, v1.x_);
        y_ = lcore::minimum(v0.y_, v1.y_);
        z_ = lcore::minimum(v0.z_, v1.z_);
        w_ = lcore::minimum(v0.w_, v1.w_);
#endif
    }

    inline void Vector4::max(const Vector4& v0, const Vector4& v1)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(v0);
        lm128 r1 = load(v1);
        r0 = _mm_max_ps(r0, r1);
        store(*this, r0);

#else
        x_ = lcore::maximum(v0.x_, v1.x_);
        y_ = lcore::maximum(v0.y_, v1.y_);
        z_ = lcore::maximum(v0.z_, v1.z_);
        w_ = lcore::maximum(v0.w_, v1.w_);
#endif
    }

    inline void Vector4::muladd(const Vector4& v0, const Vector4& v1, const Vector4& v2)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(v0);
        lm128 r1 = load(v1);
        lm128 r2 = load(v2);
        r0 = _mm_mul_ps(r0, r1);
        r0 = _mm_add_ps(r0, r2);
        store(*this, r0);

#else
        x_ = v0.x_ * v1.x_ + v2.x_;
        y_ = v0.y_ * v1.y_ + v2.y_;
        z_ = v0.z_ * v1.z_ + v2.z_;
        w_ = v0.w_ * v1.w_ + v2.w_;
#endif
    }

    inline void Vector4::muladd(f32 a, const Vector4& v0, const Vector4& v1)
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = _mm_load1_ps(&a);
        lm128 r1 = load(v0);
        lm128 r2 = load(v1);
        r0 = _mm_mul_ps(r0, r1);
        r0 = _mm_add_ps(r0, r2);
        store(*this, r0);

#else
        x_ = a * v0.x_ + v1.x_;
        y_ = a * v0.y_ + v1.y_;
        z_ = a * v0.z_ + v1.z_;
        w_ = a * v0.w_ + v1.w_;
#endif
    }

    inline void Vector4::floor()
    {
        x_ = lmath::floor(x_);
        y_ = lmath::floor(y_);
        z_ = lmath::floor(z_);
        w_ = lmath::floor(w_);
    }

    inline void Vector4::invert()
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = _mm_set1_ps(1.0f);
        lm128 r1 = load(*this);
        r0 = _mm_div_ps(r0, r1);
        store(*this, r0);
#else
        x_ = 1.0f/x_;
        y_ = 1.0f/y_;
        z_ = 1.0f/z_;
        w_ = 1.0f/w_;
#endif
    }

    inline void Vector4::sqrt()
    {
#if defined(LMATH_USE_SSE)
        lm128 r0 = load(*this);
        r0 = _mm_sqrt_ps(r0);
        store(*this, r0);

#else
        x_ = lmath::sqrt(x_);
        y_ = lmath::sqrt(y_);
        z_ = lmath::sqrt(z_);
        w_ = lmath::sqrt(w_);
#endif
    }
}

#endif //INC_LMATH_VECTOR4_H__
