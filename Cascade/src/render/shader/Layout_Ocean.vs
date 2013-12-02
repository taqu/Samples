struct VS_INPUT
{
    float2 position : POSITION;
};

float4 main( VS_INPUT input ) : SV_POSITION
{
    return float4(input.position, 0, 1);
}
