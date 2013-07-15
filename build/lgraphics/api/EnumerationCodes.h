#ifndef INC_ENUMERATIONCODES_H__
#define INC_ENUMERATIONCODES_H__
/**
@file EnumerationCodes.h
@author t-sakai
@date 2010/06/15 create

*/
#include "Enumerations.h"

namespace lgraphics
{
    //-------------------------------------------------------
    //---
    //--- Pool
    //---
    //-------------------------------------------------------
    enum CodePool
    {
        CodePool_Default = 0,
        CodePool_Managed,
        CodePool_SystemMem,
        CodePool_Scratch,
        CodePool_UserMem,
        CodePool_FORCEDWORD = LIME_FORCE_DWORD,
    };

    CodePool encodePool(Pool type);
    Pool decodePool(CodePool type);


    //-------------------------------------------------------
    //---
    //--- Resource
    //---
    //-------------------------------------------------------
    enum CodeResourceType
    {
        CodeResource_Surface = 0,
        CodeResource_Volume,
        CodeResource_Texture,
        CodeResource_VolumeTexture,
        CodeResource_CubeTexture,
        CodeResource_VertexBuffer,
        CodeResource_IndexBuffer,

        CodeResource_FORCEDWORD = LIME_FORCE_DWORD,
    };

    CodeResourceType encodeResource(ResourceType type);
    ResourceType decodeResource(CodeResourceType type);

    //-------------------------------------------------------
    //---
    //--- Usage
    //---
    //-------------------------------------------------------
    enum CodeUsage
    {
        CodeUsage_None = 0,
        CodeUsage_AutoGenMipMap,
        CodeUsage_DepthStenceil,
        CodeUsage_Displacement,
        CodeUsage_DoNotClip,
        CodeUsage_Dynamic,
        CodeUsage_RenderTarget,
    };

    CodeUsage encodeUsage(Usage type);
    Usage decodeUsage(CodeUsage type);

    //-------------------------------------------------------
    //---
    //--- Lock
    //---
    //-------------------------------------------------------
    enum CodeLock
    {
        CodeLock_None = 0,
        CodeLock_Discard,
        CodeLock_NoOverwrite,
    };

    CodeLock encodeLock(Lock type);
    Lock decodeLock(CodeLock type);

    //-------------------------------------------------------
    //---
    //--- DeclType
    //---
    //-------------------------------------------------------
    enum CodeDeclType
    {
        CodeDeclType_Float1 =0,
        CodeDeclType_Float2,
        CodeDeclType_Float3,
        CodeDeclType_Float4,
        CodeDeclType_Color,
        CodeDeclType_UB4,
        CodeDeclType_Short2,
        CodeDeclType_Short4,
        CodeDeclType_UB4N,
        CodeDeclType_Short2N,
        CodeDeclType_Short4N,
        CodeDeclType_UShort2N,
        CodeDeclType_UShort4N,
        CodeDeclType_UDec3N,
        CodeDeclType_Dec3N,
        CodeDeclType_Float16_2,
        CodeDeclType_Float16_4,
        CodeDeclType_UnUsed,
        CodeDeclType_Num = DeclType_UnUsed,
    };

    CodeDeclType encodeDeclType(DeclType type);
    DeclType decodeDeclType(CodeDeclType type);

    //-------------------------------------------------------
    //---
    //--- DeclUsage
    //---
    //-------------------------------------------------------
    enum CodeDeclUsage
    {
        CodeDeclUsage_Position = 0,
        CodeDeclUsage_BlendWeight,
        CodeDeclUsage_BlendIndicies,
        CodeDeclUsage_Normal,
        CodeDeclUsage_PSize,
        CodeDeclUsage_Texcoord,
        CodeDeclUsage_Tangent,
        CodeDeclUsage_Binormal,
        CodeDeclUsage_TessFactor,
        CodeDeclUsage_PositionT,
        CodeDeclUsage_Color,
        CodeDeclUsage_Fog,
        CodeDeclUsage_Depth,
        CodeDeclUsage_Sample,
        CodeDeclUsage_Num,
    };

    CodeDeclUsage encodeDeclUsage(DeclUsage type);
    DeclUsage decodeDeclUsage(CodeDeclUsage type);

    //-------------------------------------------------------
    //---
    //--- PrimitiveType
    //---
    //-------------------------------------------------------
    enum CodePrimitiveType
    {
        CodePrimitive_PointList = 0,
        CodePrimitive_LineList,
        CodePrimitive_LineStrip,
        CodePrimitive_TriangleList,
        CodePrimitive_TriangleStrip,
        CodePrimitive_TriangleFan,
    };

    CodePrimitiveType encodePrimitive(PrimitiveType type);
    PrimitiveType decodePrimitive(CodePrimitiveType type);

    //-------------------------------------------------------
    //---
    //--- BufferFormat
    //---
    //-------------------------------------------------------
    enum CodeBufferFormat
    {
        CodeBuffer_Unknown = 0,
        CodeBuffer_D32,
        CodeBuffer_D24S8,
        CodeBuffer_D16,
        CodeBuffer_Index16,
        CodeBuffer_Index32,
        CodeBuffer_Vertex,

        CodeBuffer_DXT1,
        CodeBuffer_DXT2,
        CodeBuffer_DXT3,
        CodeBuffer_DXT4,
        CodeBuffer_DXT5,

