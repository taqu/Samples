cbuffer ConstantBuffer0 : register(b0)
{
    int size;
    int mask;
    int shift;
    int padding00;

    float invWidth;
    float invHeight;
    float occlusionAttenuation;
    float padding02;
}

cbuffer ConstantBuffer1 : register(b1)
{
    float4x4 mivp;
    float4 LPVMin; //‚S”Ô–Ú‚ªƒZƒ‹ƒTƒCƒY‚Ì‹t”
    float4 viewZ;
}

Texture2D texColor : register(t0);
Texture2D texNormal : register(t1);
Texture2D texDepth : register(t2);

struct VSOutput
{
    float4 position : POSITION;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
};

VSOutput main(uint vertexID : SV_VertexID)
{
    int u = vertexID & mask;
    int v = (int)(vertexID >> shift);

    int3 uvw = int3(u, v, 0);
    float3 normal = texNormal.Load(uvw).rgb;
    normal = normalize(normal * 2.0f - float3(1.0f, 1.0f, 1.0f));

    float4 color = texColor.Load(uvw);
    float depth = texDepth.Load(uvw).x;

    float4 projected = float4(invWidth*u*2.0f-1.0f, 1.0f-invHeight*v*2.0f, depth, 1.0f);
    float4 pos = mul(projected, mivp);
    pos.xyz = (pos.xyz - LPVMin.xyz) * LPVMin.w - float3(0.5f, 0.5f, 0.5f);

    float z = dot(projected, viewZ);

    VSOutput output;
    output.position = pos;
    output.normal = normal;
    output.color.rgb = color.rgb * color.a;
    output.color.a = z * z * occlusionAttenuation;//invWidth * invHeight * invLPVCellSize * invLPVCellSize;
    return output;
}
