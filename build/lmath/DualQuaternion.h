#ifndef INC_LMATH_DUALQUATERNION_H__
#define INC_LMATH_DUALQUATERNION_H__
/**
@file DualQuaternion.h
@author t-sakai
@date 2009/11/16 create
*/
#include "lmathcore.h"
#include "lmathutil.h"
#include "Vector3.h"
#include "Quaternion.h"

namespace lmath
{
    class DualQuaternion
    {
    public:
        DualQuaternion()
        {
        }

        ~DualQuaternion()
        {
        }

        inline void identity();
        inline const f32* get() const;

        inline const lmath::Quaternion& getRotation() const;
        inline const lmath::Quaternion& getTranslation() const;

        /**
        @brief 回転してから、移動の順でセット
        @param rotation
        @param translation
        */
        inline void set(const lmath::Quaternion& rotation, const lmath::Vector3& translation);

        /**
        @brief 移動してから、回転の順でセット
        @param translation
        @param rotation
        */
        inline void set(const lmath::Vector3& translation, const lmath::Quaternion& rotation);

        inline void set(const lmath::Quaternion& rotation, const lmath::Quaternion& translation);

        DualQuaternion& mul(const DualQuaternion& d0, const DualQuaternion& d1);

        inline DualQuaternion& operator+=(const DualQuaternion& q);
        inline DualQuaternion& operator-=(const DualQuaternion& q);

        inline DualQuaternion& operator*=(f32 f);

        inline void normalizeRot();

        /// 後から平行移動
        inline void translate(f32 x, f32 y, f32 z);

        /// 後から平行移動
        inline void translate(const lmath::Vector3& v);

        /// 前から平行移動
        inline void preTranslate(f32 x, f32 y, f32 z);

        /// 前から平行移動
        inline void preTranslate(const lmath::Vector3& v);

        /// 後から回転
        inline void rotate(const lmath::Quaternion& r);

        /// 前から回転
        inline void preRotate(const lmath::Quaternion& r);

        void invert();

        void getMatrix(lmath::Matrix34& mat) const;

        void transform(lmath::Vector3& v) const;

        /**
        @brief 線形補間。q = (1-t)*q1 + t*q2
        @param q0 ... 
        @param q1 ... 
        @param t ... 補間比
        */
        DualQuaternion& lerp(const DualQuaternion& q0, const DualQuaternion& q1, f32 t);

    private:

                lmath::Quaternion dq_[2];
    };

    inline void DualQuaternion::identity()
    {
        dq_[0].identity();
        dq_[1].set(0.0f, 0.0f, 0.0f, 0.0f);
    }

    inline const lmath::Quaternion& DualQuaternion::getRotation() const
    {
        return dq_[0];
    }

    inline const lmath::Quaternion& DualQuaternion::getTranslation() const
    {
        return dq_[1];
    }

    inline void DualQuaternion::set(const lmath::Quaternion& rotation, const lmath::Vector3& translation)
    {
        lmath::QuatTrans2UnitDualQuat(rotation, translation, dq_);
    }

    inline void DualQuaternion::set(const lmath::Vector3& translation, const lmath::Quaternion& rotation)
    {
        lmath::TransQuat2UnitDualQuat(rotation, translation, dq_);
    }

    inline void DualQuaternion::set(const lmath::Quaternion& rotation, const lmath::Quaternion& translation)
    {
        dq_[0]= rotation;
        dq_[1] = translation;
    }

    inline const f32* DualQuaternion::get() const
    {
        return reinterpret_cast<const f32*>(dq_);
    }

    inline DualQuaternion& DualQuaternion::operator+=(const DualQuaternion& q)
    {
        dq_[0] += q.dq_[0];
        dq_[1] += q.dq_[1];
        return *this;
    }

    inline DualQuaternion& DualQuaternion::operator-=(const DualQuaternion& q)
    {
        dq_[0] -= q.dq_[0];
        dq_[1] -= q.dq_[1];
        return *this;
    }

    inline DualQuaternion& DualQuaternion::operator*=(f32 f)
    {
        dq_[0] *= f;
        dq_[1] *= f;
        return *this;
    }

    inline void DualQuaternion::normalizeRot()
    {
        dq_[0].normalize();
    }

    // 後から平行移動
    inline void DualQuaternion::translate(f32 x, f32 y, f32 z)
    {
        translate(Vector3(x, y, z));
    }

    // 後から平行移動
    inline void DualQuaternion::translate(const lmath::Vector3& v)
    {
        Vector3 v0 = v;
        v0 *= 0.5f;

        lmath::Quaternion t0;
        t0.mul(dq_[0], v0);

        //回転は変わらない
        dq_[1] += t0;
    }

    // 前から平行移動
    inline void DualQuaternion::preTranslate(f32 x, f32 y, f32 z)
    {
        preTranslate(Vector3(x, y, z));
    }

    // 前から平行移動
    inline void DualQuaternion::preTranslate(const lmath::Vector3& v)
    {
        Vector3 v0 = v;
        v0 *= 0.5f;

        lmath::Quaternion t1;
        t1.mul(v0, dq_[0]);

        //回転は変わらない
        dq_[1] += t1;
    }


    // 後から回転
    inline void DualQuaternion::rotate(const lmath::Quaternion& r)
    {
        dq_[0] *= r;
        dq_[1] *= r;
    }

    // 前から回転
    inline void DualQuaternion::preRotate(const lmath::Quaternion& r)
    {
        dq_[0].mul(r, dq_[0]);
        dq_[1].mul(r, dq_[1]);
    }
}


#endif //INC_LMATH_DUALQUATERNION_H__
