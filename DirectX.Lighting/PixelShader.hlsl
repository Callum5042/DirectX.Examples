
#include "Header.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
	// float3 lightDir = float3(-0.577f, 0.577f, -0.577f);
	float3 lightDir = float3(-0.0f, 0.0f, -0.5f);

	float4 finalColor = saturate(dot(lightDir, input.Normal));

	finalColor.a = 1.0f;
	return finalColor;
}