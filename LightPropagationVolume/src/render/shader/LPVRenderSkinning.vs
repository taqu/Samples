#define NUM_PALETTE_MATRICES 256

cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 mwvp;
    float4x4 mw;
    float4x4 mlwvp;
}

cbuffer ConstantBuffer1 : register(b1)
{
    float4 LPVMin;
    float invLPVSize;
    float invLPVTexSize;
    float LPVAttenuation;
    float padding2;
}

cbuffer ConstantBuffer2 : register(b2)
{
    float4 palette[3*NUM_PALETTE_MATRICES];
}

//--------------------------------
struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    uint2 bone : BLENDINDICES;
    float weight : BLENDWEIGHT;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : TEXCOORD0;
    float4 texLPV : TEXCOORD1;
    float3 texS : TEXCOORD2;
    float3 worldPos : TEXCOORD3;
};

float4 skinningPosition(float4 position, uint index, float weight)
{
    index *= 3;
    float4 ret;
    ret.x = dot(position, palette[index + 0]) * weight;
    ret.y = dot(position, palette[index + 1]) * weight;
    ret.z = dot(position, palette[index + 2]) * weight;
    ret.w = 1;
    return ret;
}

float3 skinningNormal(float3 normal, uint index, float weight)
{
    index *= 3;
    float3 ret;
    ret.x = dot(normal, palette[index + 0].xyz) * weight;
    ret.y = dot(normal, palette[index + 1].xyz) * weight;
    ret.z = dot(normal, palette[index + 2].xyz) * weight;
    return ret;
}

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;

    float4 position = float4(input.position, 1);
    position = skinningPosition(position, input.bone.x, input.weight);
    position.xyz += skinningPosition(position, input.bone.y, 1-input.weight).xyz;

    float3 normal = skinningNormal(input.normal, input.bone.x, input.weight);
    normal += skinningNormal(input.normal, input.bone.y, 1-input.weight);

    float4 outPos;

    //linear-z
    outPos = mul(position, mwvp);
    outPos.z *= outPos.w;
    output.position = outPos;

    output.normal.xyz = normalize( mul(normal, (float3x3)mw) );

    output.texS = mul(position, mlwvp).xyz;//float3(0.5f*lpos.x + 0.5f, 0.5f - 0.5f*lpos.y, lpos.z);
    output.texS.z = saturate(output.texS.z);

    float4 world = mul(position, mw);
    output.texLPV.xyz = (world.xyz - LPVMin.xyz) * LPVMin.w * invLPVTexSize;
    output.texLPV.w = LPVAttenuation;
    output.worldPos = world.xyz;
    return output;
}
