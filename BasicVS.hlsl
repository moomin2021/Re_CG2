#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv:TEXCOORD)
{
	// ピクセルシェーダーに渡す値
	VSOutput output;

	// 座標に行列を乗算
	output.svpos = mul(mat, pos);
	output.normal = normal;
	output.uv = uv;
	return output;
}