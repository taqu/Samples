cbuffer ConstantBuffer0 : register(b0)
{
    float4 info : packoffset(c0.x); //{near, aspect, 0, 0}
    float4x4 mivp : packoffset(c1.x);
}

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 dir : TEXCOORD0;
};

VSOutput main(uint vertexID : SV_VertexID)
{
    uint sx,sy;
    sx = vertexID & 0x01U;
    sy = vertexID >> 1;

    float vx = 2.0*sx - 1;
    float vy = 1 - 2.0*sy;

    float4 dir = float4(vx, vy, info.x, 1);
    dir = mul(dir, mivp);

    VSOutput output;
    output.pos = float4(vx, vy, 0.999, 1);
    output.dir = normalize(dir.xyz);
    return output;
}
