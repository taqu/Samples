#define USE_EDGE_OFFSET 1

cbuffer ConstantBuffer0 : register(b0)
{
    float4 worldCenter;
    float4 worldSize; //world size, half world size, voxel size, half voxel size
}

struct VSOutput
{
    float4 position : SV_Position;
};

struct GSOutput
{
    float4 position : SV_Position;
    float4 upos : Position;
    float4 clip : Clip;
    int axis : Axis;
};

float2 edgeNormalXY(float3 edge)
{
    return normalize(float2(-edge.y, edge.x));
}

float2 edgeNormalXZ(float3 edge)
{
    return normalize(float2(-edge.z, edge.x));
}

float2 edgeNormalZY(float3 edge)
{
    return normalize(float2(-edge.y, edge.z));
}

[maxvertexcount(3)]
void main(triangle VSOutput input[3], inout TriangleStream<GSOutput> triStream)
{
    float3 pos0 = input[0].position.xyz - worldCenter.xyz;
    float3 pos1 = input[1].position.xyz - worldCenter.xyz;
    float3 pos2 = input[2].position.xyz - worldCenter.xyz;

    float3 edge0 = pos1 - pos0;
    float3 edge1 = pos2 - pos1;
    float3 edge2 = pos0 - pos2;

    float3 normal = cross(edge0, -edge2);
    float3 discr = abs(normal);
    int axis = 2;
    float axisMax = discr.x;
    if(axisMax<discr.y){
        axis = 1;
        axisMax = discr.y;
    }
    if(axisMax<discr.z){
        axis = 0;
    }

#if USE_EDGE_OFFSET
    float3 edgeOffset[3];
    float boundOffset = worldSize.w/worldSize.y;
    edgeOffset[0] = normalize(cross(edge0, normal));
    edgeOffset[1] = normalize(cross(edge1, normal));
    edgeOffset[2] = normalize(cross(edge2, normal));
#endif

    float3 offset = float3(0, 0, 0.5);
    float4 clip;
    if(0 == axis){
        clip.xy = max(pos0.xy, max(pos1.xy, pos2.xy));
        clip.zw = min(pos0.xy, min(pos1.xy, pos2.xy));
        float3 inv = 1/float3(worldSize.y, worldSize.y, -worldSize.x);

        pos0 = pos0.xyz * inv + offset;
        pos1 = pos1.xyz * inv + offset;
        pos2 = pos2.xyz * inv + offset;

#if USE_EDGE_OFFSET
        pos0.xy += (edgeOffset[0].xy + edgeOffset[2].xy) * boundOffset;
        pos1.xy += (edgeOffset[0].xy + edgeOffset[1].xy) * boundOffset;
        pos2.xy += (edgeOffset[1].xy + edgeOffset[2].xy) * boundOffset;
#endif

    }else if(1 == axis){
        clip.xy = max(pos0.xz, max(pos1.xz, pos2.xz));
        clip.zw = min(pos0.xz, min(pos1.xz, pos2.xz));
        float3 inv = 1/float3(worldSize.y, worldSize.y, worldSize.x);

        pos0 = pos0.xzy * inv + offset;
        pos1 = pos1.xzy * inv + offset;
        pos2 = pos2.xzy * inv + offset;

#if USE_EDGE_OFFSET
        pos0.xy += (edgeOffset[0].xz + edgeOffset[2].xz) * boundOffset;
        pos1.xy += (edgeOffset[0].xz + edgeOffset[1].xz) * boundOffset;
        pos2.xy += (edgeOffset[1].xz + edgeOffset[2].xz) * boundOffset;
#endif
    }else{
        clip.xy = max(pos0.zy, max(pos1.zy, pos2.zy));
        clip.zw = min(pos0.zy, min(pos1.zy, pos2.zy));
        float3 inv = 1/float3(worldSize.y, worldSize.y, worldSize.x);

        pos0 = pos0.zyx * inv + offset;
        pos1 = pos1.zyx * inv + offset;
        pos2 = pos2.zyx * inv + offset;

#if USE_EDGE_OFFSET
        pos0.xy += (edgeOffset[0].zy + edgeOffset[2].zy) * boundOffset;
        pos1.xy += (edgeOffset[0].zy + edgeOffset[1].zy) * boundOffset;
        pos2.xy += (edgeOffset[1].zy + edgeOffset[2].zy) * boundOffset;
#endif
    }

    clip.xy += worldSize.w;
    clip.zw -= worldSize.w;
    clip /= worldSize.y;

    GSOutput output;
    output.position = output.upos = float4(pos0, 1);
    output.clip = clip;
    output.axis = axis;
    triStream.Append(output);

    output.position = output.upos = float4(pos1, 1);
    triStream.Append(output);

    output.position = output.upos = float4(pos2, 1);
    triStream.Append(output);

    triStream.RestartStrip();
}
