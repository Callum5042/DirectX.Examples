
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix Texture;
	int UseAlpha;
}

cbuffer LightBuffer :register(b1)
{
	float4 gLightDiffuse;
	float4 gLightAmbient;
	float4 gLightSpecular;
	float4 gLightDirection;

	float3 gEyePosW;
}

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Texture : TEXCOORD;
};

struct PS_INPUT
{
	float3 Position : POSITION;
	float4 PositionH : SV_POSITION;
	float3 Normal : NORMAL;
	float2 Texture : TEXCOORD;
};

Texture2D gDiffuseMap : register(t0);
Texture2D gOpacityMap : register(t1);

SamplerState samplerAnisotropic : register(s0);