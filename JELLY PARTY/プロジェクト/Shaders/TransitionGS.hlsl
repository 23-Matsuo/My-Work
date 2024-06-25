#include "Transition.hlsli"

static const unsigned int vnum = 4;

//w��0���ƍs��v�Z�����������Ȃ�̂�1�ɕύX
static const float4 offset_array[vnum] =
{
	float4( 0.0f, 0.0f, 0.0f, 1.0f),	// ����
	float4( 1.0f, 0.0f, 0.0f, 1.0f),	// �E��
	float4( 0.0f, 1.0f, 0.0f, 1.0f),	// ����
	float4( 1.0f, 1.0f, 0.0f, 1.0f),	// �E��
};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{
	for (uint i = 0; i < vnum; i++)
	{
		//�W�I���g���o��
		PS_INPUT element;
		
		// ���_�z�u�����߂�
		element.Pos.xy = offset_array[i].xy * 2.0f - float2(1.0f, 1.0f);

		// ���ʂȂ̂�z��0
		element.Pos.zw = float2(0.0f, 1.0f);

		// �F���
		element.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

		element.Tex = offset_array[i].xy;

		output.Append(element);
	}
	output.RestartStrip();
}
