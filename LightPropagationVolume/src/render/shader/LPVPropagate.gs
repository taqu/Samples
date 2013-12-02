struct VSOutput
{
    float4 position : SV_Position;
    uint depth : DepthIndex;
};

struct GSOutput
{
    float4 position : SV_Position;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount (3)]
void main(triangle VSOutput input[3], inout TriangleStream<GSOutput> triangleStream)
{
    GSOutput output;
    [unroll]
    for(int i=0; i<3; ++i){
        output.position = input[i].position;
        output.RTIndex = input[i].depth;
        triangleStream.Append(output);
    }
}
