/**
@file Voxel.cpp
@author t-sakai
@date 2013/09/30 create
*/
#include "Voxel.h"
#include <lmath/geometry/RayTest.h>

namespace render
{
    Voxel::Voxel()
    {
    }

    Voxel::~Voxel()
    {
    }

    bool Voxel::intersect(f32& tmin, f32& tmax, const lmath::Ray& ray) const
    {
        lmath::Quaternion inv;
        inv.conjugate(orientation_);

        lmath::Ray rayInOBB;
        rayInOBB.t_ = ray.t_;
        rayInOBB.origin_.sub(ray.origin_, position_);
        rayInOBB.direction_.rotate(inv, ray.direction_);
        rayInOBB.invertDirection();

        lmath::Vector3 bmax = halfSize_;
        lmath::Vector3 bmin = -bmax;

        return lmath::testRayAABB(tmin, tmax, rayInOBB, bmin, bmax);
    }

    const lmath::Quaternion& Voxel::getOrientation() const
    {
        return orientation_;
    }

    lmath::Quaternion& Voxel::getOrientation()
    {
        return orientation_;
    }

    void Voxel::setOrientation(const lmath::Quaternion& orientation)
    {
        orientation_ = orientation;
    }

    const lmath::Vector3& Voxel::getPosition() const
    {
        return position_;
    }

    lmath::Vector3& Voxel::getPosition()
    {
        return position_;
    }

    void Voxel::setPosition(const lmath::Vector3& position)
    {
        position_ = position;
    }

    const lmath::Vector3& Voxel::getHalfSize() const
    {
        return halfSize_;
    }

    lmath::Vector3& Voxel::getHalfSize()
    {
        return halfSize_;
    }

    void Voxel::setHalfSize(const lmath::Vector3& halfSize)
    {
        halfSize_ = halfSize;
    }

    const lmath::Vector4& Voxel::getColor() const
    {
        return color_;
    }

    lmath::Vector4& Voxel::getColor()
    {
        return color_;
    }

    void Voxel::setColor(const lmath::Vector4& color)
    {
        color_ = color;
    }
}
