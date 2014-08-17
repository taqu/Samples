#ifndef INC_LMATH_RAYTEST_H__
#define INC_LMATH_RAYTEST_H__
/**
@file RayTest.h
@author t-sakai
@date 2009/12/21 create
*/
#include "../lmathcore.h"

namespace lmath
{
    class Ray;
    class Plane;
    class Sphere;
    class Vector3;

    //-----------------------------------------------------------
    /**
    @brief 線分と平面の交差判定
    @return 交差するか
    @param t ... 交差点の陰関数パラメータ。交差点 = t*ray.direction_ + ray.origin_
    @param ray ... 
    @param plane ... 
    */
    bool testRayPlane(f32& t, const Ray& ray, const Plane& plane);

    //-----------------------------------------------------------
    /**
    @brief 線分と球の交差判定
    @return 交差するか
    @param t ... 交差点の陰関数パラメータ。交差点 = t*ray.direction_ + ray.origin_
    @param ray ... 
    @param sphere ... 
    */
    bool testRaySphere(f32& t, const Ray& ray, const Sphere& sphere);

    //-----------------------------------------------------------
    /**
    @brief 線分と三角形の交差判定
    @return 交差するか
    @param t ... 交差点の陰関数パラメータ。交差点 = t*ray.direction_ + ray.origin_
    @param ray ... 
    @param v0 ... 
    @param v1 ... 
    @param v2 ... 
    */
    bool testRayTriangle(f32& t, f32& u, f32& v, const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2);

    bool testRayRectangle(f32& t, const Ray& ray, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3);


    //-----------------------------------------------------------
    /**
    @brief 線分とAABBの交差判定
    @return 交差するか
    @param tmin ... 
    @param tmax ...
    @param ray ... 
    @param bmin ... 
    @param bmax ... 

    交差点の陰関数パラメータを最大最小返す
    */
    bool testRayAABB(f32& tmin, f32& tmax, const Ray& ray, const Vector3& bmin, const Vector3& bmax);

    bool testRayAABB(const Ray& ray, const Vector3& bmin, const Vector3& bmax);

    bool testRayAABB(f32& tmin, f32& tmax, const Ray& ray, const Vector4& bmin, const Vector4& bmax);

    bool testRayAABB(const Ray& ray, const Vector4& bmin, const Vector4& bmax);

    //-----------------------------------------------------------
    // 線分とスラブ（平行な平面の組の間）の交差判定
    bool testRaySlab(f32& tmin, f32& tmax, const Ray& ray, f32 slabMin, f32 slabMax, s32 axis);
}

#endif //INC_LMATH_RAYTEST_H__
