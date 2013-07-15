#ifndef INC_LSCENE_CAMERA_H__
#define INC_LSCENE_CAMERA_H__
/**
@file Camera.h
@author t-sakai
@date 2009/12/20 create
*/
#include <lmath/lmath.h>
#include "lscene.h"

namespace lscene
{
    class Camera
    {
    public:
        Camera();

        inline f32 getWidth() const{ return width_;}
        inline f32 getHeight() const{ return height_;}
        inline f32 getZNear() const{ return znear_;}
        inline f32 getZFar() const{ return zfar_;}
        inline f32 getAspect() const{ return aspect_;}

        const lmath::Matrix44& getViewMatrix() const
        {
            return viewMatrix_;
        }

        lmath::Matrix44& getViewMatrix()
        {
            return viewMatrix_;
        }

        void setViewMatrix(const lmath::Matrix44& view)
        {
            viewMatrix_ = view;
        }

        const lmath::Matrix44& getProjMatrix() const
        {
            return projMatrix_;
        }

        lmath::Matrix44& getProjMatrix()
        {
            return projMatrix_;
        }

        void setProjMatrix(const lmath::Matrix44& proj);

        const lmath::Matrix44& getViewProjMatrix() const
        {
            return viewProjMatrix_;
        }

        /**
        @brief 透視投影
        */
        void perspective(f32 width, f32 height, f32 znear, f32 zfar)
        {
            znear_ = znear;
            zfar_ = zfar;
            width_ = width;
            height_ = height;
            aspect_ = width/height;
            projMatrix_.perspective(width, height, znear, zfar);
        }

        /**
        @brief 透視投影
        */
        void perspectiveFov(f32 fovy, f32 aspect, f32 znear, f32 zfar)
        {
            znear_ = znear;
            zfar_ = zfar;
            height_ = 2.0f*znear*lmath::tan(0.5f*fovy);
            width_ = height_*aspect;
            aspect_ = aspect;
            projMatrix_.perspectiveFov(fovy, aspect, znear, zfar);
        }

        /**
        @brief 平行投影
        */
        void ortho(f32 width, f32 height, f32 znear, f32 zfar)
        {
            znear_ = znear;
            zfar_ = zfar;
            width_ = width;
            height_ = height;
            aspect_ = width/height;
            projMatrix_.ortho(width, height, znear, zfar);
        }

        /**
        @brief 透視投影。リニアＺ版
        */
        void perspectiveLinearZ(f32 width, f32 height, f32 znear, f32 zfar)
        {
            znear_ = znear;
            zfar_ = zfar;
            width_ = width;
            height_ = height;
            aspect_ = width/height;
            projMatrix_.perspectiveLinearZ(width, height, znear, zfar);
        }

        /**
        @brief 透視投影。リニアＺ版
        */
        void perspectiveFovLinearZ(f32 fovy, f32 aspect, f32 znear, f32 zfar)
        {
            znear_ = znear;
            zfar_ = zfar;
            height_ = 2.0f*znear*lmath::tan(0.5f*fovy);
            width_ = height_/aspect;
            aspect_ = aspect;
            projMatrix_.perspectiveFovLinearZ(fovy, aspect, znear, zfar);
        }

        void updateMatrix();

        void getEyePosition(lmath::Vector4& eye) const;

    protected:
        f32 znear_;
        f32 zfar_;
        f32 width_;
        f32 height_;
        f32 aspect_;

        lmath::Matrix44 viewMatrix_;
        lmath::Matrix44 projMatrix_;
        lmath::Matrix44 viewProjMatrix_;
    };
}

#endif //INC_LSCENE_CAMERA_H__
