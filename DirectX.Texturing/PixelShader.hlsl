
#include "Header.hlsli"

float4 main(PS_INPUT pin) : SV_TARGET
{
	// Apply diffuse texture
	float4 colour = gDiffuseMap.Sample(samplerAnisotropic, pin.Texture);

	// Apply opacity texture map
	if (UseAlpha)
	{
		colour.a = gOpacityMap.Sample(samplerAnisotropic, pin.Texture).a;
		clip(colour.a - 0.1f);
	}

	return colour;
}