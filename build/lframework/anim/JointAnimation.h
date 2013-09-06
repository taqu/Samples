#ifndef INC_JOINTANIMATION_H__
#define INC_JOINTANIMATION_H__
/**
@file JointAnimation.h
@author t-sakai
@date 2010/11/12 create

*/
#include "lanim.h"
#include "JointPose.h"

namespace lanim
{
    class JointAnimation
    {
    public:
        inline JointAnimation();
        inline JointAnimation(s32 numPoses);
        inline ~JointAnimation();

        inline void initialize();

        /// ポーズ数取得
        inline s32 getNumPoses() const;

        inline void setNumPoses(s32 numPoses);

        /// 名前取得
        inline const Name& getName() const;

        /// 名前セット
        inline void setName(const Name& name);
        inline void setName(const Char* name, u32 length);

        /// ジョイントポーズ取得
        inline const JointPoseWithFrame& getPose(s32 index) const;
        inline JointPoseWithFrame& getPose(s32 index);

        /// ジョイントポーズ設定
        inline void setPose(s32 index, JointPoseWithFrame& pose);

        s32 binarySearchIndex(u32 frame) const;

        /// 次のフレームのインデックスを探索。順方向のみ
        inline s32 getNextIndex(u32 frame, s32 index) const;

        /// 次のフレームのインデックスを探索。逆方向のみ
        inline s32 getPrevIndex(u32 frame, s32 index) const;

        inline const JointPoseWithFrame& binarySearch(u32 frame) const
        {
            return poses_[ binarySearchIndex(frame) ];
        }
    private:
        Name name_;
        s32 numPoses_;
        //u32 currentIndex_;
        JointPoseWithFrame *poses_;
    };

    inline JointAnimation::JointAnimation()
        :numPoses_(0)
        //,currentIndex_(0)
        ,poses_(NULL)
    {
    }


    inline JointAnimation::JointAnimation(s32 numPoses)
        :numPoses_(numPoses)
        //,currentIndex_(0)
        ,poses_(NULL)
    {
        LASSERT(numPoses_>=0);
        poses_ = LIME_NEW JointPoseWithFrame[numPoses_];
    }

    inline JointAnimation::~JointAnimation()
    {
        LIME_DELETE_ARRAY(poses_);
    }

    inline void JointAnimation::initialize()
    {
        //currentIndex_ = 0;
    }

    // ポーズ数取得
    inline s32 JointAnimation::getNumPoses() const
    {
        return numPoses_;
    }

    inline void JointAnimation::setNumPoses(s32 numPoses)
    {
        LASSERT(numPoses>=0);
        numPoses_ = numPoses;
        LIME_DELETE_ARRAY(poses_);
        poses_ = LIME_NEW JointPoseWithFrame[numPoses_];
    }

    // 名前取得
    inline const Name& JointAnimation::getName() const
    {
        return name_;
    }

    // 名前セット
    inline void JointAnimation::setName(const Name& name)
    {
        name_ = name;
    }

    inline void JointAnimation::setName(const Char* name, u32 length)
    {
        name_.assign(name, length);
    }

    // ジョイントポーズ取得
    inline const JointPoseWithFrame& JointAnimation::getPose(s32 index) const
    {
        LASSERT(0<=index && index<numPoses_);
        return poses_[index];
    }

    // ジョイントポーズ取得
    inline JointPoseWithFrame& JointAnimation::getPose(s32 index)
    {
        LASSERT(0<=index && index<numPoses_);
        return poses_[index];
    }

    // ジョイントポーズ設定
    inline void JointAnimation::setPose(s32 index, JointPoseWithFrame& pose)
    {
        LASSERT(0<=index && index<numPoses_);
        poses_[index] = pose;
    }

    // 次のフレームのインデックスを探索。順方向のみ
    inline s32 JointAnimation::getNextIndex(u32 frame, s32 index) const
    {
        for(s32 i=index+1; i<numPoses_; ++i){
            if(frame<=poses_[i].frameNo_){
                index = i;
                break;
            }
        }
        return index;
    }

    // 次のフレームのインデックスを探索。逆方向のみ
    inline s32 JointAnimation::getPrevIndex(u32 frame, s32 index) const
    {
        for(s32 i=index-1; 0<=i; --i){
            if(poses_[i].frameNo_<=frame){
                index = i;
                break;
            }
        }
        return index;
    }
}
#endif //INC_JOINTANIMATION_H__
