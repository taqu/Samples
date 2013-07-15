#ifndef INC_LANIM_ANIMATIONCONTROLER_H__
#define INC_LANIM_ANIMATIONCONTROLER_H__
/**
@file AnimationControler.h
@author t-sakai
@date 2010/11/16 create

*/
#include "../Flags.h"
#include "lanim.h"
#include "AnimationClip.h"
#include "SkeletonPose.h"
#include "Skeleton.h"

namespace lanim
{
    enum AnimFlag
    {
        AnimFlag_Active = (0x01U << 0), //アクティブか
        AnimFlag_Loop = (0x01U << 1),
        AnimFlag_AutoDisactive = (0x01U << 2),
        AnimFlag_Num = 3,
    };

    /**
    @brief コントロールベース
    */
    class AnimationControler
    {
    public:
        virtual ~AnimationControler(){}

        virtual void initialize() = 0;
        virtual void update() =0;
        virtual void reset(f32 frame) =0;

        virtual void setSkeleton(Skeleton::pointer& skeleton) = 0;

        /// ポーズ取得
        virtual const SkeletonPose& getSkeletonPose() const = 0;
        virtual SkeletonPose& getSkeletonPose() = 0;
    protected:
        AnimationControler()
        {}

    private:
        AnimationControler(const AnimationControler&);
        AnimationControler& operator=(const AnimationControler&);
    };


    /**
    @brief アニメーションリソース
    */
    class AnimationControlerResource
    {
    public:
        AnimationControlerResource()
            :frameDuration_(1.0f)
        {}

        virtual ~AnimationControlerResource(){}

        void setSkeleton(Skeleton::pointer& skeleton);

        const SkeletonPose& getSkeletonPose() const{ return skeletonPose_;}
        SkeletonPose& getSkeletonPose(){ return skeletonPose_;}

        ///
        inline f32 getFrameDuration() const{ return frameDuration_;}
    protected:
        f32 frameDuration_;
        Skeleton::pointer skeleton_;
        SkeletonPose skeletonPose_;

    private:
        AnimationControlerResource(const AnimationControlerResource&);
        AnimationControlerResource& operator=(const AnimationControlerResource&);
    };

}
#endif //INC_LANIM_ANIMATIONCONTROLER_H__
