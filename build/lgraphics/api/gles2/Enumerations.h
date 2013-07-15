#ifndef INC_LGRAPHICS_ES2_ENUMERATIONS_H__
#define INC_LGRAPHICS_ES2_ENUMERATIONS_H__
/**
@file Enumerations.h
@author t-sakai
@date 2010/05/29 create
*/
#include "../../lgraphicsAPIInclude.h"

namespace lgraphics
{
#define LIME_FORCE_DWORD (0xFFFFFFFFU)

    enum DisplayFormat
    {
        Display_R5G6B5 =0,
        Display_X8R8G8B8,
        Display_A2R10G10B10,
        DisplayFormat_Num,
    };

    enum PresentInterval
    {
        PresentInterval_Default = 0,
        PresentInterval_One,
        PresentInterval_Two,
        PresentInterval_Three,
        PresentInterval_Four,
        PresentInterval_Immediate = PresentInterval_Default,
    };

    enum ResourceType
    {
        Resource_Surface = 0,
        Resource_Volume,
        Resource_Texture,
        Resource_VolumeTexture,
        Resource_CubeTexture,
        Resource_VertexBuffer,
        Resource_IndexBuffer,

        Resource_FORCEDWORD = LIME_FORCE_DWORD,
    };

    enum Pool
    {
        Pool_Default = GL_STATIC_DRAW,
        Pool_Managed = GL_STATIC_DRAW,
        Pool_SystemMem = GL_DYNAMIC_DRAW,
        Pool_Scratch = GL_DYNAMIC_DRAW,
        Pool_UserMem = GL_DYNAMIC_DRAW,
        Pool_FORCEDWORD = LIME_FORCE_DWORD,
    };

    enum Usage
    {
        Usage_None = 0,
        Usage_AutoGenMipMap,
        Usage_DepthStenceil,
        Usage_Displacement,
        Usage_DoNotClip,
        Usage_Dynamic,
        Usage_RenderTarget,
        Usage_WriteOnly,
    };

    enum Lock
    {
        Lock_None = 0,
        Lock_Discard,
        Lock_NoOverwrite,
    };

    enum ClearTarget
    {
        ClearTarget_Color = GL_COLOR_BUFFER_BIT,
        ClearTarget_Depth = GL_DEPTH_BUFFER_BIT,
        ClearTarget_Stencil = GL_STENCIL_BUFFER_BIT,
    };

    enum DeclType
    {
        DeclType_Float1 =0,
        DeclType_Float2,
        DeclType_Float3,
        DeclType_Float4,
        DeclType_Color,
        DeclType_UB4,
        DeclType_Short2,
        DeclType_Short4,
        DeclType_UB4N,
        DeclType_Short2N,
        DeclType_Short4N,
        DeclType_UShort2N,
        DeclType_UShort4N,
        DeclType_UDec3N,
        DeclType_Dec3N,
        DeclType_Float16_2,
        DeclType_Float16_4,
        DeclType_UnUsed,
        DeclType_Num = DeclType_UnUsed,
    };

    enum DeclUsage
    {
        DeclUsage_Position = 0,
        DeclUsage_BlendWeight,
        DeclUsage_BlendIndicies,
        DeclUsage_Normal,
        DeclUsage_PSize,
        DeclUsage_Texcoord,
        DeclUsage_Tangent,
        DeclUsage_Binormal,
        DeclUsage_TessFactor,
        DeclUsage_PositionT,
        DeclUsage_Color,
        DeclUsage_Fog,
        DeclUsage_Depth,
        DeclUsage_Sample,
        DeclUsage_Num,
    };

    enum DeclMethod
    {
        DeclMethod_Default = 0,
        //DeclMethod_PartialU,
        //DeclMethod_PartialV,
        //DeclMethod_CrossUV,
        //DeclMethod_UV,
        //DeclMethod_LookUp,
        //DeclMethod_LookUpPerSampled,
        DeclMethod_Normalize,
        DeclMethod_Num,
    };

    enum PrimitiveType
    {
        Primitive_PointList = GL_POINTS,
        Primitive_LineList = GL_LINES,
        Primitive_LineStrip = GL_LINE_STRIP,
        Primitive_TriangleList = GL_TRIANGLES,
        Primitive_TriangleStrip = GL_TRIANGLE_STRIP,
        Primitive_TriangleFan = GL_TRIANGLE_FAN,
    };

    enum BufferFormat
    {
        Buffer_Unknown = 0,
        Buffer_D16,
        Buffer_D32,
        Buffer_D24S8,
        Buffer_Index16,
        Buffer_Index32,
        Buffer_Vertex,

        Buffer_DXT1,
        Buffer_DXT2,
        Buffer_DXT3,
        Buffer_DXT4,
        Buffer_DXT5,

