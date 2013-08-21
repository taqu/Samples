#ifndef INC_FRACTAL_TIMER_H__
#define INC_FRACTAL_TIMER_H__
/**
@file Timer.h
@author t-sakai
@date 2013/04/08 create
*/

#include "stdafx.h"

namespace fractal
{
    class Timer
    {
    public:
        Timer();
        ~Timer();

        void reset();
        void update();

        f32 getDeltaTime() const{ return deltaTime_;}
        f32 getInvDeltaTime() const{ return invDeltaTime_;}
    private:
        lcore::ClockType prevTime_;
        u32 frameCount_;
        f32 deltaTime_;
        f32 invDeltaTime_;
        f32 accumulation_[8];
    };
}
#endif //INC_FRACTAL_TIMER_H__
