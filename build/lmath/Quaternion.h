#ifndef INC_LMATH_QUATERNION_H__
#define INC_LMATH_QUATERNION_H__
/**
@file Quaternion.h
@author t-sakai
@date 2009/09/21 create
*/
#include "lmathcore.h"
#include "Vector3.h"
#include "Vector4.h"

namespace lmath
{
    class Matrix34;

    class Quaternion
    {
    public:
        Quaternion()
        {}

        inline Quaternion(const Quaternion& rhs);
        inline Quaternion& operator=(const Quaternion& rhs);

        Quaternion(f32 w, f32 x, f32 y, f32 z)
            :w_(w),
            x_(x),
            y_(y),
            z_(z)
        {
        }

        ~Quaternion()
        {
        }

        inline void set(f32 w, f32 x, f32 y, f32 z);

        inline void set(const Vector4& v);

        inline f32 operator[](s32 index) const;
        inline f32& operator[](s32 index);

        /// 単位四元数
        inline void identity();

        void setRotateX(f32 radian);
        void setRotateY(f32 radian);
        void setRotateZ(f32 radian);

        void setRotateXYZ(f32 radx, f32 rady, f32 radz);
        void setRotateXZY(f32 radx, f32 rady, f32 radz);
        void setRotateYZX(f32 radx, f32 rady, f32 radz);
        void setRotateYXZ(f32 radx, f32 rady, f32 radz);
        void setRotateZXY(f32 radx, f32 rady, f32 radz);
        void setRotateZYX(f32 radx, f32 rady, f32 radz);

        inline void setRotateAxis(const lmath::Vector3& axis, f32 radian);
        inline void setRotateAxis(const lmath::Vector4& axis, f32 radian);
        void setRotateAxis(f32 x, f32 y, f32 z, f32 radian);

        void lookAt(const Vector3& eye, const Vector3& at);
        /**
        @param dir ... 正規化済み方向
        */
        void lookAt(const Vector3& dir);
        void lookAt(const Vector4& eye, const Vector4& at);
        /**
        @param dir ... 正規化済み方向
        */
        void lookAt(const Vector4& dir);

        inline void conjugate();
        inline void conjugate(const Quaternion& q);

        Quaternion& invert();

        f32 length() const;
        inline f32 lengthSqr() const;

        void normalize();
        void normalize(f32 squaredLength);

        f32 dot(const Quaternion& q) const;

        inline Quaternion operator-() const;

        Quaternion& operator+=(const Quaternion& q);
        Quaternion& operator-=(const Quaternion& q);

        Quaternion& operator*=(f32 f);
        //
        Quaternion& operator*=(const Quaternion& q);

        f32 getRotationAngle() const;

        void getRotationAxis(lmath::Vector3& axis) const;
        void getMatrix(lmath::Matrix34& mat) const;
        void getMatrix(lmath::Matrix44& mat) const;

        void getEulerAngles(f32& x, f32& y, f32& z);

        void exp(f32 exponent);

        void mul(const Quaternion& q0, const Quaternion& q1);

        void mul(f32 a, const Quaternion& q);
        void mul(const Quaternion& q, f32 a){ mul(a, q);}

        void mul(const Vector3& v, const Quaternion& q);
        void mul(const Quaternion& q, const Vector3& v);

        void mul(const Vector4& v, const Quaternion& q);
        void mul(const Quaternion& q, const Vector4& v);

        /**
        @brief 線形補間。q = (1-t)*q1 + t*q2
        @param q0 ... 
        @param q1 ... 
        @param t ... 補間比
        */
        Quaternion& lerp(const Quaternion& q0, const Quaternion& q1, f32 t);

        /**
        @brief 球面線形補間。q = (1-t)*q1 + t*q2
        @param q0 ... 
        @param q1 ... 
        @param t ... 補間比
        */
        Quaternion& slerp(const Quaternion& q0, const Quaternion& q1, f32 t);

        Quaternion& squad(const Quaternion& q0, const Quaternion& q1, const Quaternion& a, const Quaternion& b, f32 t);

        inline bool isNan() const;

        inline void swap(Quaternion& rhs);

#if defined(LMATH_USE_SSE)
        inline static lm128 load(const Quaternion& v)
        {
            return _mm_loadu_ps(&v.w_);
        }

        inline static void store(Quaternion& v, lm128& r)
        {
            _mm_storeu_ps(&v.w_, r);
        }
#endif

