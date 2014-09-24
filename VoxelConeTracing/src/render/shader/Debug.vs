cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 mwvp;
}

struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;
    float4 position = float4(input.position, 1.0f);
    output.position = mul(position, mwvp);
    output.color = input.color;

    return output;
}
