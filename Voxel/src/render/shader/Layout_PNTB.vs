struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

float4 main( VS_INPUT input ) : SV_POSITION
{
    return float4(input.position, 1);
}
