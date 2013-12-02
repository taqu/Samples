struct VSInput
{
    float4 orientation : POSITION;
    float3 position : TEXCOORD0;
    float3 size : TEXCOORD1;
    float4 color : TEXCOORD2;
};

float4 main( VSInput input ) : SV_POSITION
{
    return float4(input.position, 1);
}
