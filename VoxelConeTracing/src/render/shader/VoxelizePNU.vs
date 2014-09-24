cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 mw;
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
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;

    float4 position = float4(input.position, 1.0f);
    output.position = mul(position, mw);
    output.uv = input.uv;
    return output;
}
