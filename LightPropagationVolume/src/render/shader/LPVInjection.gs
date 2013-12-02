cbuffer ConstantBuffer0 : register(b0)
{
    float invLPVResolution;
    float padding0;
    float padding1;
    float padding2;
}

struct VSOutput
{
    float4 position : POSITION;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
};

struct GSOutput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount (1)]
void main(point VSOutput input[1], inout PointStream<GSOutput> pointStream)
{
    GSOutput output;
    output.RTIndex = input[0].position.z;
    output.position.xy = (input[0].position.xy)*invLPVResolution*2.0f - 1.0f;
    output.position.y = -output.position.y;
    output.position.zw = float2(0.0f, 1.0f);

    output.normal = input[0].normal;
    output.color = input[0].color;
    pointStream.Append(output);
}
