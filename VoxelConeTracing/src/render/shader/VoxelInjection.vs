cbuffer ConstantBuffer0 : register(b0)
{
    int4 voxelSize; //voxel resolution, (voxel resolution)^2, voxel resolution * num samples, num samples
    float4 voxelInvSize; //1/(voxel resolution), 1/(voxel resolution)^2, 0.5/(voxel resolution), 1/(num samples)^3
}

cbuffer ConstantBuffer1 : register(b1)
{
    float4 voxelCenter;
    float4 voxelWorldSize;
}

cbuffer ConstantBuffer2 : register(b2)
{
    float4x4 mlwvp;
}

struct VSOutput
{
    float4 position : SV_Position;
    uint4 tex0 : TEXCOORD0;
    float4 texS : TEXCOORD1;
};

uint getBytePos(int x, int y, int z)
{
    return (uint)(x + y*voxelSize.x + z*voxelSize.y);
}

VSOutput main(uint vertexID : SV_VertexID)
{
    int z = (int)(vertexID * voxelInvSize.y);
    int td = (int)(vertexID - z*voxelSize.y);

    int y = (int)(td * voxelInvSize.x);
    int x = (int)(td - y*voxelSize.x);

    float3 p = float3(x,y,z) * voxelInvSize.w;

    float3 viewPos = p - float3(0.5, 0.5, 0.5);
    viewPos *= voxelWorldSize.xyz;

    VSOutput output = (VSOutput)0;
    output.position.xy = p.xy*2 - float2(1,1);
    output.position.zw = float2(p.z, 1);
    output.tex0.xyz = uint3(x,y,z);
    output.tex0.w = getBytePos(x,y,z);
    output.texS = mul(float4(viewPos, 1), mlwvp);
    return output;
}