        f32 w_, x_, y_, z_;
    };

#if defined(LMATH_USE_SSE)
    //extern LIME_ALIGN16 u32 QuaternionConjugateMask_[4];
    extern LIME_ALIGN16 u32 QuaternionMulMask0_[4];
    extern LIME_ALIGN16 u32 QuaternionMulMask1_[4];
    extern LIME_ALIGN16 u32 QuaternionMulMask2_[4];
#endif

    inline Quaternion::Quaternion(const Quaternion& rhs)
    {
#if defined(LMATH_USE_SSE)
        lm128 t = load(rhs);
        store(*this, t);
#else
        set(rhs.w_, rhs.x_, rhs.y_, rhs.z_);
#endif
    }

    inline Quaternion& Quaternion::operator=(const Quaternion& rhs)
    {
#if defined(LMATH_USE_SSE)
        lm128 t = load(rhs);
        store(*this, t);
#else
        set(rhs.w_, rhs.x_, rhs.y_, rhs.z_);
#endif
        return *this;
    }


    inline void Quaternion::set(f32 w, f32 x, f32 y, f32 z)
    {
        w_ = w; x_ = x; y_ = y; z_ = z;
    }

    inline void Quaternion::set(const Vector4& v)
    {
        w_ = v.w_; x_ = v.x_; y_ = v.y_; z_ = v.z_;
    }

    inline Quaternion Quaternion::operator-() const
    {
#if defined(LMATH_USE_SSE)
        f32 f;
        *((u32*)&f) = 0x80000000U;
        lm128 mask = _mm_load1_ps(&f);
        lm128 r0 = load(*this);
        r0 = _mm_xor_ps(r0, mask);

        Quaternion ret;
        store(ret, r0);
        return ret;
#else
        return Quaternion(-w_, -x_, -y_, -z_);
#endif
    }

    inline f32 Quaternion::operator[](s32 index) const
    {
        LASSERT(0<=index && index < 4);
        return (&w_)[index];
    }

    inline f32& Quaternion::operator[](s32 index)
    {
        LASSERT(0<=index && index < 4);
        return (&w_)[index];
    }

    //---------------------------------------
    // 単位四元数
    inline void Quaternion::identity()
    {
#if defined(LMATH_USE_SSE)
        const f32 one = 1.0f;

        lm128 t = _mm_load_ss(&one);
        _mm_storeu_ps(&w_, t);

#else
        w_ = 1.0f;
        x_ = y_ = z_ = 0.0f;
#endif
    }

    inline void Quaternion::setRotateAxis(const lmath::Vector3& axis, f32 radian)
    {
        setRotateAxis(axis.x_, axis.y_, axis.z_, radian);
    }

    inline void Quaternion::setRotateAxis(const lmath::Vector4& axis, f32 radian)
    {
        setRotateAxis(axis.x_, axis.y_, axis.z_, radian);
    }

    inline f32 Quaternion::lengthSqr() const
    {
        return ( w_*w_ + x_*x_ + y_*y_ + z_*z_);
    }

    inline void Quaternion::conjugate()
    {
//#if defined(LMATH_USE_SSE)
#if 0
        lm128 mask = _mm_loadu_ps((f32*)QuaternionConjugateMask_);

        lm128 r0 = load(*this);
        r0 = _mm_xor_ps(r0, mask);

        store(*this, r0);
#else
        x_ = -x_;
        y_ = -y_;
        z_ = -z_;
#endif
    }

    inline void Quaternion::conjugate(const Quaternion& q)
    {
//#if defined(LMATH_USE_SSE)
#if 0
        lm128 mask = _mm_loadu_ps((f32*)QuaternionConjugateMask_);

        lm128 r0 = load(q);
        r0 = _mm_xor_ps(r0, mask);

        store(*this, r0);
#else
        w_ =  q.w_;
        x_ = -q.x_;
        y_ = -q.y_;
        z_ = -q.z_;
#endif
    }

    inline bool Quaternion::isNan() const
    {
        return (lcore::isNan(w_) || lcore::isNan(x_) || lcore::isNan(y_) || lcore::isNan(z_));
    }

    inline void Quaternion::swap(Quaternion& rhs)
    {
#if defined(LMATH_USE_SSE)
        lm128 t0 = load(*this);
        lm128 t1 = load(rhs);

        store(*this, t1);
        store(rhs, t0);
#else
        lcore::swap(w_, rhs.w_);
        lcore::swap(x_, rhs.x_);
        lcore::swap(y_, rhs.y_);
        lcore::swap(z_, rhs.z_);
#endif
    }
}

#endif //INC_LMATH_QUATERNION_H__
