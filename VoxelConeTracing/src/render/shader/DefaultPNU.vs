#define NUM_CASCADES 2
#define MAX_CASCADES 4

cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 mwvp;
    float4x4 mw;
    float4x4 mv;
    float4 cameraPos;
    float4x4 mlwvp[MAX_CASCADES];
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
    float4 worldPos : TEXCOORD2;
    float3 viewNormal : TEXCOORD3;
    float4 texS[NUM_CASCADES] : TEXCOORD4;
};

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;

    float4 position = float4(input.position, 1.0f);

    output.position = mul(position, mwvp);

    output.normal.xyz = normalize( mul(input.normal, (float3x3)mw) );
    output.uv = input.uv;

    output.worldPos.xyz = mul(position, mw).xyz;
    output.worldPos.w = distance(output.worldPos.xyz, cameraPos.xyz);
    output.viewNormal = normalize( mul(output.normal, (float3x3)mv) );
    [unroll(MAX_CASCADES)]
    for(int i=0; i<NUM_CASCADES; ++i){
        output.texS[i] = mul(position, mlwvp[i]);
    }

    return output;
}
