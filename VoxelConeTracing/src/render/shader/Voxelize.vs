cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 mwvp;
}

//--------------------------------
struct VSInput
{
    float3 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_Position;
};

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;

    float4 position = mul(float4(input.position, 1), mwvp);
    position.xy = position.xy * 0.5 + float2(0.5, 0.5);
    position.w = 1;
    output.position = position;
    return output;
}
