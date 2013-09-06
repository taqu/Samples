#ifndef INC_FONTTYPE_H__
#define INC_FONTTYPE_H__
/**
@file FontType.h
@author t-sakai
@date 2011/09/24 create
*/
#include <lcore/lcore.h>

namespace font
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

    static const u32 MaxCharsInFont = 256;
}
#endif //INC_FONTTYPE_H__
