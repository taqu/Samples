/**
@file ShaderType.cpp
@author t-sakai
@date 2011/10/16 create
*/
#include "ShaderType.h"

#include <lgraphics/api/ShaderRef.h>

namespace render
{

namespace
{
    //-----------------------------------------------------------------------------
    //---
    //--- シェーダ
    //---
    //-----------------------------------------------------------------------------

    //Normal
    //------------------------------------
    const u8 DefaultVSSource[] =
    {
#include "shader/DefaultVS.byte"
    };
    const u32 DefaultVSSourceSize = sizeof(DefaultVSSource);

    const u8 DefaultPSSource[] =
    {
#include "shader/DefaultPS.byte"
    };
    const u32 DefaultPSSourceSize = sizeof(DefaultPSSource);

    const u8 DefaultTexturePSSource[] =
    {
#include "shader/DefaultTexturePS.byte"
    };
    const u32 DefaultTexturePSSourceSize = sizeof(DefaultTexturePSSource);

    //デプスバッファ作成
    //------------------------------------
    const u8 DepthVSSource[] =
    {
#include "shader/DepthVS.byte"
    };
    const u32 DepthVSSourceSize = sizeof(DepthVSSource);

    const u8 DepthPSSource[] =
    {
#include "shader/DepthPS.byte"
    };
    const u32 DepthPSSourceSize = sizeof(DepthPSSource);


    //テクスチャでフィル
    //------------------------------------
    const u8 RectangleVSSource[] =
    {
#include "shader/RectangleVS.byte"
    };
    const u32 RectangleVSSourceSize = sizeof(RectangleVSSource);

    const u8 RectanglePSSource[] =
    {
#include "shader/RectanglePS.byte"
    };
    const u32 RectanglePSSourceSize = sizeof(RectanglePSSource);


    //DebugVC
    //------------------------------------
    const u8 DebugVCVSSource[] =
    {
#include "shader/DebugVCVS.byte"
    };
    const u32 DebugVCVSSourceSize = sizeof(DebugVCVSSource);

    const u8 DebugVCPSSource[] =
    {
#include "shader/DebugVCPS.byte"
    };
    const u32 DebugVCPSSourceSize = sizeof(DebugVCPSSource);

    //TonemappingStatic
    //------------------------------------
    const u8 TonemappingStaticPSSource[] =
    {
#include "shader/TonemappingStaticPS.byte"
    };
    const u32 TonemappingStaticPSSourceSize = sizeof(TonemappingStaticPSSource);


    //Voxelize
    //------------------------------------
    const u8 VoxelizeVSSource[] =
    {
#include "shader/VoxelizeVS.byte"
    };
    const u32 VoxelizeVSSourceSize = sizeof(VoxelizeVSSource);

    const u8 VoxelizeGSSource[] =
    {
#include "shader/VoxelizeGS.byte"
    };
    const u32 VoxelizeGSSourceSize = sizeof(VoxelizeGSSource);

    const u8 VoxelizePSSource[] =
    {
#include "shader/VoxelizePS.byte"
    };
    const u32 VoxelizePSSourceSize = sizeof(VoxelizePSSource);

    const u8 VoxelizeTexturePSSource[] =
    {
#include "shader/VoxelizeTexturePS.byte"
    };
    const u32 VoxelizeTexturePSSourceSize = sizeof(VoxelizeTexturePSSource);


    //VoxelRender
    //------------------------------------
    const u8 VoxelRenderVSSource[] =
    {
#include "shader/VoxelRenderVS.byte"
    };
    const u32 VoxelRenderVSSourceSize = sizeof(VoxelRenderVSSource);

    const u8 VoxelRenderPSSource[] =
    {
#include "shader/VoxelRenderPS.byte"
    };
    const u32 VoxelRenderPSSourceSize = sizeof(VoxelRenderPSSource);

    //OctreeTag
    //------------------------------------
    const u8 OctreeTagVSSource[] =
    {
#include "shader/OctreeTagVS.byte"
    };
    const u32 OctreeTagVSSourceSize = sizeof(OctreeTagVSSource);

    const u8 OctreeTagPSSource[] =
    {
#include "shader/OctreeTagPS.byte"
    };
    const u32 OctreeTagPSSourceSize = sizeof(OctreeTagPSSource);


    //OctreeCreateNode
    //------------------------------------
    const u8 OctreeCreateNodePSSource[] =
    {
#include "shader/OctreeCreateNodePS.byte"
    };
    const u32 OctreeCreateNodePSSourceSize = sizeof(OctreeCreateNodePSSource);

    //OctreeInitNode
    //------------------------------------
    const u8 OctreeInitNodePSSource[] =
    {
#include "shader/OctreeInitNodePS.byte"
    };
    const u32 OctreeInitNodePSSourceSize = sizeof(OctreeInitNodePSSource);

    //OctreeWriteValue
    //------------------------------------
    const u8 OctreeWriteValuePSSource[] =
    {
#include "shader/OctreeWriteValuePS.byte"
    };
    const u32 OctreeWriteValuePSSourceSize = sizeof(OctreeWriteValuePSSource);

    //OctreeWriteValue
    //------------------------------------
    const u8 OctreePropagatePSSource[] =
    {
#include "shader/OctreePropagatePS.byte"
    };
    const u32 OctreePropagatePSSourceSize = sizeof(OctreePropagatePSSource);

