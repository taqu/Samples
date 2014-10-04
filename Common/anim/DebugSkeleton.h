#ifndef INC_RENDER_DEBUGSKELETON_H__
#define INC_RENDER_DEBUGSKELETON_H__
/**
@file DebugSkeleton.h
@author t-sakai
@date 2013/07/31 create
*/
#include "anim.h"
#include <lframework/anim/Skeleton.h>

namespace render
{
    class AnimObject;
}

namespace load
{
    struct VertexPNUBone;
}

namespace anim
{
    class DebugSkeleton
    {
    public:
        static void create(render::AnimObject& animObject, const lanim::Skeleton::pointer& skeleton);

    private:
        static void calcOffset(lmath::Vector3& offset, u8 jointIndex, const lanim::Skeleton::pointer& skeleton);
    };
}
#endif //INC_RENDER_DEBUGSKELETON_H__
