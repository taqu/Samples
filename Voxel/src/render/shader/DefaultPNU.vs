cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 mwvp;
    float4x4 mw;
    float4x4 mlwvp;

}

//--------------------------------
struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float3 texS : TEXCOORD2;
    float3 worldPos : TEXCOORD3;
};

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;

    float4 position = float4(input.position, 1.0f);
    float4 outPos;

    outPos = mul(position, mwvp);
    output.position = outPos;

    output.normal.xyz = normalize( mul(input.normal, (float3x3)mw) );
    output.uv = input.uv;
    output.texS = mul(position, mlwvp).xyz;//float3(0.5f*lpos.x + 0.5f, 0.5f - 0.5f*lpos.y, lpos.z);
    output.texS.z = saturate(output.texS.z);

    float4 world = mul(position, mw);
    output.worldPos = world.xyz;
    return output;
}
