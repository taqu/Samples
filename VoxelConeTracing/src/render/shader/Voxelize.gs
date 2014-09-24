struct VSOutput
{
    float4 position : SV_Position;
};

struct GSOutput
{
    float4 position : SV_Position;
    float3 screenPos : TEXCOORD0;
    float3 normal : TEXCOORD1;
    int axis : TEXCOORD2;
};

[maxvertexcount(3)]
void main(triangle VSOutput input[3], inout TriangleStream<GSOutput> triStream)
{
    float3 pos0 = input[0].position.xyz;
    float3 pos1 = input[1].position.xyz;
    float3 pos2 = input[2].position.xyz;

    float3 edge0 = pos1 - pos0;
    float3 edge1 = pos2 - pos1;
    float3 edge2 = pos2 - pos0;

    float3 normal = normalize(cross(edge0, edge2));
    float3 discr = abs(normal);
    int axis = 0;
    float axisMax = discr.x;
    if(axisMax<discr.y){
        axis = 1;
        axisMax = discr.y;
    }
    if(axisMax<discr.z){
        axis = 2;
    }

    GSOutput output;
    output.normal = normal;
    output.axis = axis;

    if(0==axis){
        for(int i=0; i<3; ++i){
            output.position.zw = input[i].position.xw;
            output.position.xy = input[i].position.zy * 2 - float2(1,1);
            output.screenPos = input[i].position.xyz;
            triStream.Append(output);
        }
        triStream.RestartStrip();

    }else if(1==axis){
        for(int i=0; i<3; ++i){
            output.position.zw = input[i].position.yw;
            output.position.xy = input[i].position.xz * 2 - float2(1,1);
            output.screenPos = input[i].position.xyz;
            triStream.Append(output);
        }
        triStream.RestartStrip();

    }else{
        for(int i=0; i<3; ++i){
            output.position.zw = input[i].position.zw;
            output.position.xy = input[i].position.xy * 2 - float2(1,1);
            output.screenPos = input[i].position.xyz;
            triStream.Append(output);
        }
        triStream.RestartStrip();
    }
}
