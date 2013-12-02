cbuffer ConstantBuffer : register(b0)
{
    float4x4 mwvp[4];
}

//--------------------------------
struct VSInput
{
    float3 position : POSITION;
    uint instanceId : SV_InstanceID;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    uint instanceId : SV_InstanceID;
};

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;

    float4 outPos = mul(float4(input.position, 1.0f), mwvp[input.instanceId]);
    //outPos.z *= outPos.w;

    output.position = outPos;
    output.instanceId = input.instanceId;
    return output;
}
