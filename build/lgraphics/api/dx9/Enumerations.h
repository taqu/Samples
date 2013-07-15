#ifndef INC_LGRAPHICS_DX9_ENUMERATIONS_H__
#define INC_LGRAPHICS_DX9_ENUMERATIONS_H__
/**
@file Enumerations.h
@author t-sakai
@date 2010/02/14 create
*/
#include <d3d9.h>

namespace lgraphics
{
#define LIME_FORCE_DWORD (0xFFFFFFFFU)
#define LIME_FORCE_WORD (0xFFFFU)

    enum VertexTexture
    {
        VertexTexture_Base = D3DVERTEXTEXTURESAMPLER0,
    };

    enum ErrorCode
    {
        Error_None = D3D_OK,
        Error_DeviceLost = D3DERR_DEVICELOST,
    };

    enum PresentInterval
    {
        PresentInterval_Default = D3DPRESENT_INTERVAL_DEFAULT,
        PresentInterval_One = D3DPRESENT_INTERVAL_ONE,
        PresentInterval_Two = D3DPRESENT_INTERVAL_TWO,
        PresentInterval_Three = D3DPRESENT_INTERVAL_THREE,
        PresentInterval_Four = D3DPRESENT_INTERVAL_FOUR,
        PresentInterval_Immediate = D3DPRESENT_INTERVAL_IMMEDIATE,
    };

    enum SwapEffect
    {
        SwapEffect_Discard = D3DSWAPEFFECT_DISCARD,
        SwapEffect_Sequencial = D3DSWAPEFFECT_FLIP,
    };

    enum DriverType
    {
        DriverType_Unknown   = D3DDEVTYPE_REF,
        DriverType_Hardware  = D3DDEVTYPE_HAL,
        DriverType_Reference = D3DDEVTYPE_REF,
        DriverType_Null      = D3DDEVTYPE_NULLREF,
        DriverType_Software  = D3DDEVTYPE_SW,
        DriverType_Warp      = D3DDEVTYPE_SW,
    };

    enum ResourceType
    {
        Resource_Surface = D3DRTYPE_SURFACE,
        Resource_Volume  = D3DRTYPE_VOLUME,
        Resource_Texture = D3DRTYPE_TEXTURE,
        Resource_VolumeTexture = D3DRTYPE_VOLUMETEXTURE,
        Resource_CubeTexture = D3DRTYPE_CUBETEXTURE,
        Resource_VertexBuffer = D3DRTYPE_VERTEXBUFFER,
        Resource_IndexBuffer = D3DRTYPE_INDEXBUFFER,

        Resource_FORCEDWORD = D3DRTYPE_FORCE_DWORD,
    };

    enum StateType
    {
        StateType_All,
        StateType_Pixel,
        StateType_Vertex,
        StateType_Num
    };

    enum Pool
    {
        Pool_Default = D3DPOOL_DEFAULT,
        Pool_Managed = D3DPOOL_MANAGED,
        Pool_SystemMem = D3DPOOL_SYSTEMMEM,
        Pool_Scratch = D3DPOOL_SCRATCH,
        Pool_UserMem,
        Pool_FORCEWORD = LIME_FORCE_WORD,
    };

    enum Usage
    {
        Usage_None = 0,
        Usage_AutoGenMipMap = D3DUSAGE_AUTOGENMIPMAP,
        Usage_DepthStenceil = D3DUSAGE_DEPTHSTENCIL,
        Usage_Displacement = D3DUSAGE_DMAP,
        Usage_DoNotClip = D3DUSAGE_DONOTCLIP,
        Usage_Dynamic = D3DUSAGE_DYNAMIC,
        Usage_RenderTarget = D3DUSAGE_RENDERTARGET,
        Usage_WriteOnly = D3DUSAGE_WRITEONLY,
    };

    enum Lock
    {
        Lock_None = 0,
        Lock_Discard = D3DLOCK_DISCARD,
        Lock_NoOverwrite = D3DLOCK_NOOVERWRITE,
    };

    enum ClearTarget
    {
        ClearTarget_Color = D3DCLEAR_TARGET,
        ClearTarget_Depth = D3DCLEAR_ZBUFFER,
        ClearTarget_Stencil = D3DCLEAR_STENCIL,
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
        DeclMethod_PartialU,
        DeclMethod_PartialV,
        DeclMethod_CrossUV,
        DeclMethod_UV,
        DeclMethod_LookUp,
        DeclMethod_LookUpPerSampled,
        DeclMethod_Normalize = DeclMethod_Default,
    };

