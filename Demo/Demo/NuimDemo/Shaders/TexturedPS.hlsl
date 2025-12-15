Texture2D gTex : register(t0);
SamplerState gSamp : register(s0);

struct PSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PSIn i) : SV_TARGET
{
    return gTex.Sample(gSamp, i.uv);
}
