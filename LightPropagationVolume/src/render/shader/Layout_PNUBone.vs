struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    uint2 bone : BLENDINDICES;
    float weight : BLENDWEIGHT;
};

float4 main( VS_INPUT input ) : SV_POSITION
{
    return float4(input.position, 1);
}
