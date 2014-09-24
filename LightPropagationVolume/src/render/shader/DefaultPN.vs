#define NUM_CASCADES 3

cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 mwvp;
    float4x4 mw;
    float4x4 mv;
    float4x4 mlwvp[4];
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
    float3 worldPos : TEXCOORD1;
    float3 viewNormal : TEXCOORD2;
    float4 texS[NUM_CASCADES] : TEXCOORD3;
};

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;

    //linear-z
    float4 position = float4(input.position, 1.0f);
    float4 outPos;

    outPos = mul(position, mwvp);
    outPos.z *= outPos.w;
    output.position = outPos;

    output.normal.xyz = normalize( mul(input.normal, (float3x3)mw) );
    [unroll(4)]
    for(int i=0; i<NUM_CASCADES; ++i){
        output.texS[i] = mul(position, mlwvp[i]);
    }

    float4 world = mul(position, mw);
    output.worldPos = world.xyz;

    output.viewNormal = normalize( mul(output.normal, (float3x3)mv) );

    return output;
}
