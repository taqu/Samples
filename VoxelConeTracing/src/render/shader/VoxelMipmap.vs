cbuffer ConstantBuffer0 : register(b0)
{
    int resolution;
    int resolution2;
    float invResolution;
    float invResolution2;
}

struct VSOutput
{
    float4 position : SV_Position;
    uint3 tex0 : TEXCOORD0;
};

VSOutput main(uint vertexID : SV_VertexID)
{
    int z = (int)(vertexID * invResolution2);
    int td = (int)(vertexID - z*resolution2);

    int y = (int)(td * invResolution);
    int x = (int)(td - y*resolution);

    VSOutput output = (VSOutput)0;
    output.position = float4(0,0,0.5,1);
    output.tex0.xyz = uint3(x, y, z);
    return output;
}
