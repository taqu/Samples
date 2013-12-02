cbuffer ConstantBuffer : register(b0)
{
    float4 pos[4];
}

struct VSInput
{
    uint vertexID : SV_VertexID;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 tex0 : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;

    output.position = float4(pos[input.vertexID].xy, 0.0f, 1.0f);

    output.tex0 = pos[input.vertexID].zw;

    return output;
}
