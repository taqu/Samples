#ifndef INC_LMATH_MATRIX34_H__
#define INC_LMATH_MATRIX34_H__
/**
@file Matrix34.h
@author t-sakai
@date 2011/06/29 create
*/
#include "lmathcore.h"
#include "Vector3.h"

namespace lmath
{
    class Matrix44;

    //--------------------------------------------
    //---
    //--- Matrix34
    //---
    //--------------------------------------------
    /**
    左手座標系、列ベクトル、ベクトルに対して左から掛けて変換する。
    */
    class Matrix34
    {
    public:
        Matrix34()
        {}

        Matrix34(f32 m00, f32 m01, f32 m02, f32 m03,
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23)
        {
            m_[0][0] = m00; m_[0][1] = m01; m_[0][2] = m02; m_[0][3] = m03;
            m_[1][0] = m10; m_[1][1] = m11; m_[1][2] = m12; m_[1][3] = m13;
            m_[2][0] = m20; m_[2][1] = m21; m_[2][2] = m22; m_[2][3] = m23;
        }

        explicit Matrix34(const Matrix44& rhs);

        ~Matrix34()
        {}

        /// 値セット
        void set(f32 m00, f32 m01, f32 m02, f32 m03,
            f32 m10, f32 m11, f32 m12, f32 m13,
            f32 m20, f32 m21, f32 m22, f32 m23)
        {
            m_[0][0] = m00; m_[0][1] = m01; m_[0][2] = m02; m_[0][3] = m03;
            m_[1][0] = m10; m_[1][1] = m11; m_[1][2] = m12; m_[1][3] = m13;
            m_[2][0] = m20; m_[2][1] = m21; m_[2][2] = m22; m_[2][3] = m23;
        }

        /// 要素アクセス
        inline f32 operator()(s32 r, s32 c) const;

        /// 要素アクセス
        inline f32& operator()(s32 r, s32 c);

        Matrix34& operator*=(f32 f);
        Matrix34& operator*=(const Matrix34& rhs);
        Matrix34& operator+=(const Matrix34& rhs);
        Matrix34& operator-=(const Matrix34& rhs);
        Matrix34& operator=(const Matrix34& rhs);

        Matrix34 operator-() const;

        void identity();

        /// 3x3部分行列の転置
        void transpose33();

        /// 3x3部分行列の行列式
        f32 determinant33() const;

        /// 3x3部分行列の逆行列
        void invert33();

        /// 逆行列
        void invert();

        /// 平行移動セット
        inline void setTranslate(const Vector3& v);

        /// 平行移動セット
        inline void setTranslate(f32 x, f32 y, f32 z);

        /// 後から平行移動
        void translate(const Vector3& v);

        /// 後から平行移動
        void translate(f32 x, f32 y, f32 z);

        /// 前から平行移動
        inline void preTranslate(const Vector3& v);

        /// 前から平行移動
        void preTranslate(f32 x, f32 y, f32 z);

        /// 後から回転
        void rotateX(f32 radian);

        /// 後から回転
        void rotateY(f32 radian);

        /// 後から回転
        void rotateZ(f32 radian);

        /// 軸回転
        void setRotateAxis(const Vector3& axis, f32 radian)
        {
            setRotateAxis(axis.x_, axis.y_, axis.z_, radian);
        }

        /// 軸回転
        void setRotateAxis(f32 x, f32 y, f32 z, f32 radian);

        inline void setScale(f32 s);
        inline void scale(f32 s);

        void mul(const Matrix34& m0, const Matrix34& m1);

        Matrix34& operator*=(const Matrix44& rhs);

        bool isNan() const;

#if defined(LMATH_USE_SSE)
        //SSEセット・ストア命令
        static void load(lm128& r0, lm128& r1, lm128& r2, const Matrix34& m);
        static void store(Matrix34& m, const lm128& r0, const lm128& r1, const lm128& r2);
#endif

        f32 m_[3][4];
    };

    inline f32 Matrix34::operator()(s32 r, s32 c) const
    {
        LASSERT(0<= r && r < 3);
        LASSERT(0<= c && c < 4);

        return m_[r][c];
    }

    inline f32& Matrix34::operator()(s32 r, s32 c)
    {
        LASSERT(0<= r && r < 3);
        LASSERT(0<= c && c < 4);

        return m_[r][c];
    }

    // 平行移動セット
    inline void Matrix34::setTranslate(const Vector3& v)
    {
        m_[0][3] = v.x_;
        m_[1][3] = v.y_;
        m_[2][3] = v.z_;

    }

    // 平行移動セット
    inline void Matrix34::setTranslate(f32 x, f32 y, f32 z)
    {
        m_[0][3] = x;
        m_[1][3] = y;
        m_[2][3] = z;
    }

    // 前から平行移動
    inline void Matrix34::preTranslate(const Vector3& v)
    {
        preTranslate(v.x_, v.y_, v.z_);
    }

    inline void Matrix34::setScale(f32 s)
    {
        m_[0][0] = s;
        m_[1][1] = s;
        m_[2][2] = s;
    }

    inline void Matrix34::scale(f32 s)
    {
        m_[0][0] *= s;
        m_[1][1] *= s;
        m_[2][2] *= s;
    }

}
#endif //INC_LMATH_MATRIX34_H__
