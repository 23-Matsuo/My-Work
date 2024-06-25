#include "Confetti.hlsli"

static const int vnum = 4;

static const float4 offset_array[vnum] =
{
	float4(-0.05f,  0.05f, 0.0f, 0.0f),	// 左上
	float4( 0.05f,  0.05f, 0.0f, 0.0f),	// 右上
	float4(-0.05f, -0.05f, 0.0f, 0.0f),	// 左下
	float4( 0.05f, -0.05f, 0.0f, 0.0f),	// 右下				   
};

float4x4 CreateRotation(float3 angle)
{
    float4x4 rotationMatrix;
    
    float cosX = cos(angle.x);
    float sinX = sin(angle.x);
    
    float cosY = cos(angle.y);
    float sinY = sin(angle.y);
    
    float cosZ = cos(angle.z);
    float sinZ = sin(angle.z);
    
    // X軸回転行列
    float4x4 rotationX = float4x4(
        1.0, 0.0, 0.0, 0.0,
        0.0, cosX, -sinX, 0.0,
        0.0, sinX, cosX, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    // Y軸回転行列
    float4x4 rotationY = float4x4(
        cosY, 0.0, sinY, 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sinY, 0.0, cosY, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    // Z軸回転行列
    float4x4 rotationZ = float4x4(
        cosZ, -sinZ, 0.0, 0.0,
        sinZ, cosZ, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    // 回転行列作成
    rotationMatrix = mul(rotationZ, mul(rotationY, rotationX));
    
    return rotationMatrix;
}


[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{

	for (int i = 0; i < vnum; i++)
	{
		PS_INPUT element;
		
        float3 rotate = float3(input[0].Color.a, input[0].Tex);
        
        // オフセット
        float3 offset = offset_array[i].xyz;
		
        // 板ポリの回転行列を作成
        float4x4 rotationMatrix = CreateRotation(rotate);
        
        // ワールド行列に掛けた回転行列を作成
        float4x4 rotationWorld = mul(matWorld, rotationMatrix);

        // ポジション計算
        element.Pos = input[0].Pos + mul(offset, rotationWorld);
		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);
	    // 色設定
        element.Color.rgb = input[0].Color.rgb;
        element.Color.a = 1.0f;
		// uv設定
        element.Tex.x = offset.x + 0.1f;
        element.Tex.y = -offset.y + 0.1f;

		
		output.Append(element);
	}
	output.RestartStrip();
}