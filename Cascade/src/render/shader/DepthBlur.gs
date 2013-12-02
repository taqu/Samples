//--------------------------------
struct VSOutput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    uint instanceId : SV_InstanceID;
};

struct GSOutput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    uint rtIndex : SV_RenderTargetArrayIndex;
};

[maxvertexcount(3)]  
void main(triangle VSOutput input[3], inout TriangleStream<GSOutput> triStream)
{
    GSOutput output= (GSOutput)0;
    output.rtIndex = input[0].instanceId;

    output.position = input[0].position;
    output.tex = input[0].tex;
    triStream.Append(output);

    output.position = input[1].position;
    output.tex = input[1].tex;
    triStream.Append(output);

    output.position = input[2].position;
    output.tex = input[2].tex;
    triStream.Append(output);

    triStream.RestartStrip();
}
