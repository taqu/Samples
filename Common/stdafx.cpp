/**
@file stdafx.cpp
@author t-sakai
@date 2011/06/06 create
*/
#include "stdafx.h"
#include <lgraphics/io11/IODDS.h>

namespace fractal
{
    bool loadDDS(lgraphics::Texture2DRef& texture, const Char* filepath)
    {
        lcore::ifstream in(filepath, lcore::ios::binary);
        if(!in.is_open()){
            return false;
        }

        u32 size = in.getSize(0);
        u8* buffer = LIME_NEW u8[size];
        lcore::io::read(in, buffer, size);
        bool ret = lgraphics::io::IODDS::read(texture, buffer, size, lgraphics::Usage_Immutable, lgraphics::TexFilter_MinMagMipLinear, lgraphics::TexAddress_Clamp);
        LIME_DELETE_ARRAY(buffer);
        return ret;
    }
}
