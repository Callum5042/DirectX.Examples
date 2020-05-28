
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

	//
	// Lighting
	// 

	float3 lightDir = -gLightDirection.xyz;

	// Diffuse light
	float4 diffuse_light = saturate(dot(lightDir, pin.Normal)) * gLightDiffuse;

	// Ambient light
	float4 ambient_light = float4(0.4f, 0.4f, 0.4f, 1.0f) * gLightAmbient;

	// Specular lighting
	float3 viewDir = normalize(gEyePosW - pin.Position);
	float3 reflectDir = reflect(-lightDir, pin.Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), gLightSpecular.w);
	float4 specular = float4(spec * gLightSpecular.xyz, 1.0f);

	// Final result
	return (diffuse_light + ambient_light + specular) * diffuse_texture;
} 