        CodeBuffer_A16B16G16R16F,

        CodeBuffer_R32F,
        CodeBuffer_G32R32F,
        CodeBuffer_A32B32G32R32F,

        CodeBuffer_A8,
        CodeBuffer_R8G8B8,
        CodeBuffer_B8G8R8,
        CodeBuffer_A8R8G8B8,
        CodeBuffer_X8R8G8B8,
        CodeBuffer_A8B8G8R8,
        CodeBuffer_X8B8G8R8,
        CodeBuffer_A16B16G16R16,
        CodeBuffer_A2R10G10B10,
        CodeBuffer_A2B10G10R10,
        CodeBuffer_G16R16,
        CodeBuffer_L8,
        CodeBuffer_L16,

        CodeBuffer_FORCEDWORD = LIME_FORCE_DWORD,
    };

    CodeBufferFormat encodeBufferFormat(BufferFormat type);
    BufferFormat decodeBufferFormat(CodeBufferFormat type);

    //-------------------------------------------------------
    //---
    //--- MutiSampleType
    //---
    //-------------------------------------------------------
    enum CodeMutiSampleType
    {
        CodeMutiSample_None = 0,
        CodeMutiSample_NonMaskable,
        CodeMutiSample_2,
        CodeMutiSample_3,
        CodeMutiSample_4,
        CodeMutiSample_5,
        CodeMutiSample_6,
        CodeMutiSample_7,
        CodeMutiSample_8,
        CodeMutiSample_9,
        CodeMutiSample_10,
        CodeMutiSample_11,
        CodeMutiSample_12,
        CodeMutiSample_13,
        CodeMutiSample_14,
        CodeMutiSample_15,
        CodeMutiSample_16,
        
        CodeMutiSample_FORCEDWORD = LIME_FORCE_DWORD,
    };

    CodeMutiSampleType encodeMutiSample(MutiSampleType type);
    MutiSampleType decodeMutiSample(CodeMutiSampleType type);

    //-------------------------------------------------------
    //---
    //--- TextureAddress
    //---
    //-------------------------------------------------------
    enum CodeTextureAddress
    {
        CodeTexAddress_Wrap = 0,
        CodeTexAddress_Mirror,
        CodeTexAddress_Clamp,
        CodeTexAddress_Border,
        CodeTexAddress_Mirroronce,
        CodeTexAddress_None,
    };

    CodeTextureAddress encodeTextureAddress(TextureAddress type);
    TextureAddress decodeTextureAddress(CodeTextureAddress type);

    //-------------------------------------------------------
    //---
    //--- TextureFilterType
    //---
    //-------------------------------------------------------
    enum CodeTextureFilterType
    {
        CodeTexFilter_None = 0,
        CodeTexFilter_Point,
        CodeTexFilter_Linear,
        CodeTexFilter_Anisotropic,
        CodeTexFilter_PyramidalQuad,
        CodeTexFilter_GaussianQuad,
        CodeTexFilter_ConvolutionMono,
        CodeTexFilter_Num,
    };

    CodeTextureFilterType encodeTextureFilter(TextureFilterType type);
    TextureFilterType decodeTextureFilter(CodeTextureFilterType type);

    //-------------------------------------------------------
    //---
    //--- CullMode
    //---
    //-------------------------------------------------------
    enum CodeCullMode
    {
        CodeCullMode_None = 0,
        CodeCullMode_CW,
        CodeCullMode_CCW,
    };

    CodeCullMode encodeCullMode(CullMode type);
    CullMode decodeCullMode(CodeCullMode type);

    //-------------------------------------------------------
    //---
    //--- BlendType
    //---
    //-------------------------------------------------------
    enum CodeBlendType
    {
        CodeBlend_Zero = 0,
        CodeBlend_One,
        CodeBlend_SrcAlpha,
        CodeBlend_InvSrcAlpha,
        CodeBlend_DestAlpha,
        CodeBlend_InvDestAlpha,
    };

    CodeBlendType encodeBlendType(BlendType type);
    BlendType decodeBlendType(CodeBlendType type);

    //-------------------------------------------------------
    //---
    //--- CmpFunc
    //---
    //-------------------------------------------------------
    enum CodeCmpFunc
    {
        CodeCmp_Never = 0,
        CodeCmp_Less,
        CodeCmp_Equal,
        CodeCmp_LessEqual,
        CodeCmp_Greater,
        CodeCmp_NotEqual,
        CodeCmp_GreaterEqual,
        CodeCmp_Always,
    };

    CodeCmpFunc encodeCmpFunc(CmpFunc type);
    CmpFunc decodeCmpFunc(CodeCmpFunc type);


    //-------------------------------------------------------
    //---
    //--- VertexBlend
    //---
    //-------------------------------------------------------
    enum CodeVertexBlend
    {
        CodeVertexBlend_Disable = 0,
        CodeVertexBlend_1WEIGHTS,
        CodeVertexBlend_2WEIGHTS,
        CodeVertexBlend_3WEIGHTS,
    };

    CodeVertexBlend encodeVertexBlend(VertexBlend type);
    VertexBlend decodeVertexBlend(CodeVertexBlend type);
}
#endif //INC_ENUMERATIONCODES_H__
