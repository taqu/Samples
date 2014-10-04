#ifndef INC_ANIM_ANIMATION_H__
#define INC_ANIM_ANIMATION_H__
/**
@file Animation.h
@author t-sakai
@date 2013/08/01 create
*/
#include "anim.h"
#include <lcore/Vector.h>
#include <lmath/Quaternion.h>
#include <lframework/anim/AnimationClip.h>
#include <lframework/anim/Skeleton.h>

#include "AnimationState.h"

namespace lmath
{
    class DualQuaternion;
}

namespace anim
{
    class Animation
    {
    public:
        static const u32 NumMaxStates = 2;

        Animation();
        ~Animation();

        void create(u32 numClips);
        void setSkeleton(lanim::Skeleton::pointer skeleton);

        void setClip(u32 index, lanim::AnimationClip::pointer& clip);
        void play(u32 index);

        void update();
        void getMatrices(lmath::Matrix34* matrices);
        void getDualQuaternion(lmath::DualQuaternion* dq);
    private:
        struct Transform
        {
            lmath::Vector3 translation_;
            lmath::Quaternion rotation_;
        };

        void clearTransforms();
        void update(f32 frame, AnimationState& state);

        lanim::Skeleton::pointer skeleton_;

        u32 numClips_;
        lanim::AnimationClip::pointer* clips_;

        u32 numStates_;
        AnimationState states_[NumMaxStates];

        Transform* transforms_;
    };
}
#endif //INC_ANIM_ANIMATION_H__
