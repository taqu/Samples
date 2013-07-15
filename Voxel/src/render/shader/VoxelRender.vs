cbuffer ConstantBuffer0 : register(b0)
{
    float4 info; //{near, aspect}
    uint4 screen; //{width, height, 0, 0}
    float4x4 miv;
}

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 dir : TEXCOORD0;
};

VSOutput main(uint vertexID : SV_VertexID)
{
    uint sx,sy;
    sy = vertexID/screen.x;
    sx = vertexID - screen.x * sy;

    float vx = 2.0*sx/(float)screen.x - 1;
    float vy = 1 - 2.0*sy/(float)screen.y;

    float3 dir = float3(vx*info.y, vy, info.x);
    dir = normalize(dir);
    dir = mul(dir, (float3x3)miv);

    VSOutput output;
    output.pos = float4(vx, vy, 0.5, 1);
    output.dir = dir;
    return output;
}
