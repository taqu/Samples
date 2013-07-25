cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 mwvp;
    float4x4 mlocal;
    float4x4 mw;
    float4x4 mlwvp;
}

cbuffer ConstantBuffer1 : register(b1)
{
    float4 info; //{choppy scale, grid length, grid length * 2, 0}
    float4 uvInfo; //{uv scale, uv offset, 0, 0}
}

struct VSInput
{
    float2 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float3 normal : TEXCOORD0;
    float3 worldPos : TEXCOORD1;
    float2 tex0 : TEXCOORD2;
};

Texture2D texDisplace : register(t0);
SamplerState samTex : register(s0);
#if 1
void calcNormalFolding(out float3 normal, float2 uv)
{
    uint w,h;
    texDisplace.GetDimensions(w, h);

    float2 texel = float2(1/(float)w, 1/(float)h);

    float3 left = texDisplace.SampleLevel(samTex, float2(uv.x-texel.x, uv.y), 0).xyz;
    float3 right = texDisplace.SampleLevel(samTex, float2(uv.x+texel.x, uv.y), 0).xyz;

    float3 back = texDisplace.SampleLevel(samTex, float2(uv.x, uv.y-texel.y), 0).xyz;
    float3 front = texDisplace.SampleLevel(samTex, float2(uv.x, uv.y+texel.y), 0).xyz;

    normal = float3(-(right.z - left.z), info.z, -(front.z - back.z));
    //normal = float3(-(right.z - left.z), -(front.z - back.z), info.z);
    //float scale = info.x*info.y;
    //float2 dx = (right.xy - left.xy) * scale;
    //float2 dy = (front.xy - back.xy) * scale;

    //Jacobian
    //float J = (dx.x+1)*(dy.y+1) - dx.y*dy.x;
    //float fold = max(1-J,0);
}
#endif

VSOutput main(VSInput input )
{
    VSOutput output= (VSOutput)0;

    float4 position = float4(input.position.x, 0, input.position.y, 1);
    position = mul(position, mlocal);

    float2 tex0 = position.xz * uvInfo.x + uvInfo.y;

    float3 displacement = texDisplace.SampleLevel(samTex, tex0, 0).rgb;

    position.xyz += displacement;

    float4 outPos;

    //linear-z
    outPos = mul(position, mwvp);
    outPos.z *= outPos.w;
    output.position = outPos;

    float3 normal;
    calcNormalFolding(normal, tex0);
    //calcNormalFolding(normal, float2(0,0));
    output.normal = normalize( mul(normal, (float3x3)mw) );
    output.tex0 = tex0;
    output.worldPos = mul(position, mw).xyz;
    return output;
}
