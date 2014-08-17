#ifndef INC_LANIM_H__
#define INC_LANIM_H__
/**
@file lanim.h
@author t-sakai
@date 2010/11/12 create

*/
#include <lcore/lcore.h>
#include <lcore/String.h>
#include <lgraphics/lgraphicscore.h>

namespace lcore
{
    class ostream;
    class istream;
}

namespace lanim
{
    using lcore::Char;
    using lcore::s8;
    using lcore::s16;
    using lcore::s32;
    using lcore::u8;
    using lcore::u16;
    using lcore::u32;
    using lcore::f32;
    using lcore::f64;

    typedef const char* HANDLE;

#if LIME_USE_DUAL_QUATERNION_FOR_SKINNING
    static const u8 LANIM_MAX_SKINNING_MATRICES = (100-1);
#else
    static const u8 LANIM_MAX_SKINNING_MATRICES = lgraphics::LIME_MAX_SKINNING_MATRICES;
#endif

    static const u32 LANIM_MAX_PALLET_MATRICES = LANIM_MAX_SKINNING_MATRICES;

    static const u32 LANIM_MAX_NAME_SIZE = 24;

    static const u32 LANIM_MAX_ANIM_NODE = 512;


    typedef lcore::StringWithHash<LANIM_MAX_NAME_SIZE> Name;

#define LANIM_FRAME_DURATION_MIN (1.192092896e-04F)

    static const u8 InvalidJointIndex = 0xFFU;
    static const u8 MaxJoints = 0xFEU;

    enum JointType
    {
        JointType_Rotation =0, //回転
        JointType_RotTrans,    //回転・移動
        JointType_IK,          //IK
        JointType_UnderIK,    //IK影響下
        JointType_UnderRot,    //回転影響下
        JointType_ToIK,        //IK接続先
        JointType_Hide,        //非表示
        JointType_Twist,       //捻り
        JointType_FreeRot,     //回転運動。捻り影響下。先端からの捻りの影響を受ける

        JointType_End, //ジョイント連結の終端
        JointType_Num,
    };

    enum JointFlag
    {
        JointFlag_IKLimitHeading = (0x01U << 0), //曲げる軸を制限する
        JointFlag_Num = 1,
    };

    void write(lcore::ostream& os, const Name& name);
    void read(Name& name, lcore::istream& is);
}

#endif //INC_LANIM_H__
