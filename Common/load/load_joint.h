#ifndef INC_LOAD_JOINT_H__
#define INC_LOAD_JOINT_H__
/**
@file load_joint.h
@author t-sakai
@date 2014/06/02 create
*/
#include "load.h"
#include <lmath/Vector3.h>

namespace load
{
    class Joint
    {
    public:
        u8 parent_;
        u8 subjectTo_;
        u8 type_;
        u8 flag_;
        lmath::Vector3 position_; /// オブジェクトローカル位置
        Char name_[MaxNameSize];
    };
}
#endif //INC_LOAD_JOINT_H__
