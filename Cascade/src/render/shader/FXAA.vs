cbuffer ConstantBuffer0 : register(b0)
{
    float4 rcpFrame; //(1/width, 1/height, 0, 0)
    float4 rcpFrameOpt; //(2/width, 2/height, 0.5/width, 0.5/height)
}

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 posPos : TEXCOORD1;
};

VSOutput main(uint id : SV_VertexID)
{
    VSOutput output;
    float2 tex = float2((id<<1)&0x02U, id&0x02U);
    output.position = float4(tex * float2(2,-2) + float2(-1, 1), 0.5, 1);

    output.tex = tex + rcpFrameOpt.zw;
    output.posPos.xy = tex;
    output.posPos.zw = tex + rcpFrame.xy;
    return output;
}
