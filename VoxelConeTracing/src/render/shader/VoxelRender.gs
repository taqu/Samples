cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 mwvp;
}

cbuffer ConstantBuffer1 : register(b1)
{
    int4 voxelSize; //voxel resolution, (voxel resolution)^2, voxel resolution * num samples, num samples
    float4 voxelInvSize; //1/(voxel resolution), 1/(voxel resolution)^2, 0.5/(voxel resolution), 1/(num samples)^3
}

cbuffer ConstantBuffer2 : register(b2)
{
    float4 voxelCenter;
    float4 voxelWorldSize;
}

struct VSOutput
{
    float3 position : TEXCOORD0;
    uint4 tex0 : TEXCOORD1;
};

struct GSOutput
{
    float4 position : SV_Position;
    float4 coverage : TEXCOORD0;
};

Texture3D<float4> tex0[6] : register(t0);

float4 sampleValue(uint4 pos)
{
    float4 ret = (float4)0;

    [unroll(6)]
    for(int i=0; i<6; ++i){
        ret += tex0[i].Load(pos);
    }
    return ret/6.0;
}

[maxvertexcount(24)]
void main(point VSOutput input[1], inout TriangleStream<GSOutput> triStream)
{
    float4 value = sampleValue(input[0].tex0);
    if(value.w<=0){
        return;
    }
    float3 size = voxelWorldSize.xyz * voxelInvSize.x;

    float3 aabbMin = input[0].position;
    float3 aabbMax = input[0].position + size;

    float4 position[8];
    position[0] = mul(float4(aabbMax.x, aabbMax.y, aabbMax.z, 1), mwvp);
    position[1] = mul(float4(aabbMax.x, aabbMax.y, aabbMin.z, 1), mwvp);
    position[2] = mul(float4(aabbMax.x, aabbMin.y, aabbMax.z, 1), mwvp);
    position[3] = mul(float4(aabbMax.x, aabbMin.y, aabbMin.z, 1), mwvp);

    position[4] = mul(float4(aabbMin.x, aabbMax.y, aabbMax.z, 1), mwvp);
    position[5] = mul(float4(aabbMin.x, aabbMax.y, aabbMin.z, 1), mwvp);
    position[6] = mul(float4(aabbMin.x, aabbMin.y, aabbMax.z, 1), mwvp);
    position[7] = mul(float4(aabbMin.x, aabbMin.y, aabbMin.z, 1), mwvp);

    GSOutput output;
    output.coverage = value;

    //+X
    output.position = position[0];
    triStream.Append(output);
    output.position = position[2];
    triStream.Append(output);
    output.position = position[1];
    triStream.Append(output);
    output.position = position[3];
    triStream.Append(output);
    triStream.RestartStrip();

    //-X
    output.position = position[7];
    triStream.Append(output);
    output.position = position[6];
    triStream.Append(output);
    output.position = position[5];
    triStream.Append(output);
    output.position = position[4];
    triStream.Append(output);
    triStream.RestartStrip();

    //+Y
    output.position = position[0];
    triStream.Append(output);
    output.position = position[1];
    triStream.Append(output);
    output.position = position[4];
    triStream.Append(output);
    output.position = position[5];
    triStream.Append(output);
    triStream.RestartStrip();

    //-Y
    output.position = position[2];
    triStream.Append(output);
    output.position = position[6];
    triStream.Append(output);
    output.position = position[3];
    triStream.Append(output);
    output.position = position[7];
    triStream.Append(output);
    triStream.RestartStrip();

    //+Z
    output.position = position[0];
    triStream.Append(output);
    output.position = position[4];
    triStream.Append(output);
    output.position = position[2];
    triStream.Append(output);
    output.position = position[6];
    triStream.Append(output);
    triStream.RestartStrip();

    //-Z
    output.position = position[7];
    triStream.Append(output);
    output.position = position[5];
    triStream.Append(output);
    output.position = position[3];
    triStream.Append(output);
    output.position = position[1];
    triStream.Append(output);
    triStream.RestartStrip();
}
