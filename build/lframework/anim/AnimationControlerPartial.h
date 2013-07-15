#ifndef INC_ANIMATIONCONTROLERPARTIAL_H__
#define INC_ANIMATIONCONTROLERPARTIAL_H__
/**
@file AnimControlerPartial.h
@author t-sakai
@date 2010/11/17 create
@date 2011/04/07 更新単位を整数化
*/
#include <lcore/Vector.h>
#include "AnimationControler.h"

namespace lanim
{
    //--------------------------------------------------------------
    //---
    //--- AnimationControlerPartial
    //---
    //--------------------------------------------------------------
    /**
    */
    class AnimationControlerPartial
    {
    public:
        struct JointMap
        {
            u32 frame_;
            u16 joint_;
            u16 clipJoint_;
        };
        typedef lcore::vector_arena<JointMap> JointMapVector;
        typedef lframework::Flags Flags;

        AnimationControlerPartial();
        AnimationControlerPartial(const AnimationControlerPartial& rhs);
        ~AnimationControlerPartial();

        AnimationControlerPartial& operator=(const AnimationControlerPartial& rhs);

        /**
        @brief クリップ登録
        @param clip ... クリップ
        */
        bool setClip(AnimationClip::pointer& clip)
        {
            LASSERT(clip != NULL);
            clip_ = clip;
            return resetClip();
        }

        inline void setSkeleton(Skeleton::pointer& skeleton);

        /// クリップ取得
        inline const AnimationClip* getClip() const;

        /**
        @brief ポーズセット。
        @todo ポーズをセットしておくインターフェイスはどうかと思う
        */
        inline void setSkeletonPose(SkeletonPose& skeletonPose);

        bool resetClip();

        /// ジョイントインデックス数取得
        inline u32 getNumJointIndices() const;

        /// ジョイントインデックス取得
        inline const JointMap& getJointMap(u32 index) const;

        /// フラグセット取得
        inline const Flags& getFlags() const;

        /// フラグセット取得
        inline Flags& getFlags();

        /// 現在のフレーム時間取得
        inline f32 getFrame() const;

        /// 現在のフレーム時間セット
        inline void setFrame(f32 frame);

        /// ブレンド率取得
        inline f32 getBlendRate() const;

        /// ブレンド率セット
        inline void setBlendRate(f32 rate);

        /// フレーム時間更新
        void updateFrame(f32 duration);


        void initialize();

        void getPose();
        void getPoseLoop();

        void blendPose();
        void blendPoseLoop();

        void updateMatrix();
    protected:
        Flags flags_;
        Skeleton::pointer skeleton_;
        f32 frame_;
        f32 blendRate_;
        AnimationClip::pointer clip_;
        JointMapVector jointMap_;
        SkeletonPose* skeletonPose_;
    };


    //--------------------------------------------------------------
    //--- AnimationControlerPartial implementation
    //--------------------------------------------------------------
    // スケルトンセット
    inline void AnimationControlerPartial::setSkeleton(Skeleton::pointer& skeleton)
    {
        skeleton_ = skeleton;
    }


    // クリップ取得
    inline const AnimationClip* AnimationControlerPartial::getClip() const
    {
        return clip_.get();
    }

    // ポーズセット
    inline void AnimationControlerPartial::setSkeletonPose(SkeletonPose& skeletonPose)
    {
        skeletonPose_ = &skeletonPose;
    }

    // ジョイントインデックス数取得
    inline u32 AnimationControlerPartial::getNumJointIndices() const
    {
        return jointMap_.size();
    }

    // ジョイントインデックス取得
    inline const AnimationControlerPartial::JointMap& AnimationControlerPartial::getJointMap(u32 index) const
    {
        return jointMap_[index];
    }


    // フラグセット取得
    inline const AnimationControlerPartial::Flags& AnimationControlerPartial::getFlags() const
    {
        return flags_;
    }

    // フラグセット取得
    inline AnimationControlerPartial::Flags& AnimationControlerPartial::getFlags()
    {
        return flags_;
    }


    // 現在のフレーム時間取得
    inline f32 AnimationControlerPartial::getFrame() const
    {
        return frame_;
    }

    // 現在のフレーム時間セット
    inline void AnimationControlerPartial::setFrame(f32 frame)
    {
        LASSERT(frame>=0.0f);
        frame_ = (frame > clip_->getLastFrame())? clip_->getLastFrame() : frame;
    }

    // ブレンド率取得
    inline f32 AnimationControlerPartial::getBlendRate() const
    {
        return blendRate_;
    }

    // ブレンド率セット
    inline void AnimationControlerPartial::setBlendRate(f32 rate)
    {
        blendRate_ = rate;
    }

}

#endif //INC_ANIMATIONCONTROLERPARTIAL_H__
