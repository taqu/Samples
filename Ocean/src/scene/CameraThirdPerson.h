#ifndef INC_SCENE_CAMERATHIRDPERSON_H__
#define INC_SCENE_CAMERATHIRDPERSON_H__
/**
@file CameraThirdPerson.h
@author t-sakai
@date 2013/07/16 create
*/
#include "scene.h"
#include <lmath/Quaternion.h>

namespace lmath
{
    class Vector3;
    class Vector4;
    class Quaternion;
}

namespace scene
{
    class CameraThirdPerson
    {
    public:
        CameraThirdPerson();
        ~CameraThirdPerson();

        void update(const lmath::Vector4& target, const lmath::Quaternion& orientation);

    private:
        f32 distance_;
        f32 height_;
        lmath::Vector4 position_;
        lmath::Quaternion orientation_;
    };
}
#endif //INC_SCENE_CAMERATHIRDPERSON_H__
