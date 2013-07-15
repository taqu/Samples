#ifndef INC_JOINTPOSE_H__
#define INC_JOINTPOSE_H__
/**
@file JointPose.h
@author t-sakai
@date 2010/11/12 create

*/
#include "lanim.h"
#include <lmath/Vector3.h>
#include <lmath/Quaternion.h>

namespace lanim
{
    class JointPose
    {
    public:
        lmath::Vector3 offset_;
        lmath::Vector3 translation_;
        lmath::Quaternion rotation_;
    };

    class JointPoseWithFrame
    {
    public:
        u32 frameNo_;
        lmath::Vector3 translation_;
        lmath::Quaternion rotation_;
    };
}
#endif //INC_JOINTPOSE_H__
