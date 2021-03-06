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
#include "render/ShaderType.h"
#include "load/load.h"

namespace render
{
    class Material
    {
    public:
        enum Flag
        {
            Flag_CastShadow = 0x01U << 0,
            Flag_RecieveShadow = 0x01U << 1,
            Flag_RefractiveIndex = 0x01U << 2, //shadow_.w_ がrefractive indexか
        };

        enum Texture
        {
            Tex_Albedo = load::TexType_Albedo,
            //Tex_Normal = load::TexType_Normal,
            Tex_Num = load::TexType_Num,
        };

        Material();
        ~Material();

        /// スワップ
        void swap(Material& rhs);

        //s32 shadingModel_;
        //s32 blendFunc_;

        u32 flags_;
        lmath::Vector4 diffuse_; //rgba
        lmath::Vector4 specular_; //rgb shininess
        lmath::Vector4 shadow_; //shadow, shadow, shadow, refractive index or fresnel

        s16 textureIDs_[Tex_Num];
        lgraphics::Texture2DRef* textures_[Tex_Num];
    };
}
#endif //INC_RENDER_MATERIAL_H__