    enum PrimitiveType
    {
        Primitive_PointList = D3DPT_POINTLIST,
        Primitive_LineList = D3DPT_LINELIST,
        Primitive_LineStrip = D3DPT_LINESTRIP,
        Primitive_TriangleList = D3DPT_TRIANGLELIST,
        Primitive_TriangleStrip = D3DPT_TRIANGLESTRIP,
        Primitive_TriangleFan = D3DPT_TRIANGLEFAN,
    };

    enum BufferFormat
    {
        Buffer_Unknown = D3DFMT_UNKNOWN,
        Buffer_D32 = D3DFMT_D32,
        Buffer_D24S8 = D3DFMT_D24S8,
        Buffer_D16 = D3DFMT_D16,
        Buffer_Index16 = D3DFMT_INDEX16,
        Buffer_Index32 = D3DFMT_INDEX32,
        Buffer_Vertex = D3DFMT_VERTEXDATA,

        Buffer_DXT1 = D3DFMT_DXT1,
        Buffer_DXT2 = D3DFMT_DXT2,
        Buffer_DXT3 = D3DFMT_DXT3,
        Buffer_DXT4 = D3DFMT_DXT4,
        Buffer_DXT5 = D3DFMT_DXT5,

        Buffer_A16B16G16R16F = D3DFMT_A16B16G16R16F,
        Buffer_R32F = D3DFMT_R32F,
        Buffer_G32R32F = D3DFMT_G32R32F,
        Buffer_A32B32G32R32F = D3DFMT_A32B32G32R32F,

        Buffer_A8 = D3DFMT_A8,
        Buffer_R8G8B8 = D3DFMT_R8G8B8,
        Buffer_A8R8G8B8 = D3DFMT_A8R8G8B8,
        Buffer_X8R8G8B8 = D3DFMT_X8R8G8B8,
        Buffer_A8B8G8R8 = D3DFMT_A8B8G8R8,
        Buffer_X8B8G8R8 = D3DFMT_X8B8G8R8,

        Buffer_R5G6B5 = D3DFMT_R5G6B5,
        Buffer_B5G6R5 = D3DFMT_R5G6B5,
        Buffer_A4R4G4B4 = D3DFMT_A4R4G4B4,
        Buffer_A4B4G4R4 = D3DFMT_A4R4G4B4,

        Buffer_A16B16G16R16 = D3DFMT_A16B16G16R16,
        Buffer_A2R10G10B10 = D3DFMT_A2R10G10B10,
        Buffer_A2B10G10R10 = D3DFMT_A2B10G10R10,
        Buffer_G16R16 = D3DFMT_G16R16,
        Buffer_L8 = D3DFMT_L8,
        Buffer_L16 = D3DFMT_L16,

        Buffer_FORCEDWORD = D3DFMT_FORCE_DWORD,
    };

    enum MutiSampleType
    {
        MutiSample_None = D3DMULTISAMPLE_NONE,
        MutiSample_NonMaskable = D3DMULTISAMPLE_NONMASKABLE,
        MutiSample_2 = D3DMULTISAMPLE_2_SAMPLES,
        MutiSample_3 = D3DMULTISAMPLE_3_SAMPLES,
        MutiSample_4 = D3DMULTISAMPLE_4_SAMPLES,
        MutiSample_5 = D3DMULTISAMPLE_5_SAMPLES,
        MutiSample_6 = D3DMULTISAMPLE_6_SAMPLES,
        MutiSample_7 = D3DMULTISAMPLE_7_SAMPLES,
        MutiSample_8 = D3DMULTISAMPLE_8_SAMPLES,
        MutiSample_9 = D3DMULTISAMPLE_9_SAMPLES,
        MutiSample_10 = D3DMULTISAMPLE_10_SAMPLES,
        MutiSample_11 = D3DMULTISAMPLE_11_SAMPLES,
        MutiSample_12 = D3DMULTISAMPLE_12_SAMPLES,
        MutiSample_13 = D3DMULTISAMPLE_13_SAMPLES,
        MutiSample_14 = D3DMULTISAMPLE_14_SAMPLES,
        MutiSample_15 = D3DMULTISAMPLE_15_SAMPLES,
        MutiSample_16 = D3DMULTISAMPLE_16_SAMPLES,
        
        MutiSample_FORCEDWORD = D3DMULTISAMPLE_FORCE_DWORD,
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
        TexAddress_Wrap = D3DTADDRESS_WRAP,
        TexAddress_Mirror = D3DTADDRESS_MIRROR,
        TexAddress_Clamp = D3DTADDRESS_CLAMP,
        TexAddress_Border = D3DTADDRESS_BORDER,
        TexAddress_Mirroronce = D3DTADDRESS_MIRRORONCE,
        TexAddress_None,
    };

