/**
@file CameraThirdPerson.cpp
@author t-sakai
@date 2013/07/16 create
*/
#include "CameraThirdPerson.h"

#include <lframework/scene/Scene.h>
#include "System.h"

namespace scene
{
    CameraThirdPerson::CameraThirdPerson()
        :distance_(30.0f)
        ,height_(5.0f)
    {
        position_.zero();
        orientation_.identity();
    }

    CameraThirdPerson::~CameraThirdPerson()
    {
    }

    void CameraThirdPerson::update(const lmath::Vector4& target, const lmath::Quaternion& orientation)
    {
        orientation_ = orientation;
        position_.set(0.0f, height_, -distance_, 0.0f);
        position_.rotate(orientation_);

        lmath::Vector4 back = position_;
        back.normalize();

        position_ += target;

        lmath::Vector4 up;
        if(0.99f<back.y_){
            up.set(0.0f, 0.0f, 1.0f, 0.0f);
        }else if(back.y_<-0.99f){
            up.set(0.0f, 0.0f, -1.0f, 0.0f);
        }else{
            up.set(0.0f, 1.0f, 0.0f, 0.0f);
        }

        lmath::Matrix44 view;
        view.lookAt(position_, target, up);

        lscene::Camera& camera = fractal::System::getScene().getCamera();

        camera.setViewMatrix(view);
        camera.updateMatrix();
    }
}
