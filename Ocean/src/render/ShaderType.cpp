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

    //Ocean
    //------------------------------------
    const u8 OceanVSSource[] =
    {
#include "shader/OceanVS.byte"
    };
    const u32 OceanVSSourceSize = sizeof(OceanVSSource);

    const u8 OceanPSSource[] =
    {
#include "shader/OceanPS.byte"
    };
    const u32 OceanPSSourceSize = sizeof(OceanPSSource);

    //OceanWire
    //------------------------------------
    const u8 OceanWirePSSource[] =
    {
#include "shader/OceanWirePS.byte"
    };
    const u32 OceanWirePSSourceSize = sizeof(OceanWirePSSource);

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

    //HDAO
    //------------------------------------
    const u8 HDAOPSSource[] =
    {
#include "shader/HDAOPS.byte"
    };
    const u32 HDAOPSSourceSize = sizeof(HDAOPSSource);

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
        { FullQuadVSSource, FullQuadVSSourceSize, NULL, 0 },
        { FXAAVSSource, FXAAVSSourceSize, FXAAPSSource, FXAAPSSourceSize },
        { NULL, 0, TonemappingStaticPSSource, TonemappingStaticPSSourceSize },
        { NULL, 0, GlarePSSource, GlarePSSourceSize },
        { NULL, 0, GaussPSSource, GaussPSSourceSize },
        { OceanVSSource, OceanVSSourceSize, OceanPSSource, OceanPSSourceSize },
        { NULL, 0, OceanWirePSSource, OceanWirePSSourceSize },
        { SkyBoxVSSource, SkyBoxVSSourceSize, SkyBoxPSSource, SkyBoxPSSourceSize },
        { NULL, 0, HDAOPSSource, HDAOPSSourceSize },
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
