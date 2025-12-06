cbuffer ConstantBuffer : register(b0)
{
    float4x4 wvp;
}

struct VSInput
{
    float3 position : POSITION;
};

struct VSOutput
{
    float4 position : SV_POSITION;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    float4 position = float4(input.position, 1.0);
    output.position = mul(position, wvp);
    return output;
}