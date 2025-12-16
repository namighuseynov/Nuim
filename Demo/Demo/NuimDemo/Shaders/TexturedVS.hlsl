cbuffer ConstantBuffer : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
    
    float2 uvTiling;
    float2 uvOffset;
};

struct VSIn
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VSOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOut main(VSIn v)
{
    VSOut o;
    float4 wp = mul(float4(v.pos, 1), world);
    float4 vp = mul(wp, view);
    o.pos = mul(vp, proj);
    o.uv = v.uv * uvTiling + uvOffset;
    return o;
}