        Buffer_A16B16G16R16F,

        Buffer_R32F,
        Buffer_G32R32F,
        Buffer_A32B32G32R32F,

        Buffer_A8,
        Buffer_R8G8B8,
        Buffer_B8G8R8,
        Buffer_A8R8G8B8,
        Buffer_X8R8G8B8,
        Buffer_A8B8G8R8,
        Buffer_X8B8G8R8,
        Buffer_R5G6B5,
        Buffer_B5G6R5,
        Buffer_A4R4G4B4,
        Buffer_A4B4G4R4,
        Buffer_A16B16G16R16,
        Buffer_A2R10G10B10,
        Buffer_A2B10G10R10,
        Buffer_G16R16,
        Buffer_L8,
        Buffer_L16,
        Buffer_ETC1,

        Buffer_FORCEDWORD = LIME_FORCE_DWORD,
    };

    enum MutiSampleType
    {
        MutiSample_None = 0,
        MutiSample_NonMaskable,
        MutiSample_2,
        MutiSample_3,
        MutiSample_4,
        MutiSample_5,
        MutiSample_6,
        MutiSample_7,
        MutiSample_8,
        MutiSample_9,
        MutiSample_10,
        MutiSample_11,
        MutiSample_12,
        MutiSample_13,
        MutiSample_14,
        MutiSample_15,
        MutiSample_16,
        
        MutiSample_FORCEDWORD = LIME_FORCE_DWORD,
    };


    enum TextureType
    {
        TexType_Diffuse =0,
        TexType_Specular,
        TexType_Ambient,
        TexType_Emissive,
        TexType_Height,
        TexType_Normal,
        TexType_Shininess,
        TexType_Opacity,
        TexType_Displacement,
        TexType_LightMap,
        TexType_Reflection,
        TexType_User,
    };

    enum TextureAddress
    {
        TexAddress_Wrap = GL_REPEAT,
        TexAddress_Mirror = GL_MIRRORED_REPEAT,
        TexAddress_Clamp = GL_CLAMP_TO_EDGE,
        TexAddress_Border = GL_CLAMP_TO_EDGE,
        TexAddress_Mirroronce = GL_CLAMP_TO_EDGE,
        TexAddress_None = GL_REPEAT,
    };

    enum TextureFilterType
    {
        TexFilter_None = GL_NEAREST,
        TexFilter_Point = GL_NEAREST,
        TexFilter_Linear = GL_LINEAR,
        TexFilter_Anisotropic = GL_LINEAR,
        TexFilter_PyramidalQuad = GL_LINEAR,
        TexFilter_GaussianQuad = GL_LINEAR,
        TexFilter_ConvolutionMono = GL_LINEAR,

        TexFilter_PointMipMapPoint = GL_NEAREST_MIPMAP_NEAREST,
        TexFilter_LinearMipMapPoint = GL_LINEAR_MIPMAP_NEAREST,
        TexFilter_PointMipMapLinear = GL_NEAREST_MIPMAP_LINEAR,
        TexFilter_LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR,

        TexFilter_Num = 11,
    };    

    enum CullMode
    {
        CullMode_None = 0,
        CullMode_CW = GL_CW,
        CullMode_CCW = GL_CCW,
    };

    enum BlendType
    {
        Blend_Zero = GL_ZERO,
        Blend_One = GL_ONE,
        Blend_SrcAlpha = GL_SRC_ALPHA,
        Blend_InvSrcAlpha = GL_ONE_MINUS_SRC_ALPHA,
        Blend_DestAlpha = GL_DST_ALPHA,
        Blend_InvDestAlpha = GL_ONE_MINUS_DST_ALPHA,
    };

    enum CmpFunc
    {
        Cmp_Never = GL_NEVER,
        Cmp_Less = GL_LESS,
        Cmp_Equal = GL_EQUAL,
        Cmp_LessEqual = GL_LEQUAL,
        Cmp_Greater = GL_GREATER,
        Cmp_NotEqual = GL_NOTEQUAL,
        Cmp_GreaterEqual = GL_GEQUAL,
        Cmp_Always = GL_ALWAYS,
    };

    enum VertexBlend
    {
        VertexBlend_Disable =0,
        VertexBlend_1WEIGHTS,
        VertexBlend_2WEIGHTS,
        VertexBlend_3WEIGHTS,
    };

    enum FaceDirection
    {
        FaceDir_Font = GL_FRONT,
        FaceDir_Back = GL_BACK,
        FaceDir_FrontBack = GL_FRONT_AND_BACK,
    };

    enum Extension
    {
        Ext_ETC1 = LCORE_BIT(0),
        Ext_Num = 1,
    };
}


#endif //INC_LGRAPHICS_ES2_ENUMERATIONS_H__
