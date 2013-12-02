#ifndef INC_RENDER_VOLUME_H__
#define INC_RENDER_VOLUME_H__
/**
@file Voxel.h
@author t-sakai
@date 2013/09/30 create
*/
#include "render/render.h"
#include <lmath/Quaternion.h>
#include <lmath/Vector3.h>
#include <lmath/Vector4.h>
#include <lmath/geometry/Ray.h>

namespace lmath
{
    class Ray;
}

namespace render
{
    class Voxel
    {
    public:
        Voxel();
        ~Voxel();

        bool intersect(f32& tmin, f32& tmax, const lmath::Ray& ray) const;

        const lmath::Quaternion& getOrientation() const;
        lmath::Quaternion& getOrientation();
        void setOrientation(const lmath::Quaternion& orientation);

        const lmath::Vector3& getPosition() const;
        lmath::Vector3& getPosition();
        void setPosition(const lmath::Vector3& position);

        const lmath::Vector3& getHalfSize() const;
        lmath::Vector3& getHalfSize();
        void setHalfSize(const lmath::Vector3& halfSize);

        const lmath::Vector4& getColor() const;
        lmath::Vector4& getColor();
        void setColor(const lmath::Vector4& color);
    private:
        Voxel(const Voxel&);
        Voxel& operator=(const Voxel&);

        lmath::Quaternion orientation_;
        lmath::Vector3 position_;
        lmath::Vector3 halfSize_;
        lmath::Vector4 color_;
    };
}

#endif //INC_RENDER_VOLUME_H__