    enum TextureFilterType
    {
        TexFilter_None = D3DTEXF_NONE,
        TexFilter_Point = D3DTEXF_POINT,
        TexFilter_Linear = D3DTEXF_LINEAR,
        TexFilter_Anisotropic = D3DTEXF_ANISOTROPIC,
        TexFilter_PyramidalQuad = D3DTEXF_PYRAMIDALQUAD,
        TexFilter_GaussianQuad = D3DTEXF_GAUSSIANQUAD,
        TexFilter_ConvolutionMono = D3DTEXF_CONVOLUTIONMONO,

        TexFilter_PointMipMapPoint = D3DTEXF_LINEAR,
        TexFilter_LinearMipMapPoint = D3DTEXF_LINEAR,
        TexFilter_PointMipMapLinear = D3DTEXF_LINEAR,
        TexFilter_LinearMipMapLinear = D3DTEXF_LINEAR,

        TexFilter_Num = 11,
    };

    enum FillMode
    {
        FillMode_Point = D3DFILL_POINT,
        FillMode_Wireframe = D3DFILL_WIREFRAME,
        FillMode_Solid = D3DFILL_SOLID,
    };

    enum CullMode
    {
        CullMode_None = D3DCULL_NONE,
        CullMode_CW = D3DCULL_CW,
        CullMode_CCW = D3DCULL_CCW,
    };

    enum BlendType
    {
        Blend_Zero = D3DBLEND_ZERO,
        Blend_One = D3DBLEND_ONE,
        Blend_SrcAlpha = D3DBLEND_SRCALPHA,
        Blend_InvSrcAlpha = D3DBLEND_INVSRCALPHA,
        Blend_DestAlpha = D3DBLEND_DESTALPHA,
        Blend_InvDestAlpha = D3DBLEND_INVDESTALPHA,
    };

    enum CmpFunc
    {
        Cmp_Never = D3DCMP_NEVER,
        Cmp_Less = D3DCMP_LESS,
        Cmp_Equal = D3DCMP_EQUAL,
        Cmp_LessEqual = D3DCMP_LESSEQUAL,
        Cmp_Greater = D3DCMP_GREATER,
        Cmp_NotEqual = D3DCMP_NOTEQUAL,
        Cmp_GreaterEqual = D3DCMP_GREATEREQUAL,
        Cmp_Always = D3DCMP_ALWAYS,
    };

    enum StencilOp
    {
        StencilOp_Keep = D3DSTENCILOP_KEEP,
        StencilOp_Zero = D3DSTENCILOP_ZERO,
        StencilOp_Replace = D3DSTENCILOP_REPLACE,
        StencilOp_IncrSat = D3DSTENCILOP_INCRSAT,
        StencilOp_DecrSat = D3DSTENCILOP_DECRSAT,
        StencilOp_Invert = D3DSTENCILOP_INVERT,
        StencilOp_Incr = D3DSTENCILOP_INCR,
        StencilOp_Decr = D3DSTENCILOP_DECR,
    };

    enum VertexBlend
    {
        VertexBlend_Disable =D3DVBF_DISABLE,
        VertexBlend_1WEIGHTS =D3DVBF_1WEIGHTS,
        VertexBlend_2WEIGHTS =D3DVBF_2WEIGHTS,
        VertexBlend_3WEIGHTS =D3DVBF_3WEIGHTS,
    };


    enum TransformType
    {
        Transform_View =D3DTS_VIEW,
        Transform_Projection =D3DTS_PROJECTION,
        Transform_Texture0 =D3DTS_TEXTURE0,
        Transform_Texture1 =D3DTS_TEXTURE1,
        Transform_Texture2 =D3DTS_TEXTURE2,
        Transform_Texture3 =D3DTS_TEXTURE3,
        Transform_Texture4 =D3DTS_TEXTURE4,
        Transform_Texture5 =D3DTS_TEXTURE5,
        Transform_Texture6 =D3DTS_TEXTURE6,
        Transform_Texture7 =D3DTS_TEXTURE7,
        Transform_FORCEDWORD = D3DTS_FORCE_DWORD,
    };

#define Transform_WorldMatrix(index) (TransformType)(index + 256)

    enum VertexShaderSampler
    {
        VSSampler0 = D3DVERTEXTEXTURESAMPLER0,
        VSSampler1 = D3DVERTEXTEXTURESAMPLER1,
        VSSampler2 = D3DVERTEXTEXTURESAMPLER2,
        VSSampler3 = D3DVERTEXTEXTURESAMPLER3,
        VSSampler_Num = 4,
    };
}


#endif //INC_LGRAPHICS_DX9_ENUMERATIONS_H__
