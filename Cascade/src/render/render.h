#ifndef INC_RENDER_RENDER_H__
#define INC_RENDER_RENDER_H__
/**
@file render.h
@author t-sakai
@date 2011/10/16 create
*/
#include <lcore/lcore.h>

#include <lgraphics/lgraphics.h>

namespace render
{
    using lcore::s8;
    using lcore::s16;
    using lcore::s32;

    using lcore::u8;
    using lcore::u16;
    using lcore::u32;

    using lcore::f32;
    using lcore::f64;

    using lcore::Char;

    static const u32 MaxVertices = 0xFFFFU;
    static const u32 MaxNameLength = 31;
    static const u32 MaxNameSize = MaxNameLength + 1;

    static const u8 MaxNodes = 0xFEU;
    static const u8 InvalidNodeIndex = 0xFFU;

    typedef u8 MeshIndexType;

    static const u32 LTRUE = 1;
    static const u32 LFALSE = 0;

//#define RENDER_ENABLE_INDIPENDENT_ALPHA
}
#endif //INC_RENDER_RENDER_H__
