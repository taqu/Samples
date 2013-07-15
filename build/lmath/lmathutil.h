#ifndef INC_LMATH_LMATHUTIL_H__
#define INC_LMATH_LMATHUTIL_H__
/**
@file lmathutil.h
@author t-sakai
@date 2010/09/09 create

*/
#include "lmathcore.h"

namespace lmath
{
    class Vector3;
    class Vector4;
    class Matrix34;
    class Quaternion;

    void calcBBox(Vector3& bmin, Vector3& bmax, const Vector3& v0, const Vector3& v1, const Vector3& v2);

    void calcMedian(Vector3& median, const Vector3& v0, const Vector3& v1, const Vector3& v2);

    void calcAABBPoints(Vector4* AABB, const Vector4& aabbMin, const Vector4& aabbMax);

    /**
    @brief Ferguson-Coons曲線
    @param dst ... 出力。
    @param t ... 0-1
    @param p0 ... 点0
    @param v0 ... 点0での速度
    @param p1 ... 点1
    @param v1 ... 点1での速度
    */
    void fergusonCoonsCurve(Vector4& dst, f32 t, const Vector4& p0, const Vector4& v0, const Vector4& p1, const Vector4& v1);

    /**
    @brief Bezier曲線
    @param dst ... 出力。
    @param t ... 0-1
    @param p0 ... 点0
    @param p1 ... 点1
    @param p2 ... 点2
    @param p3 ... 点3
    */
    void cubicBezierCurve(Vector4& dst, f32 t, const Vector4& p0, const Vector4& p1, const Vector4& p2, const Vector4& p3);

    /**
    @brief Catmull-Rom曲線
    @param dst ... 出力。
    @param t ... 0-1
    @param p0 ... 点0
    @param p1 ... 点1
    @param p2 ... 点2
    @param p3 ... 点3
    */
    void catmullRomCurve(Vector4& dst, f32 t, const Vector4& p0, const Vector4& p1, const Vector4& p2, const Vector4& p3);
    void catmullRomCurveStartPoint(Vector4& dst, f32 t, const Vector4& p1, const Vector4& p2, const Vector4& p3);
    void catmullRomCurveEndPoint(Vector4& dst, f32 t, const Vector4& p0, const Vector4& p1, const Vector4& p2);

    /**
    @brief Catmull-Rom曲線
    @param dst ... 出力。
    @param t ... 0-1
    @param p0 ... 点0
    @param p1 ... 点1
    @param p2 ... 点2
    @param p3 ... 点3
    */
    void catmullRomCurve(f32& dst, f32 t, f32 p0, f32 p1, f32 p2, f32 p3);

    //--------------------------------------------------------
    //--- 四元数
    //--------------------------------------------------------
    /**
    @brief 単位四元数と移動ベクトルを単位デュアルクウォータニオンに変換

    移動してから、回転
    */
    void TransQuat2UnitDualQuat(const Quaternion& quat, const Vector3& trans, Quaternion dq[2]);

    /**
    @brief 単位四元数と移動ベクトルを単位デュアルクウォータニオンに変換

    回転してから、移動
    */
    void QuatTrans2UnitDualQuat(const Quaternion& quat, const Vector3& trans, Quaternion dq[2]);


    /**
    @brief 単位デュアルクウォータニオンを単位四元数と移動ベクトルに変換
    */
    void UnitDualQuat2QuatTrans(const f32 dq[2][4], Quaternion& quat, Vector3& trans);


    /**
    @brief デュアルクウォータニオンを単位四元数と移動ベクトルに変換

    デュアルクウォータニオンは非ゼロの非デュアルパート（四元数）の場合
    */
    void DualQuaternion2QuatTrans(const f32 dq[2][4], Quaternion& quat, Vector3& trans);

    /**
    @brief 慣性座標からオブジェクト座標へのオイラー角を取得

    慣性座標は、原点がオブジェクト座標と等しく、軸がワールド座標と平行な座標系
    */
    void getEulerInertialToObject(f32& head, f32& pitch, f32& bank, const lmath::Matrix34& m);

    /**
    @brief 慣性座標からオブジェクト座標へのオイラー角を取得

    慣性座標は、原点がオブジェクト座標と等しく、軸がワールド座標と平行な座標系
    */
    void getEulerInertialToObject(f32& head, f32& pitch, f32& bank, const lmath::Quaternion& q);


    /**
    @brief オブジェクト座標から慣性座標へのオイラー角を取得

    慣性座標は、原点がオブジェクト座標と等しく、軸がワールド座標と平行な座標系
    */
    void getEulerObjectToInertial(f32& head, f32& pitch, f32& bank, const lmath::Matrix34& m);

    /**
    @brief オブジェクト座標から慣性座標へのオイラー角を取得

    慣性座標は、原点がオブジェクト座標と等しく、軸がワールド座標と平行な座標系
    */
    void getEulerObjectToInertial(f32& head, f32& pitch, f32& bank, const lmath::Quaternion& q);

    /**
    @brief オイラー角から四元数を取得
    */
    void getQuaternionObjectToInertial(Quaternion& q, f32 head, f32 pitch, f32 bank);

    /**
    @brief オイラー角から四元数を取得
    */
    void getQuaternionInertialToObject(Quaternion& q, f32 head, f32 pitch, f32 bank);
}
#endif //INC_LMATH_LMATHUTIL_H__
