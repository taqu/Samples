#ifndef INC_SKELETONPOSE_H__
#define INC_SKELETONPOSE_H__
/**
@file SkeletonPose.h
@author t-sakai
@date 2010/11/12 create

*/
#include "lanim.h"
namespace lmath
{
    class Matrix34;
}

namespace lanim
{
    class JointPose;

    class SkeletonPose
    {
    public:
        SkeletonPose();
        explicit SkeletonPose(u32 numJoints);
        ~SkeletonPose();

        /// ジョイント数取得
        inline u32 getNumJoints() const;

        inline JointPose* getPoses();
        inline const JointPose* getPoses() const;

        inline lmath::Matrix34* getMatrices();
        inline const lmath::Matrix34* getMatrices() const;


        /// スワップ
        void swap(SkeletonPose& rhs);
    private:
        u32 numJoints_;
        JointPose* localPoses_;
        lmath::Matrix34* matrix_;
    };

    // ジョイント数取得
    inline u32 SkeletonPose::getNumJoints() const
    {
        return numJoints_;
    }

    inline JointPose* SkeletonPose::getPoses()
    {
        return localPoses_;
    }

    inline const JointPose* SkeletonPose::getPoses() const
    {
        return localPoses_;
    }

    inline lmath::Matrix34* SkeletonPose::getMatrices()
    {
        return matrix_;
    }

    inline const lmath::Matrix34* SkeletonPose::getMatrices() const
    {
        return matrix_;
    }
}
#endif //INC_SKELETONPOSE_H__
