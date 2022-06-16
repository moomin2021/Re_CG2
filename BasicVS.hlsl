#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv:TEXCOORD)
{
	// ピクセルシェーダーに渡す値
	VSOutput output;
	output.svpos = pos;
	output.uv = uv;
	return output;
}