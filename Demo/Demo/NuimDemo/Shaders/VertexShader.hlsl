cbuffer ConstantBuffer : register(b0)
{
    float4x4 world;
    float4x4 view;
    float4x4 proj;
};

struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    float4 pos = float4(input.position, 1.0f);

    // APPLY MATRICES
    float4 worldPos = mul(pos, world);
    float4 viewPos = mul(worldPos, view);
    float4 projPos = mul(viewPos, proj);

    output.position = projPos;
    output.color = input.color;

    return output;
}
