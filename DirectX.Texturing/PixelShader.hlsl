
#include "Header.hlsli"

float4 main(PS_INPUT pin) : SV_TARGET
{
	float4 colour = gDiffuseMap.Sample(samplerAnisotropic, pin.Texture);
	clip(colour.a - 0.1f);

	colour.a = gOpacityMap.Sample(samplerAnisotropic, pin.Texture);

	return colour;
}