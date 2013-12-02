//--------------------------------
struct VSInput
{
    float4 orientation : POSITION;
    float3 position : TEXCOORD0;
    float3 size : TEXCOORD1;
    float4 color : TEXCOORD2;
};

struct VSOutput
{
    float3 position : TEXCOORD0;
    float3 size : TEXCOORD1;
    float3 color : TEXCOORD2;
    float3 invSize : TEXCOORD3;
    float3x3 mat : TEXCOORD4;
};

void getMatrix(out float3 axisX, out float3 axisY, out float3 axisZ, float4 quaternion)
{
    float3 q2 = 2 * quaternion.yzw * quaternion.yzw; //(xx, yy, zz)
    float3 w2 = 2 * quaternion.xxx * quaternion.yzw; //2*(wx, wy, wz)
    float3 t = 2 * quaternion.yyz * quaternion.zww; //2*(xy, xz, yz)

    axisX = float3(1-q2.y-q2.z, t.x-w2.z, t.y+w2.y);
    axisY = float3(t.x+w2.z, 1-q2.x-q2.z, t.z-w2.x);
    axisZ = float3(t.y-w2.y, t.z+w2.x, 1-q2.x-q2.y);
}

VSOutput main(VSInput input)
{
    VSOutput output= (VSOutput)0;

    output.position = input.position;
    output.size = input.size;
    output.color = input.color.xyz * input.color.w;
    output.invSize = 0.5/input.size;
    getMatrix(output.mat[0], output.mat[1], output.mat[2], input.orientation);

    return output;
}
