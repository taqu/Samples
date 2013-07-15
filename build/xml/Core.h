#ifndef __XML_CORE_H__
#define __XML_CORE_H__
/**
@file Core.h
@author t-sakai
@date 2009/01/29 create
@data 2009/05/19 lcoreライブラリ用に変更
*/
#include <string>
#include <lcore/lcore.h>

namespace xml
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

#define XML_ASSERT(exp) LASSERT(exp)

#define XML_NEW LIME_NEW
#define XML_DELETE(p) LIME_DELETE(p)
}

#endif //__XML_CORE_H__
