/**
@file FontShader.cpp
@author t-sakai
@date 2011/09/24
*/
#include "FontShader.h"
#include <lmath/lmath.h>
#include <lgraphics/lgraphics.h>

namespace font
{
    void ShaderVS::initialize()
    {
        constants_ = lgraphics::ConstantBuffer::create(
            sizeof(ConstantBuffer),
            lgraphics::Usage_Default,
            lgraphics::CPUAccessFlag_None,
            lgraphics::ResourceMisc_None);

    }


    namespace
    {
        // バーテックスシェーダソース
        const char FontVSSource[] =
            "cbuffer ConstantBuffer : register(b0)\n"
            "{\n"
            "float4 xparam;\n"
            "float4 yparam;\n"
            "}\n"
            "struct VSInput\n"
            "{\n"
            "float2 position : POSITION;\n"
            "float2 tex0 : TEXCOORD0;\n"
            "};\n"
            "struct VSOut\n"
            "{\n"
            "float4 position : SV_POSITION;\n"
            "float2 tex0 : TEXCOORD0;\n"
            "};\n"
            "VSOut main(VSInput input)\n"
            "{\n"
            "VSOut output = (VSOut)0;\n"
            "float4 pos = float4(input.position, 0.0, 1.0);\n"
            "output.position.x = dot(pos, xparam);\n"
            "output.position.y = dot(pos, yparam);\n"
            "output.position.z = 0.0;\n"
            "output.position.w = 1.0;\n"
            "output.tex0 = input.tex0;\n"
            "return output;\n"
            "}\n";

        const lcore::u32 FontVSSourceSize = sizeof(FontVSSource);


        // ピクセルシェーダソース
        const char FontPSSource[] =
            "Texture2D texAlbedo : register(t0);\n"
            "SamplerState samLinear : register(s0);\n"
            "struct VSOut\n"
            "{\n"
            "float4 position : SV_POSITION;\n"
            "float2 tex0 : TEXCOORD0;\n"
            "};\n"
            "float4 main(VSOut input) : SV_Target\n"
            "{\n"
            "float4 color = texAlbedo.Sample(samLinear, input.tex0);\n"
            "color.a = color.g + color.b;\n"
            "color.rgb = color.g;\n"
            "return color;\n"
            "}\n";

        const lcore::u32 FontPSSourceSize = sizeof(FontPSSource);


        // ピクセルシェーダソース
        const char FontDistanceFieldPSSource[] =
            "static const float SmoothCenter = 0.10f;\n"
            "static const float OutlineCenter = 0.24f;\n"
            "static const float OutLineColor = 0.1f;\n"
            "static const float GlyphColor = 1.0f;\n"
            "Texture2D texAlbedo : register(t0);\n"
            "SamplerState samLinear : register(s0);\n"
            "struct VSOut\n"
            "{\n"
            "float4 position : SV_POSITION;\n"
            "float2 tex0 : TEXCOORD0;\n"
            "};\n"
            "float4 main(VSOut input) : SV_Target\n"
            "{\n"
            "float4 color = texAlbedo.Sample(samLinear, input.tex0);\n"
            "float smoothWidth = fwidth(color.r);\n"
            "float r = smoothstep(OutlineCenter-smoothWidth, OutlineCenter+smoothWidth, color.r);\n"
            "color.a = smoothstep(SmoothCenter-smoothWidth, SmoothCenter+smoothWidth, color.r);\n"
            "color.rgb = lerp(OutLineColor, GlyphColor, r);\n"
            "return color;\n"
            "}\n";

        const lcore::u32 FontDistanceFieldPSSourceSize = sizeof(FontDistanceFieldPSSource);
    }

    // 頂点シェーダ作成
    void createShader(ShaderVS& vs, lgraphics::BlobRef& blob)
    {
        lgraphics::VertexShaderRef vsRef = lgraphics::Shader::createVertexShaderFromMemory(
            FontVSSource,
            FontVSSourceSize,
            0,
            NULL,
            &blob);

        vs.shader_ = vsRef;

        vs.initialize();

    }

    // ピクセルシェーダ作成
    void createShader(ShaderPS& ps)
    {
        lgraphics::PixelShaderRef psRef = lgraphics::Shader::createPixelShaderFromMemory(
            FontPSSource,
            FontPSSourceSize,
            0,
            NULL,
            NULL);

        ps.shader_ = psRef;
    }

    // ピクセルシェーダ作成
    void createDistanceFieldShader(ShaderPS& ps)
    {
        lgraphics::PixelShaderRef psRef = lgraphics::Shader::createPixelShaderFromMemory(
            FontDistanceFieldPSSource,
            FontDistanceFieldPSSourceSize,
            0,
            NULL,
            NULL);

        ps.shader_ = psRef;
    }
}
