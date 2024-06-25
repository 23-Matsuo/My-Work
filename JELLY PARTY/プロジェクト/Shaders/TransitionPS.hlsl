#include "Transition.hlsli"

Texture2D tex : register(t0);

SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// �^�����Ȕ|���S��
	float4 fade = float4(0.0f, 0.0f, 0.0f, 1.0f);
	// �X���C�h������
	fade.a = lerp(0.0f, fade.a, step(input.Tex.y, slidePos.y));

	return fade;
}