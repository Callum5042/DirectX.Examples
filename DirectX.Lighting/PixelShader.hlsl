
#include "Header.hlsli"

float4 main(PS_INPUT pin) : SV_TARGET
{
	float4 diffuse_texture = gDiffuseMap.Sample(samplerAnisotropic, pin.Texture);

	if (UseAlpha)
	{
		diffuse_texture.a = gOpacityMap.Sample(samplerAnisotropic, pin.Texture);
		clip(diffuse_texture.a - 0.1f);
	}

	float3 lightDir = float3(0.3f, -0.5f, -0.7f);

	float4 finalColour = diffuse_texture;

	finalColour += saturate(dot(lightDir, pin.Normal)) * diffuse_texture;

	return finalColour;
}