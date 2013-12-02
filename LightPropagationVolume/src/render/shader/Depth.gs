//--------------------------------
struct VSOutput
{
    float4 position : SV_POSITION;
    uint instanceId : SV_InstanceID;
};

struct GSOutput
{
    float4 position : SV_POSITION;
    uint rtIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(3)]  
void main(triangle VSOutput input[3], inout TriangleStream<GSOutput> triStream)
{
    GSOutput output= (GSOutput)0;
    output.rtIndex = input[0].instanceId;

    output.position = input[0].position;
    triStream.Append(output);

    output.position = input[1].position;
    triStream.Append(output);

    output.position = input[2].position;
    triStream.Append(output);

    triStream.RestartStrip();
}
