/**
@file DebugCamera.cpp
@author t-sakai
@date 2012/02/24 create
*/
#include "stdafx.h"
#include "DebugCamera.h"

#include <lmath/Quaternion.h>
#include <lframework/scene/Scene.h>
#include <lframework/scene/Camera.h>

#include "Input.h"

namespace fractal
{
    void DebugCamera::initialize(lscene::Camera& camera, const lmath::Vector3& position, const lmath::Vector3& target)
    {
        up_.set(0.0f, 1.0f, 0.0f);
        rotation_.set(0.0f, 0.0f, 0.0f);

        // カメラ設定

        position_ = position;
        direction_.sub(target, position);
        direction_.normalize();

        lmath::Matrix44 mat;
        mat.lookAt(position, target, up_);
        camera.setViewMatrix(mat);

        camera.updateMatrix();

        matRotation_.identity();
    }

    void DebugCamera::update(lscene::Camera& camera)
    {
        static const f32 Speed = 0.5f;
        static const f32 DurationToRad = (0.005f*PI);
        static const f32 MinRadX = -PI_2*0.95f;
        static const f32 MaxRadX = PI_2*0.95f;

        // カメラ設定

        Input::Keyboard keyboard = Input::getKeyboard();

        if(keyboard.isOn(Input::Key_W)){
            position_.x_ -= Speed * matRotation_.m_[0][2];
            position_.y_ -= Speed * matRotation_.m_[1][2];
            position_.z_ -= Speed * matRotation_.m_[2][2];
        }

        if(keyboard.isOn(Input::Key_S)){
            position_.x_ += Speed * matRotation_.m_[0][2];
            position_.y_ += Speed * matRotation_.m_[1][2];
            position_.z_ += Speed * matRotation_.m_[2][2];
        }


        if(keyboard.isOn(Input::Key_A)){
            position_.x_ += Speed * matRotation_.m_[0][0];
            position_.y_ += Speed * matRotation_.m_[1][0];
            position_.z_ += Speed * matRotation_.m_[2][0];
        }


        if(keyboard.isOn(Input::Key_D)){
            position_.x_ -= Speed * matRotation_.m_[0][0];
            position_.y_ -= Speed * matRotation_.m_[1][0];
            position_.z_ -= Speed * matRotation_.m_[2][0];
        }

        if(keyboard.isOn(Input::Key_R)){
            rotation_.zero();
        }


        Input::Mouse mouse = Input::getMouse();
        if(mouse.isOn(Input::MouseButton_Left)){
            rotation_.x_ -= DurationToRad * mouse.getAxisDuration(Input::MouseAxis_Y);
            rotation_.y_ += DurationToRad * mouse.getAxisDuration(Input::MouseAxis_X);

            rotation_.x_ = lcore::clamp(rotation_.x_, MinRadX, MaxRadX);
            if(rotation_.y_>PI2){
                rotation_.y_ -= PI2;
            }else if(rotation_.y_<-PI2){
                rotation_.y_ += PI2;
            }
            lmath::Vector3 axisX(1.0f, 0.0f, 0.0f);
            lmath::Vector3 axisY(0.0f, 1.0f, 0.0f);

            lmath::Quaternion rotX;
            rotX.setRotateAxis(axisX, rotation_.x_);

            lmath::Quaternion rotY;
            rotY.setRotateAxis(axisY, rotation_.y_);

            rotX *= rotY;
            rotX.getMatrix(matRotation_);

            direction_.set(0.0f, 0.0f, -1.0f);
            direction_.mul33(matRotation_, direction_);
        }
        lmath::Vector3 target;
        target.add(position_, direction_);

        lmath::Matrix44 mat;
        mat.lookAt(position_, target, up_);

        camera.setViewMatrix(mat);

        camera.updateMatrix();
    }
}
