#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv:TEXCOORD)
{
	// ピクセルシェーダーに渡す値
	VSOutput output;
	//matrix mat;

	//mat = world * view * projection;

	// 座標に行列を乗算
	//output.svpos = mul(world, pos);
	//output.svpos = mul(output.svpos, view);
	//output.svpos = mul(output.svpos, projection);
	output.svpos = mul(mat, pos);
	output.normal = normal;
	output.uv = uv;
	//output.norw = mul(normal, world);
	return output;
}