
#include "Header.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}
 
VS_OUTPUT main(float4 Pos : POSITION, float2 tex : TEXCOORD0)
{
	VS_OUTPUT output;
	
	output.Pos = mul(Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	output.Texture = tex;
	return output;
}