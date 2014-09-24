cbuffer ConstantBuffer0 : register(b0)
{
    int4 voxelSize; //voxel resolution, (voxel resolution)^2
    float4 voxelInvSize; //1/(voxel resolution), 1/(voxel resolution)^2, 0.5/(voxel resolution)
}

cbuffer ConstantBuffer1 : register(b1)
{
    float4 voxelCenter;
    float4 voxelWorldSize;
}

struct VSOutput
{
    float3 position : TEXCOORD0;
    uint4 tex0 : TEXCOORD1;
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

    float3 p = float3(x,y,z) * voxelInvSize.x - float3(0.5, 0.5, 0.5);
    p *= voxelWorldSize.xyz;

    VSOutput output = (VSOutput)0;
    output.position = p;
    output.tex0 = uint4(x,y,z,0);//getBytePos(x,y,z);
    return output;
}
