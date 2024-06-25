#include "JellySM.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    output.Pos = float4(input.Pos, 1);

    output.Pos = mul(output.Pos, matView);
    output.Pos = mul(output.Pos, matProj);

    output.Color = input.Color;
    output.UV = input.UV;
	
    return output;
}