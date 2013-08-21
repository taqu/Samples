#ifndef INC_LANIM_JOINT_H__
#define INC_LANIM_JOINT_H__
/**
@file Joint.h
@author t-sakai
@date 2010/11/12 create

*/
#include <lmath/Vector3.h>
#include "lanim.h"

namespace lanim
{
    class Joint
    {
    public:
        Joint()
            :parentIndex_(InvalidJointIndex)
            ,subjectTo_(InvalidJointIndex)
            ,type_(0)
            ,flag_(0)
        {}

        ~Joint()
        {}

        u8 getParentIndex() const{ return parentIndex_;}
        void setParentIndex(u8 index){ parentIndex_ = index;}

        u8 getSubjectTo() const{ return subjectTo_;}
        void setSubjectTo(u8 index){ subjectTo_ = index;}

        /// 位置取得
        const lmath::Vector3& getPosition() const{ return position_;}

        /// 位置セット
        void setPosition(const lmath::Vector3& position){ position_ = position;}

        u8 getType() const{ return type_;}
        void setType(u8 type){ type_ = type;}

        u8 getFlag() const{ return flag_;}
        void setFlag(u8 flag){ flag_ = flag;}

        inline Joint& operator=(const Joint& rhs);
    private:
        u8 parentIndex_;
        u8 subjectTo_;
        u8 type_;
        u8 flag_;
        lmath::Vector3 position_; /// オブジェクトローカル位置
    };

    inline Joint& Joint::operator=(const Joint& rhs)
    {
        parentIndex_ = rhs.parentIndex_;
        subjectTo_ = rhs.subjectTo_;
        type_ = rhs.type_;
        flag_ = rhs.flag_;
        position_ = rhs.position_;
        return *this;
    }
}
#endif //INC_LANIM_JOINT_H__
