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
    const u8 DefaultPNVSSource[] =
    {
#include "shader/DefaultPNVS.byte"
    };
    const u32 DefaultPNVSSourceSize = sizeof(DefaultPNVSSource);

    const u8 DefaultPSSource[] =
    {
#include "shader/DefaultPS.byte"
    };
    const u32 DefaultPSSourceSize = sizeof(DefaultPSSource);

    const u8 DefaultPNUVSSource[] =
    {
#include "shader/DefaultPNUVS.byte"
    };
    const u32 DefaultPNUVSSourceSize = sizeof(DefaultPNUVSSource);

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

    const u8 DepthGSSource[] =
    {
#include "shader/DepthGS.byte"
    };
    const u32 DepthGSSourceSize = sizeof(DepthGSSource);

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

    //FullQuad
    //------------------------------------
    const u8 FullQuadVSSource[] =
    {
#include "shader/FullQuadVS.byte"
    };
    const u32 FullQuadVSSourceSize = sizeof(FullQuadVSSource);

    //FXAA
    //------------------------------------
    const u8 FXAAVSSource[] =
    {
#include "shader/FXAAVS.byte"
    };
    const u32 FXAAVSSourceSize = sizeof(FXAAVSSource);

    const u8 FXAAPSSource[] =
    {
#include "shader/FXAAPS.byte"
    };
    const u32 FXAAPSSourceSize = sizeof(FXAAPSSource);

    //TonemappingStatic
    //------------------------------------
    const u8 TonemappingStaticPSSource[] =
    {
#include "shader/TonemappingStaticPS.byte"
    };
    const u32 TonemappingStaticPSSourceSize = sizeof(TonemappingStaticPSSource);

    //TonemappingStaticAO
    //------------------------------------
    const u8 TonemappingStaticAOPSSource[] =
    {
#include "shader/TonemappingStaticAOPS.byte"
    };
    const u32 TonemappingStaticAOPSSourceSize = sizeof(TonemappingStaticAOPSSource);

    //Glare
    //------------------------------------
    const u8 GlarePSSource[] =
    {
#include "shader/GlarePS.byte"
    };
    const u32 GlarePSSourceSize = sizeof(GlarePSSource);

    //Gauss
    //------------------------------------
    const u8 GaussPSSource[] =
    {
#include "shader/GaussPS.byte"
    };
    const u32 GaussPSSourceSize = sizeof(GaussPSSource);

    //SkyBox
    //------------------------------------
    const u8 SkyBoxVSSource[] =
    {
#include "shader/SkyBoxVS.byte"
    };
    const u32 SkyBoxVSSourceSize = sizeof(SkyBoxVSSource);

    const u8 SkyBoxPSSource[] =
    {
#include "shader/SkyBoxPS.byte"
    };
    const u32 SkyBoxPSSourceSize = sizeof(SkyBoxPSSource);

    //Debug
    //------------------------------------
    const u8 DebugVSSource[] =
    {
#include "shader/DebugVS.byte"
    };
    const u32 DebugVSSourceSize = sizeof(DebugVSSource);

    const u8 DebugPSSource[] =
    {
#include "shader/DebugPS.byte"
    };
    const u32 DebugPSSourceSize = sizeof(DebugPSSource);

    //Voxelize
    //------------------------------------
    const u8 VoxelizePVSSource[] =
    {
#include "shader/VoxelizeVS.byte"
    };
    const u32 VoxelizePVSSourceSize = sizeof(VoxelizePVSSource);

    const u8 VoxelizePGSSource[] =
    {
#include "shader/VoxelizeGS.byte"
    };
    const u32 VoxelizePGSSourceSize = sizeof(VoxelizePGSSource);


    const u8 VoxelizePSSource[] =
    {
#include "shader/VoxelizePS.byte"
    };
    const u32 VoxelizePSSourceSize = sizeof(VoxelizePSSource);


    //VoxelRender
    //------------------------------------
    const u8 VoxelRenderVSSource[] =
    {
#include "shader/VoxelRenderVS.byte"
    };
    const u32 VoxelRenderVSSourceSize = sizeof(VoxelRenderVSSource);

    const u8 VoxelRenderGSSource[] =
    {
#include "shader/VoxelRenderGS.byte"
    };
    const u32 VoxelRenderGSSourceSize = sizeof(VoxelRenderGSSource);

    const u8 VoxelRenderPSSource[] =
    {
#include "shader/VoxelRenderPS.byte"
    };
    const u32 VoxelRenderPSSourceSize = sizeof(VoxelRenderPSSource);

    //Reflective ShadowMap
    //------------------------------------
    const u8 ReflectiveShadowMapVSSource[] =
    {
#include "shader/ReflectiveShadowMapVS.byte"
    };
    const u32 ReflectiveShadowMapVSSourceSize = sizeof(ReflectiveShadowMapVSSource);


    const u8 ReflectiveShadowMapPSSource[] =
    {
#include "shader/ReflectiveShadowMapPS.byte"
    };
    const u32 ReflectiveShadowMapPSSourceSize = sizeof(ReflectiveShadowMapPSSource);

    const u8 ReflectiveShadowMapTextureVSSource[] =
    {
#include "shader/ReflectiveShadowMapTextureVS.byte"
    };
    const u32 ReflectiveShadowMapTextureVSSourceSize = sizeof(ReflectiveShadowMapTextureVSSource);

    const u8 ReflectiveShadowMapTexturePSSource[] =
    {
#include "shader/ReflectiveShadowMapTexturePS.byte"
    };
    const u32 ReflectiveShadowMapTexturePSSourceSize = sizeof(ReflectiveShadowMapTexturePSSource);

    //VoxelInjection
    //------------------------------------
    const u8 VoxelInjectionVSSource[] =
    {
#include "shader/VoxelInjectionVS.byte"
    };
    const u32 VoxelInjectionVSSourceSize = sizeof(VoxelInjectionVSSource);


    const u8 VoxelInjectionPSSource[] =
    {
#include "shader/VoxelInjectionPS.byte"
    };
    const u32 VoxelInjectionPSSourceSize = sizeof(VoxelInjectionPSSource);

    //VoxelMipmap
    //------------------------------------
    const u8 VoxelMipmapVSSource[] =
    {
#include "shader/VoxelMipmapVS.byte"
    };
    const u32 VoxelMipmapVSSourceSize = sizeof(VoxelMipmapVSSource);


    const u8 VoxelMipmapPSSource[] =
    {
#include "shader/VoxelMipmapPS.byte"
    };
    const u32 VoxelMipmapPSSourceSize = sizeof(VoxelMipmapPSSource);

    //ReconstructZ
    //------------------------------------
    const u8 ReconstructZSource[] =
    {
#include "shader/ReconstructZPS.byte"
    };
    const u32 ReconstructZSourceSize = sizeof(ReconstructZSource);

    //Downsample2x2
    //------------------------------------
    const u8 Downsample2x2PSSource[] =
    {
#include "shader/Downsample2x2PS.byte"
    };
    const u32 Downsample2x2PSSourceSize = sizeof(Downsample2x2PSSource);

    //Copy
    //------------------------------------
    const u8 CopyPSSource[] =
    {
#include "shader/CopyPS.byte"
    };
    const u32 CopyPSSourceSize = sizeof(CopyPSSource);

    //SAO
    //------------------------------------
    const u8 SAOPSSource[] =
    {
#include "shader/SAOPS.byte"
    };
    const u32 SAOPSSourceSize = sizeof(SAOPSSource);

    //SAOBlur
    //------------------------------------
    const u8 SAOBlurPSSource[] =
    {
#include "shader/SAOBlurPS.byte"
    };
    const u32 SAOBlurPSSourceSize = sizeof(SAOBlurPSSource);


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
        { DefaultPNVSSource, DefaultPNVSSourceSize, DefaultPSSource, DefaultPSSourceSize },
        { DefaultPNUVSSource, DefaultPNUVSSourceSize, DefaultTexturePSSource, DefaultTexturePSSourceSize },
        { DepthVSSource, DepthVSSourceSize, DepthPSSource, DepthPSSourceSize },
        { RectangleVSSource, RectangleVSSourceSize, RectanglePSSource, RectanglePSSourceSize },
        { DebugVCVSSource, DebugVCVSSourceSize, DebugVCPSSource, DebugVCPSSourceSize },
        { FullQuadVSSource, FullQuadVSSourceSize, NULL, 0 },
        { FXAAVSSource, FXAAVSSourceSize, FXAAPSSource, FXAAPSSourceSize },
        { NULL, 0, TonemappingStaticPSSource, TonemappingStaticPSSourceSize },
        { NULL, 0, TonemappingStaticAOPSSource, TonemappingStaticAOPSSourceSize },
        { NULL, 0, GlarePSSource, GlarePSSourceSize },
        { NULL, 0, GaussPSSource, GaussPSSourceSize },
        { SkyBoxVSSource, SkyBoxVSSourceSize, SkyBoxPSSource, SkyBoxPSSourceSize },
        { DebugVSSource, DebugVSSourceSize, DebugPSSource, DebugPSSourceSize },

        { VoxelizePVSSource, VoxelizePVSSourceSize, VoxelizePSSource, VoxelizePSSourceSize },
        { VoxelRenderVSSource, VoxelRenderVSSourceSize, VoxelRenderPSSource, VoxelRenderPSSourceSize },
        { ReflectiveShadowMapVSSource, ReflectiveShadowMapVSSourceSize, ReflectiveShadowMapPSSource, ReflectiveShadowMapPSSourceSize },
        { ReflectiveShadowMapTextureVSSource, ReflectiveShadowMapTextureVSSourceSize, ReflectiveShadowMapTexturePSSource, ReflectiveShadowMapTexturePSSourceSize },
        { VoxelInjectionVSSource, VoxelInjectionVSSourceSize, VoxelInjectionPSSource, VoxelInjectionPSSourceSize },
        { VoxelMipmapVSSource, VoxelMipmapVSSourceSize, VoxelMipmapPSSource, VoxelMipmapPSSourceSize },

        { NULL, 0, ReconstructZSource, ReconstructZSourceSize },
        { NULL, 0, Downsample2x2PSSource, Downsample2x2PSSourceSize },
        { NULL, 0, CopyPSSource, CopyPSSourceSize },
        { NULL, 0, SAOPSSource, SAOPSSourceSize },
        { NULL, 0, SAOBlurPSSource, SAOBlurPSSourceSize },
    };


    //-----------------------------------------------------------------------------
    struct EntryGS
    {
        const u8* gssrc_;
        u32 gssize_;
    };

    const EntryGS ShaderGSEntries[ShaderGS_Num]=
    {
        { NULL, 0 },
        { DepthGSSource, DepthGSSourceSize },
        { VoxelizePGSSource, VoxelizePGSSourceSize },
        { VoxelRenderGSSource, VoxelRenderGSSourceSize },
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
