static const float4 ScreenPosition[4] =
{
    float4(-1.0f,  1.0f, 0.0f, 1.0f),
    float4( 1.0f,  1.0f, 0.0f, 1.0f),
    float4(-1.0f, -1.0f, 0.0f, 1.0f),
    float4( 1.0f, -1.0f, 0.0f, 1.0f),
};

struct VSInput
{
    uint vertexID : SV_VertexID;
    uint instanceID : SV_InstanceID;
};

struct VSOutput
{
    float4 position : SV_Position;
    uint depth : DepthIndex;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = ScreenPosition[input.vertexID];
    output.depth = input.instanceID;
    return output;
}
