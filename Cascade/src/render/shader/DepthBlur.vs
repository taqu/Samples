struct VSOutput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    uint instanceId : SV_InstanceID;
};

VSOutput main(uint id : SV_VertexID, uint instanceId : SV_InstanceID)
{
    VSOutput output;
    float2 tex = float2((id<<1)&0x02U, id&0x02U);
    output.position = float4(tex * float2(2,-2) + float2(-1, 1), 0.5, 1);

    output.tex = tex;
    output.instanceId = instanceId;

    return output;
}
