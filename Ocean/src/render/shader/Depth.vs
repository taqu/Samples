cbuffer ConstantBuffer : register(b0)
{
    float4x4 mwvp;
}

//--------------------------------
struct VSInput
{
    float3 position : POSITION;
    //float3 normal : NORMAL;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;

    float4 outPos = mul(float4(input.position, 1.0f), mwvp);
    //outPos.z *= outPos.w;

    output.position = outPos;

    return output;
}
