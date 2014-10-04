#ifndef INC_DEBUGCAMERA_H__
#define INC_DEBUGCAMERA_H__
/**
@file DebugCamera.h
@author t-sakai
@date 2012/02/24 create
*/
namespace lscene
{
    class Camera;
}

namespace fractal
{
    class DebugCamera
    {
    public:
        void initialize(lscene::Camera& camera, const lmath::Vector3& position, const lmath::Vector3& target);
        void update(lscene::Camera& camera);

        lmath::Vector3 position_;
        lmath::Vector3 direction_;
        lmath::Vector3 up_;
        lmath::Vector3 rotation_;

        lmath::Matrix34 matRotation_;
    };
}
#endif //INC_DEBUGCAMERA_H__
