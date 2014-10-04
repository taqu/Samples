#ifndef INC_LOAD_MATERIAL_H__
#define INC_LOAD_MATERIAL_H__
/**
@file load_material.h
@author t-sakai
@date 2012/04/01 create
*/
#include <string>

namespace load
{
    class Material
    {
    public:
        enum Flag
        {
            Flag_CastShadow = 0x01U << 0,
            Flag_RecieveShadow = 0x01U << 1,
            Flag_RefractiveIndex = 0x01U << 2, //shadow_.w_ ‚ªrefractive index‚©
        };

        lmath::Vector4 diffuse_; //rgba
        lmath::Vector4 specular_; //rgb shininess
        lmath::Vector4 shadow_; //shadow, shadow, shadow, refractive index or fresnel

        u32 flags_;

        s16 texColor_;
        s16 texNormal_;
    };
}
#endif //INC_LOAD_MATERIAL_H__
