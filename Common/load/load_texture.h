#ifndef INC_LOAD_TEXTURE_H__
#define INC_LOAD_TEXTURE_H__
/**
@file load_texture.h
@author t-sakai
@date 2012/04/12 create
*/
#include "load.h"

namespace load
{
    class Texture
    {
    public:
        u32 type_;
        Char name_[MaxFileNameSize];
    };
}
#endif //INC_LOAD_TEXTURE_H__
