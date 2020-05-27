
#include "Header.hlsli"

float4 main(PS_INPUT pin) : SV_TARGET
{
	// Apply diffuse texture
	float4 diffuse_texture = gDiffuseMap.Sample(samplerAnisotropic, pin.Texture);

	// Apply opacity texture map
	if (UseAlpha)
	{
		diffuse_texture.a = gOpacityMap.Sample(samplerAnisotropic, pin.Texture).a;
		clip(diffuse_texture.a - 0.1f);
	}

	// Lighting
	float3 lightDir = float3(0.3f, -0.5f, -0.7f);
	float4 diffuse_light = saturate(dot(lightDir, pin.Normal));

	float4 ambient_light = float4(0.4f, 0.4f, 0.4f, 1.0f);

	return (diffuse_light + ambient_light) * diffuse_texture;
}