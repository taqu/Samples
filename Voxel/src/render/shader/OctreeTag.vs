
struct VSOutput
{
    float4 pos : SV_Position;
    uint vertexID : Index;
};

VSOutput main(uint vertexID : SV_VertexID)
{
    VSOutput output;
    output.pos = float4(0.5, 0.5, 0.5, 1);
    output.vertexID = vertexID;
    return output;
}
