#ifndef INC_LANIM_ANIMATIONCLIP_H__
#define INC_LANIM_ANIMATIONCLIP_H__
/**
@file AnimationClip.h
@author t-sakai
@date 2010/11/12 create

*/
#include "lanim.h"
#include <lcore/smart_ptr/intrusive_ptr.h>
#include "JointAnimation.h"

namespace lanim
{
    class AnimationClip
    {
    public:
        typedef smart_ptr::intrusive_ptr<AnimationClip> pointer;

        AnimationClip();
        explicit AnimationClip(u32 numJoints);
        ~AnimationClip();

        inline f32 getLastFrame() const;
        inline void setLastFrame(f32 frame);

        inline u32 getNumJoints() const;
        inline const JointAnimation& getJointAnimation(u32 index) const;
        inline JointAnimation& getJointAnimation(u32 index);

        /// 名前取得
        inline const Name& getName() const;

        /// 名前セット
        inline void setName(const Name& name);

        /// 名前セット
        inline void setName(const char* name);

        /// 名前セット
        inline void setNameMemory(const char* name, u32 size);
    private:
        AnimationClip(const AnimationClip&);
        AnimationClip& operator=(const AnimationClip&);

        friend inline void intrusive_ptr_addref(AnimationClip* ptr);
        friend inline void intrusive_ptr_release(AnimationClip* ptr);

        /// 参照カウント
        void addRef()
        {
            ++refCount_;
        }

        /// 参照カウント開放
        void release()
        {
            if(--refCount_ == 0){
                LIME_DELETE_NONULL this;
            }
        }

        s32 refCount_;
        Name name_;
        f32 lastFrame_;
        u32 numJoints_;
        JointAnimation *jointAnims_;
    };

    inline f32 AnimationClip::getLastFrame() const
    {
        return lastFrame_;
    }

    inline void AnimationClip::setLastFrame(f32 frame)
    {
        lastFrame_ = frame;
    }

    inline u32 AnimationClip::getNumJoints() const
    {
        return numJoints_;
    }

    inline const JointAnimation& AnimationClip::getJointAnimation(u32 index) const
    {
        LASSERT(0<=index && index<numJoints_);
        return jointAnims_[index];
    }

    inline JointAnimation& AnimationClip::getJointAnimation(u32 index)
    {
        LASSERT(0<=index && index<numJoints_);
        return jointAnims_[index];
    }


    // 名前取得
    inline const Name& AnimationClip::getName() const
    {
        return name_;
    }

    // 名前セット
    inline void AnimationClip::setName(const Name& name)
    {
        name_ = name;
    }

    // 名前セット
    inline void AnimationClip::setName(const char* name)
    {
        LASSERT(name != NULL);
        name_.assign(name, lcore::strlen(name));
    }

    // 名前セット
    inline void AnimationClip::setNameMemory(const char* name, u32 size)
    {
        LASSERT(name != NULL);
        name_.assignMemory(name, size);
    }

    inline void intrusive_ptr_addref(AnimationClip* ptr)
    {
        ptr->addRef();
    }

    inline void intrusive_ptr_release(AnimationClip* ptr)
    {
        ptr->release();
    }
}
#endif //INC_LANIM_ANIMATIONCLIP_H__