    //OctreeMipmap
    //------------------------------------
    const u8 OctreeMipmapPSSource[] =
    {
#include "shader/OctreeMipmapPS.byte"
    };
    const u32 OctreeMipmapPSSourceSize = sizeof(OctreeMipmapPSSource);


    //DefaultAO
    //------------------------------------
    const u8 DefaultAOPSSource[] =
    {
#include "shader/DefaultAOPS.byte"
    };
    const u32 DefaultAOPSSourceSize = sizeof(DefaultAOPSSource);

    //DefaultAOTexture
    //------------------------------------
    const u8 DefaultAOTexturePSSource[] =
    {
#include "shader/DefaultAOTexturePS.byte"
    };
    const u32 DefaultAOTexturePSSourceSize = sizeof(DefaultAOTexturePSSource);

    //-----------------------------------------------------------------------------
    struct Entry
    {
        const u8* vssrc_;
        u32 vssize_;
        const u8* pssrc_;
        u32 pssize_;
    };

    const Entry ShaderEntries[Shader_Num]=
    {
        { DefaultVSSource, DefaultVSSourceSize, DefaultPSSource, DefaultPSSourceSize },
        { NULL, 0, DefaultTexturePSSource, DefaultTexturePSSourceSize },
        { DepthVSSource, DepthVSSourceSize, DepthPSSource, DepthPSSourceSize },
        { RectangleVSSource, RectangleVSSourceSize, RectanglePSSource, RectanglePSSourceSize },
        { DebugVCVSSource, DebugVCVSSourceSize, DebugVCPSSource, DebugVCPSSourceSize },
        { NULL, 0, TonemappingStaticPSSource, TonemappingStaticPSSourceSize },
        { VoxelizeVSSource, VoxelizeVSSourceSize, VoxelizePSSource, VoxelizePSSourceSize },
        { NULL, 0, VoxelizeTexturePSSource, VoxelizeTexturePSSourceSize },
        { VoxelRenderVSSource, VoxelRenderVSSourceSize, VoxelRenderPSSource, VoxelRenderPSSourceSize },
        { OctreeTagVSSource, OctreeTagVSSourceSize, OctreeTagPSSource, OctreeTagPSSourceSize },
        { NULL, 0, OctreeCreateNodePSSource, OctreeCreateNodePSSourceSize },
        { NULL, 0, OctreeInitNodePSSource, OctreeInitNodePSSourceSize },
        { NULL, 0, OctreeWriteValuePSSource, OctreeWriteValuePSSourceSize },
        { NULL, 0, OctreePropagatePSSource, OctreePropagatePSSourceSize },
        { NULL, 0, OctreeMipmapPSSource, OctreeMipmapPSSourceSize },

        { NULL, 0, DefaultAOPSSource, DefaultAOPSSourceSize },
        { NULL, 0, DefaultAOTexturePSSource, DefaultAOTexturePSSourceSize },
    };


    //-----------------------------------------------------------------------------
    struct EntryGS
    {
        const u8* gssrc_;
        u32 gssize_;
    };

    const EntryGS ShaderGSEntries[ShaderGS_Num]=
    {
        { VoxelizeGSSource, VoxelizeGSSourceSize },
    };
}

    Compiler::Compiler()
        :size_(DefaultSize)
    {
        buffer_ = LIME_NEW u8[DefaultSize];
    }

    Compiler::~Compiler()
    {
        LIME_DELETE_ARRAY(buffer_);
    }

    void Compiler::resize(u32 size)
    {
        if(size_<size){
            size_ = size;
            LIME_DELETE_ARRAY(buffer_);
            buffer_ = LIME_NEW u8[size_];
        }
    }

    u32 Compiler::decompress(u32 size, const u8* data)
    {
        lgraphics::ShaderDecompresser decompresser(size, data);
        u32 decompSize = decompresser.getDecompressedSize();
        resize(decompSize);
        return decompresser.decompress(buffer_);
    }

    // シェーダコンパイル
    void Compiler::compileShaderVS(lgraphics::VertexShaderRef& vs, Shader id)
    {
        LASSERT(0<=id && id<Shader_Num);

        u32 size = decompress(ShaderEntries[id].vssize_, ShaderEntries[id].vssrc_);
        if(0<size){
            vs = lgraphics::Shader::createVertexShaderFromBinary(buffer_, size);
        }
    }

    // シェーダコンパイル
    void Compiler::compileShaderGS(lgraphics::GeometryShaderRef& gs, ShaderGS id)
    {
        LASSERT(0<=id && id<ShaderGS_Num);
        u32 size = decompress(ShaderGSEntries[id].gssize_, ShaderGSEntries[id].gssrc_);
        if(0<size){
            gs = lgraphics::Shader::createGeometryShaderFromBinary(buffer_, size);
        }
    }

    // シェーダコンパイル
    void Compiler::compileShaderPS(lgraphics::PixelShaderRef& ps, Shader id)
    {
        LASSERT(0<=id && id<Shader_Num);

        u32 size = decompress(ShaderEntries[id].pssize_, ShaderEntries[id].pssrc_);
        if(0<size){
            ps = lgraphics::Shader::createPixelShaderFromBinary(buffer_, size);
        }
    }
}
