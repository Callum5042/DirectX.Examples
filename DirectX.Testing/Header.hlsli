
Texture2D textureDiffuse : register(t0);

SamplerState linearSampler
{
	Filter = MIG_MAG_MIP_LINEAR;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Texture : TEXCOORD0;
};