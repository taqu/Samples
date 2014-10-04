/**
@file Animation.cpp
@author t-sakai
@date 2013/08/01 create
*/
#include "Animation.h"
#include <lmath/lmath.h>
#include <lmath/DualQuaternion.h>

namespace anim
{
    Animation::Animation()
        :numClips_(0)
        ,clips_(NULL)
        ,numStates_(0)
        ,transforms_(NULL)
    {
    }

    Animation::~Animation()
    {
        LIME_DELETE_ARRAY(clips_);
        LIME_DELETE_ARRAY(transforms_);
    }

    void Animation::create(u32 numClips)
    {
        LIME_DELETE_ARRAY(clips_);
        numClips_ = numClips;

        clips_ = LIME_NEW lanim::AnimationClip::pointer[numClips_];
    }

    void Animation::setSkeleton(lanim::Skeleton::pointer skeleton)
    {
        u32 numTransforms = (NULL == skeleton_.get())? 0 : skeleton_->getNumJoints();
        if(numTransforms < skeleton->getNumJoints()){
            LIME_DELETE_ARRAY(transforms_);
            transforms_ = LIME_NEW Transform[skeleton->getNumJoints()];
        }
        skeleton_ = skeleton;
        clearTransforms();
    }

    void Animation::setClip(u32 index, lanim::AnimationClip::pointer& clip)
    {
        LASSERT(0<=index && index<numClips_);
        clips_[index] = clip;
    }

    void Animation::play(u32 index)
    {
        numStates_ = 1;

        states_[0].flags_ = AnimationState::Flag_Loop;
        states_[0].time_ = 0.0f;
        states_[0].clip_ = clips_[index];
    }

    void Animation::update()
    {
        static const f32 PlayTime = 0.008f;
        static const f32 UnitTime = 0.016f;

        for(u32 i=0; i<numStates_; ++i){
            AnimationState& state = states_[i];
            f32 t = state.time_/UnitTime;
            f32 count = lcore::minimum(state.clip_->getLastFrame(), t);

            update(count, state);


            t = state.time_ + PlayTime;
            f32 totalTime = state.clip_->getLastFrame() * UnitTime;
            if(state.isLoop()){
                if(totalTime<=t){
                    t -= totalTime;
                }
            }else{
                if(totalTime<t){
                    t = totalTime;
                }
            }

            state.time_ = t;
        }
    }

    void Animation::clearTransforms()
    {
        for(u32 i=0; i<skeleton_->getNumJoints(); ++i){
            transforms_[i].translation_.zero();
            transforms_[i].rotation_.identity();
        }
    }

    void Animation::update(f32 frame, AnimationState& state)
    {
        u32 current = static_cast<u32>(frame);

        lanim::AnimationClip::pointer& clip = state.clip_;
        for(u32 i=0; i<clip->getNumJoints(); ++i){
            const lanim::Joint* joint = skeleton_->findJoint(clip->getJointAnimation(i).getName());
            if(NULL == joint){
                continue;
            }

            const lanim::JointAnimation& jointAnim = clip->getJointAnimation(i);

            u16 jointIndex = skeleton_->calcJointIndex(joint);
            s32 poseIndex = jointAnim.binarySearchIndex(current);
            s32 nextPoseIndex = jointAnim.getNextIndex(current, poseIndex);

            const lanim::JointPoseWithFrame& jointPose = jointAnim.getPose(poseIndex);
            const lanim::JointPoseWithFrame& nextPose = jointAnim.getPose(nextPoseIndex);

            f32 rate = (jointPose.frameNo_ == nextPose.frameNo_)? 0.0f : (frame-jointPose.frameNo_)/(nextPose.frameNo_-jointPose.frameNo_);
            transforms_[jointIndex].translation_.lerp(jointPose.translation_, nextPose.translation_, rate);
            transforms_[jointIndex].rotation_.lerp(jointPose.rotation_, nextPose.rotation_, rate);

            //const lanim::Name& jointAnimName = clip->getJointAnimation(i).getName();
            //const lanim::Name& jointName = skeleton_->getJointName(jointIndex);

            //lcore::Log("%s[%d] -> %s[%d] (%f, %f, %f, %f)", jointAnimName.c_str(), i, jointName.c_str(), jointIndex,
            //    jointPose.rotation_.x_, jointPose.rotation_.y_, jointPose.rotation_.z_, jointPose.rotation_.w_);
        }
    }

    void Animation::getMatrices(lmath::Matrix34* matrices)
    {
        for(u32 i=0; i<skeleton_->getNumJoints(); ++i){
            lanim::Joint& joint = skeleton_->getJoint(i);

            transforms_[i].rotation_.getMatrix( matrices[i] );

            matrices[i].preTranslate(-joint.getPosition());

            lmath::Vector3 translate;
            translate.add(transforms_[i].translation_, joint.getPosition());

            matrices[i].translate(translate);

            if(lanim::InvalidJointIndex == joint.getParentIndex()){
                continue;
            }

            const lmath::Matrix34& parentMat = matrices[joint.getParentIndex()];

            matrices[i].mul(parentMat, matrices[i]);
        }
    }

    void Animation::getDualQuaternion(lmath::DualQuaternion* dq)
    {
        for(u32 i=0; i<skeleton_->getNumJoints(); ++i){
            lanim::Joint& joint = skeleton_->getJoint(i);


            dq[i].identity();

            dq[i].translate(-joint.getPosition());

            dq[i].rotate(transforms_[i].rotation_);

            lmath::Vector3 translate;
            translate.add(transforms_[i].translation_, joint.getPosition());

            dq[i].translate(translate);

            if(lanim::InvalidJointIndex == joint.getParentIndex()){
                continue;
            }

            const lmath::DualQuaternion& parentDq = dq[joint.getParentIndex()];

            dq[i].mul(dq[i], parentDq);
        }
    }
}
