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
        inline JointAnimation(u32 numPoses);
        inline ~JointAnimation();

        inline void initialize();

        /// ポーズ数取得
        inline u32 getNumPoses() const;

        inline void setNumPoses(u32 numPoses);

        /// 名前取得
        inline const Name& getName() const;

        /// 名前セット
        inline void setName(const Name& name);
        inline void setName(const Char* name, u32 length);

        /// ジョイントポーズ取得
        inline const JointPoseWithFrame& getPose(u32 index) const;
        inline JointPoseWithFrame& getPose(u32 index);

        /// ジョイントポーズ設定
        inline void setPose(u32 index, JointPoseWithFrame& pose);

        u32 binarySearchIndex(u32 frame) const;

        /// 次のフレームのインデックスを探索。順方向のみ
        inline u32 getNextIndex(u32 frame);

        inline const JointPoseWithFrame& binarySearch(u32 frame) const
        {
            return poses_[ binarySearchIndex(frame) ];
        }
    private:
        Name name_;
        u32 numPoses_;
        u32 currentIndex_;
        JointPoseWithFrame *poses_;
    };

    inline JointAnimation::JointAnimation()
        :numPoses_(0)
        ,currentIndex_(0)
        ,poses_(NULL)
    {
    }


    inline JointAnimation::JointAnimation(u32 numPoses)
        :numPoses_(numPoses)
        ,currentIndex_(0)
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
        currentIndex_ = 0;
    }

    // ポーズ数取得
    inline u32 JointAnimation::getNumPoses() const
    {
        return numPoses_;
    }

    inline void JointAnimation::setNumPoses(u32 numPoses)
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
    inline const JointPoseWithFrame& JointAnimation::getPose(u32 index) const
    {
        LASSERT(0<=index && index<numPoses_);
        return poses_[index];
    }

    // ジョイントポーズ取得
    inline JointPoseWithFrame& JointAnimation::getPose(u32 index)
    {
        LASSERT(0<=index && index<numPoses_);
        return poses_[index];
    }

    // ジョイントポーズ設定
    inline void JointAnimation::setPose(u32 index, JointPoseWithFrame& pose)
    {
        LASSERT(0<=index && index<numPoses_);
        poses_[index] = pose;
    }

    // 次のフレームのインデックスを探索。順方向のみ
    inline u32 JointAnimation::getNextIndex(u32 frame)
    {
        for(u32 i=currentIndex_; i<numPoses_; ++i){
            if(frame==poses_[i].frameNo_){
                break;
            }else if(frame<poses_[i].frameNo_){
                currentIndex_ = (i == 0)? 0 : i-1;
                break;
            }
        }
        return currentIndex_;
    }
}
#endif //INC_JOINTANIMATION_H__
