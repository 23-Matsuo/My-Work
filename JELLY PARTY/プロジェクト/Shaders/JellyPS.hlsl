#include "Jelly.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// 画像表示
	float4 output = tex.Sample(samLinear, input.UV);

    //// 法線ベクトルの取得
    //float3 normal = normalize(matWorld);

    //float3 lightDirection = normalize(lightPosition);

    //// 光の方向と法線ベクトルのドット積を計算
    //float diff = max(0, dot(normalize(normal), lightDirection));

    //// 拡散光の計算
    //float3 diffuseColor = input.Color * diff;

    //return output * float4(diffuseColor, 0.8);


	input.Color.a = 0.95f;

	return output * input.Color;
}