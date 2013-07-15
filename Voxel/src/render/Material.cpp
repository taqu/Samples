/**
@file Material.cpp
@author t-sakai
@date 2011/10/17 create
*/
#include "Material.h"

namespace render
{
    Material::Material()
        :flags_(0)
    {
        for(u32 i=0; i<Tex_Num; ++i){
            textureIDs_[i] = -1;
            textures_[i] = NULL;
        }
    }

    Material::~Material()
    {
    }

    void Material::swap(Material& rhs)
    {
        lcore::swap(flags_, rhs.flags_);
        diffuse_.swap(rhs.diffuse_);
        specular_.swap(rhs.specular_);

        for(u32 i=0; i<Tex_Num; ++i){
            lcore::swap(textureIDs_[i], rhs.textureIDs_[i]);
            lcore::swap(textures_[i], rhs.textures_[i]);
        }
    }
}
