cbuffer ConstantBuffer0 : register(b0)
{
    float4 dlDir;
    float4 dlColor;
}

cbuffer ConstantBuffer1 : register(b1)
{
    float4x4 mwvp;
    float4x4 mw;
    float4 diffuse;
}

//--------------------------------
struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : TEXCOORD0;
    float4 direct : TEXCOORD1;
};

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;

    float4 position = float4(input.position, 1.0f);
    
    float4 outPos = mul(position, mwvp);
    //outPos.z *= outPos.w;

    output.position = outPos;
    output.normal = normalize(mul(input.normal, (float3x3)mw));

    float d = max(dot(dlDir.xyz, output.normal), 0.0f);
    output.direct.rgb = d * diffuse.rgb * dlColor.xyz;
    output.direct.a = diffuse.a;

    return output;
}
