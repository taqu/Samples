cbuffer ConstantBuffer0 : register(b0)
{
    float4 voxelSize;
}

struct VSOutput
{
    float4 position : SV_Position;
};

struct GSOutput
{
    float4 position : SV_Position;
};

[maxvertexcount(3)]
void main(triangle VSOutput input[3], inout TriangleStream<GSOutput> triStream)
{
    GSOutput output;
    output.position = input[0].position;
    triStream.Append(output);

    output.position = input[1].position;
    triStream.Append(output);

    output.position = input[2].position;
    triStream.Append(output);

    triStream.RestartStrip();
}
