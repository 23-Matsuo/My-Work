#include "Confetti.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 ret = input.Color;
    ret.a = 1.0f;
	
	return ret;
}