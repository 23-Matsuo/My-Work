#include "Jelly.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// �摜�\��
	float4 output = tex.Sample(samLinear, input.UV);

    //// �@���x�N�g���̎擾
    //float3 normal = normalize(matWorld);

    //float3 lightDirection = normalize(lightPosition);

    //// ���̕����Ɩ@���x�N�g���̃h�b�g�ς��v�Z
    //float diff = max(0, dot(normalize(normal), lightDirection));

    //// �g�U���̌v�Z
    //float3 diffuseColor = input.Color * diff;

    //return output * float4(diffuseColor, 0.8);


	input.Color.a = 0.95f;

	return output * input.Color;
}