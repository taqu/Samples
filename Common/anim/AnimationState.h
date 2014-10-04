#ifndef INC_ANIM_ANIMATIONSTATE_H__
#define INC_ANIM_ANIMATIONSTATE_H__
/**
@file Animation.h
@author t-sakai
@date 2013/08/02 create
*/
#include "anim.h"
#include <lframework/anim/AnimationClip.h>

namespace anim
{
    class AnimationState
    {
    public:
        enum Blend
        {
            Blend_Mix = (0x01U<<0),
            Blend_Add = (0x01U<<1),
        };

        enum Flag
        {
            Flag_None = 0,
            Flag_Loop = (0x01U<<0),
        };

        AnimationState();
        ~AnimationState();

        bool isLoop() const{ return 0 != (flags_&Flag_Loop);}

        u32 flags_;
        f32 time_;
        lanim::AnimationClip::pointer clip_;
    };
}
#endif //INC_ANIM_ANIMATIONSTATE_H__
