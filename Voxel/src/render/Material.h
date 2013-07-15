#ifndef INC_RENDER_MATERIAL_H__
#define INC_RENDER_MATERIAL_H__
/**
@file Material.h
@author t-sakai
@date 2011/10/17 create
*/
#include <lmath/Vector3.h>
#include <lmath/Vector4.h>

#include <lgraphics/api/TextureRef.h>
#include <lgraphics/io11/IODDS.h>

#include "render.h"
#include "ShaderType.h"
#include "load.h"

namespace render
{
    class Material
    {
    public:
        enum Flag
        {
            Flag_CastShadow = 0x01U << 0,
            Flag_RecieveShadow = 0x01U << 1,
        };

        enum Texture
        {
            Tex_Albedo = load::TexType_Albedo,
            //Tex_Normal = load::TexType_Normal,
            Tex_Num = load::TexType_Num,
        };

        Material();
        ~Material();

        /// ƒXƒƒbƒv
        void swap(Material& rhs);

        //s32 shadingModel_;
        //s32 blendFunc_;

        u32 flags_;
        lmath::Vector4 diffuse_; //rgba
        lmath::Vector4 specular_; //rgb shininess
        //lmath::Vector4 ambient_; //rgb
        //lmath::Vector4 emissive_; //rgb
        lmath::Vector4 transparent_; //rgb fresnel

        s16 textureIDs_[Tex_Num];
        lgraphics::Texture2DRef* textures_[Tex_Num];
    };
}
#endif //INC_RENDER_MATERIAL_H__
