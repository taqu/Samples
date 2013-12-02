#define USE_SHADE (0)

cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 mvp;
    float4 cameraPos;
}

//--------------------------------
struct VSOutput
{
    float3 position : TEXCOORD0;
    float3 size : TEXCOORD1;
    float3 color : TEXCOORD2;
    float3 invSize : TEXCOORD3;
    float3x3 mat : TEXCOORD4;
};

struct GSOutput
{
    float4 position : SV_POSITION;
    float3 color : TEXCOORD0;
    float3 bmin : TEXCOORD1;
    float3 bmax : TEXCOORD2;
    float3 posInVoxel : TEXCOORD3;
    float3 cameraPosInVoxel : TEXCOORD4;
    float3 invSize : TEXCOORD5;
#if USE_SHADE
    float3x3 mat : TEXCOORD6;
#endif
};

[maxvertexcount(18)]
void main(point VSOutput input[1], inout TriangleStream<GSOutput> triStream)
{
    float3 size[8];
    size[0] = float3(-input[0].size.x, -input[0].size.y,  input[0].size.z);
    size[1] = float3( input[0].size.x, -input[0].size.y,  input[0].size.z);
    size[2] = float3(-input[0].size.x,  input[0].size.y,  input[0].size.z);
    size[3] = float3( input[0].size.x,  input[0].size.y,  input[0].size.z);
    size[4] = float3(-input[0].size.x, -input[0].size.y, -input[0].size.z);
    size[5] = float3( input[0].size.x, -input[0].size.y, -input[0].size.z);
    size[6] = float3(-input[0].size.x,  input[0].size.y, -input[0].size.z);
    size[7] = float3( input[0].size.x,  input[0].size.y, -input[0].size.z);

    float3 cameraPosInVoxel = mul(cameraPos.xyz - input[0].position, input[0].mat);

    GSOutput output[8];

    //[unroll(8)]
    for(int j=0; j<8; ++j){
        output[j].position = float4(input[0].position, 1);
        output[j].position.xyz += mul(input[0].mat, size[j]);
        output[j].position = mul(output[j].position, mvp);
        output[j].position.z *= output[j].position.w;
        output[j].color = input[0].color;

        output[j].bmin = size[4];
        output[j].bmax = size[3];
        output[j].posInVoxel = size[j];
        output[j].cameraPosInVoxel = cameraPosInVoxel;
        output[j].invSize = input[0].invSize;
#if USE_SHADE
        output[j].mat = input[0].mat;
#endif
    }

    triStream.Append(output[2]);
    triStream.Append(output[0]);
    triStream.Append(output[3]);
    triStream.Append(output[1]);

    triStream.Append(output[7]);
    triStream.Append(output[5]);

    triStream.Append(output[6]);
    triStream.Append(output[4]);

    triStream.Append(output[2]);
    triStream.Append(output[0]);
    triStream.RestartStrip();


    triStream.Append(output[0]);
    triStream.Append(output[4]);
    triStream.Append(output[1]);
    triStream.Append(output[5]);
    triStream.RestartStrip();

    triStream.Append(output[6]);
    triStream.Append(output[2]);
    triStream.Append(output[7]);
    triStream.Append(output[3]);
    triStream.RestartStrip();

}
