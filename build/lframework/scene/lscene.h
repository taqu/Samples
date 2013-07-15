#ifndef INC_LSCENE_H__
#define INC_LSCENE_H__
/**
@file lscene.h
@author t-sakai
@date 2010/11/21 create
*/
#include <lcore/lcore.h>
#include <lcore/String.h>

namespace lscene
{
    using lcore::s8;
    using lcore::s16;
    using lcore::s32;
    using lcore::u8;
    using lcore::u16;
    using lcore::u32;
    using lcore::f32;
    using lcore::f64;

    static const u32 MAX_NAME_BUFFER_SIZE = 48;
    static const u8 MAX_BONES = 255;
    typedef lcore::String<MAX_NAME_BUFFER_SIZE> NameString;
}
#endif //INC_LSCENE_H__
