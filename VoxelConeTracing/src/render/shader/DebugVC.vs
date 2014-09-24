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
    output.position = float4(input.position, 1.0f);
    output.color = input.color;

    return output;
}
