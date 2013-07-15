#ifndef INC_LMATH_PRIMITIVETEST_H__
#define INC_LMATH_PRIMITIVETEST_H__
/**
@file PrimitiveTest.h
@author t-sakai
@date 2009/12/20 create
*/
#include "../lmathcore.h"
#include "../Vector3.h"
#include "../Vector4.h"

namespace lmath
{
    class Plane;
    class Sphere;

    //---------------------------------------------------------------------------------
    /**
    @brief 点から平面上の最近傍点を計算
    @param result ... 出力
    @param point ... 入力点
    @param plane ... 入力平面
    @notice 入力平面は正規化されている
    */
    void closestPointPointVsPlane(lmath::Vector3& result, const lmath::Vector3& point, const Plane& plane);

    //---------------------------------------------------------------------------------
    /**
    @brief 点から平面への距離を計算
    @return 距離
    @param point ... 入力点
    @param plane ... 入力平面
    @notice 入力平面は正規化されている
    */
    f32 distancePointVsPlane(const lmath::Vector3& point, const Plane& plane);

    //---------------------------------------------------------------------------------
    /**
    @brief 点から線分への最近傍点を計算
    @return result = t*(l2-l1)となるt
    @param result ... 出力
    @param point ... 
    @param l1 ... 
    @param l2 ... 
    */
    f32 closestPointPointVsSegment(
        lmath::Vector3& result,
        const lmath::Vector3& point,
        const lmath::Vector3& l1,
        const lmath::Vector3& l2);

    //---------------------------------------------------------------------------------
    /**
    @brief 点から線分への距離を計算
    @return 点から線分への距離
    @param point ... 
    @param l1 ... 
    @param l2 ... 
    */
    f32 distancePointVsSegment(
        const lmath::Vector3& point,
        const lmath::Vector3& l1,
        const lmath::Vector3& l2);

    //---------------------------------------------------------------------------------
    /**
    @return 球と平面が交差するか
    @param sphere ...
    @param plane ... 
    @notice 接する場合も含む
    */
    bool testSphereVsPlane(f32 &t, const Sphere& sphere, const Plane& plane);

    //---------------------------------------------------------------------------------
    bool testSphereVsSphere(const Sphere& sphere0, const Sphere& sphere1);

    //---------------------------------------------------------------------------------
    bool testSphereVsSphere(f32& distance, const Sphere& sphere0, const Sphere& sphere1);

    //---------------------------------------------------------------------------------
    struct Triangle
    {
        Vector4 v_[3];
    };

    /**
    */
    void clipTriangle(
        s32& numTriangles,
        Triangle* triangles,
        const Plane& plane);
}

#endif //INC_LMATH_PRIMITIVETEST_H__
