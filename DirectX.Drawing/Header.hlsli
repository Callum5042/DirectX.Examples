
cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_INPUT
{
	float3 Position : POSITION;
};

struct PS_INPUT
{
	float3 Position : POSITION;
	float4 PositionH : SV_POSITION;
};