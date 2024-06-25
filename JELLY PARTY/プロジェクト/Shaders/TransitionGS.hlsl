#include "Transition.hlsli"

static const unsigned int vnum = 4;

//wが0だと行列計算がおかしくなるので1に変更
static const float4 offset_array[vnum] =
{
	float4( 0.0f, 0.0f, 0.0f, 1.0f),	// 左上
	float4( 1.0f, 0.0f, 0.0f, 1.0f),	// 右上
	float4( 0.0f, 1.0f, 0.0f, 1.0f),	// 左下
	float4( 1.0f, 1.0f, 0.0f, 1.0f),	// 右下
};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{
	for (uint i = 0; i < vnum; i++)
	{
		//ジオメトリ出力
		PS_INPUT element;
		
		// 頂点配置を決める
		element.Pos.xy = offset_array[i].xy * 2.0f - float2(1.0f, 1.0f);

		// 平面なのでzは0
		element.Pos.zw = float2(0.0f, 1.0f);

		// 色情報
		element.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		element.Tex = offset_array[i].xy;

		output.Append(element);
	}
	output.RestartStrip();
}
