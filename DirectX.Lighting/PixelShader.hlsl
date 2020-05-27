
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
	float3 lightDir = float3(-0.707f, 0.0f, 0.707f);
	lightDir = -lightDir;

	float4 diffuse_light = saturate(dot(lightDir, pin.Normal));
	float4 ambient_light = float4(0.4f, 0.4f, 0.4f, 1.0f);

	// Specular lighting
	float3 viewDir = normalize(gEyePosW - pin.Position);
	float3 reflectDir = reflect(-lightDir, pin.Normal);

	float specularStrength = 1;
	float3 lightColor = float3(1.0f, 1.0f, 1.0f);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float4 specular = float4(specularStrength * spec * lightColor, 1.0f);

	return (diffuse_light + ambient_light + specular) * diffuse_texture;
} 