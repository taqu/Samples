#ifndef INC_LOAD_LOAD_H__
#define INC_LOAD_LOAD_H__
/**
@file load.h
@author t-sakai
@date 2012/03/12 create
*/
#include <lcore/lcore.h>

namespace load
{
    using lcore::s8;
    using lcore::s16;
    using lcore::s32;

    using lcore::u8;
    using lcore::u16;
    using lcore::u32;
    using lcore::u64;

    using lcore::f32;
    using lcore::f64;

    using lcore::Char;

    static const u32 MaxVertices = 0xFFFFU;
    static const u8 MaxNodes = 0xFEU;
    static const u8 InvalidNode = 0xFFU;
    static const u32 MaxNameLength = 31;
    static const u32 MaxNameSize = MaxNameLength + 1;

    static const u32 LOAD_TRUE = 1;
    static const u32 LOAD_FALSE = 0;

    enum Elem
    {
        Elem_Geometry =0,
        Elem_Material,
        Elem_Mesh,
        Elem_Node,
        Elem_Texture,
        Elem_Num,
    };

    /// 頂点属性
    enum VElem
    {
        VElem_Position = (0x01U << 0),
        VElem_Normal   = (0x01U << 1),
        VElem_Tangent  = (0x01U << 2),
        VElem_Binormal = (0x01U << 3),
        VElem_Color    = (0x01U << 4),
        VElem_Texcoord = (0x01U << 5),
        VElem_Bone     = (0x01U<<6),
        VElem_BoneWeight = (0x01U<<7),
        VElem_Num = 8,
    };

    enum TexType
    {
        TexType_Albedo =0,
        TexType_Normal,
        TexType_Num,
    };

    enum RotationOrder
    {
        RotationOrder_EulerXYZ = 0,
        RotationOrder_EulerXZY,
        RotationOrder_EulerYZX,
        RotationOrder_EulerYXZ,
        RotationOrder_EulerZXY,
        RotationOrder_EulerZYX,
        RotationOrder_SphericXYZ,
    };

    //-------------------------------------
    //---
    //--- Element
    //---
    //-------------------------------------
    struct Element
    {
        u32 number_; /// 
        u32 offset_; /// ファイル先頭からのオフセット
    };

    //-------------------------------------
    //---
    //--- Header
    //---
    //-------------------------------------
    struct Header
    {
        u16 major_;
        u16 minor_;
        Element elems_[Elem_Num];
    };
}
#endif //INC_LOAD_LOAD_H__
