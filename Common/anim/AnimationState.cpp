/**
@file Animation.cpp
@author t-sakai
@date 2013/08/02 create
*/
#include "AnimationState.h"

namespace anim
{
    AnimationState::AnimationState()
        :flags_(Flag_None)
        ,time_(0.0f)
    {
    }

    AnimationState::~AnimationState()
    {
    }
}
