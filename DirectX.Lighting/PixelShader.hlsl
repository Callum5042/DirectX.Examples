
#include "Header.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 finalColor = 0;

    //do NdotL lighting for 2 lights
    for (int i = 0; i < 2; i++)
    {
        finalColor += saturate(dot((float3)vLightDir[i], input.Norm) * vLightColor[i]);
    }

    finalColor.a = 1;
    return finalColor;
}