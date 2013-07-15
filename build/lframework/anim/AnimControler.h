#ifndef INC_ANIMCONTROLER_H__
#define INC_ANIMCONTROLER_H__
/**
@file AnimControler.h
@author t-sakai
@date 2009/10/04 create
*/
#include <lgraphics/lgraphicscore.h>
#include "../scene/AnimTree.h"
#include "AnimDatabase.h"

namespace lmath
{
    class Vector3;
    class Quaternion;
    class DualQuaternion;
    class Matrix43;
}

namespace lframework
{
    class AnimDatabase;
    class AnimObject;
    class Bone;

    //-----------------------------------------------------------
    //---
    //--- AnimControler
    //---
    //-----------------------------------------------------------
#if LIME_USE_DUAL_QUATERNION_FOR_SKINNING
    // デュアルクォーターニオン
    class AnimControler
    {
    public:

        AnimControler();
        explicit AnimControler(const AnimObject& object);
        ~AnimControler();

        void initialize(const AnimObject& object);

        void update(
            AnimObject& object,
            const AnimDatabase& animDB,
            f32 frame);

    private:
        void updateNode(AnimTree& tree, const AnimDataNodeTree& dataTree, lmath::DualQuaternion parent, f32 frame);

        //Bone *currentBones_;
        lmath::Quaternion *currentRotation_;
        lmath::Vector3 *currentTranslation_;
    };
#else
    // 4x3マトリックス
    class AnimControler
    {
    public:

        AnimControler();
        AnimControler(AnimObject& object, AnimDatabase& animDB);
        ~AnimControler();

        void initialize(AnimObject& object, AnimDatabase& animDB);

        void update(f32 frame);

        // 回転取得。これだと引数チェックできないのね
        lmath::Quaternion& getRotation(u32 index);

        // 移動取得。これだと引数チェックできないのね
        lmath::Vector3& getTranslation(u32 index);

    private:
        void updateNode(AnimTree& tree, lmath::Matrix43 parent, f32 frame);

        /// 回転と平行移動
        struct State;

        AnimObject *target_;
        const AnimDatabase *animDB_;
        State *currentState_;
    };
#endif
}

#endif //INC_ANIMCONTROLER_H__
