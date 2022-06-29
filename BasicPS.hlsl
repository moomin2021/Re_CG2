#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);// -> 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);// ------> 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp, input.uv));

	// --右下億　向きのライト
	float3 light = normalize(float3(1, -1, 1));

	// --diffuceを[0, 1]の範囲にClampする
	float diffuse = saturate(dot(-light, input.normal));

	// アンビエント項を0.3として計算
	float brightness = diffuse + 0.3f;

	// --輝度をRGBに代入して出力
	return float4(texcolor.rgb * brightness, texcolor.a) * color;

	// 乗算すると色が変わる
	//return float4(input.normal, 1);// -> RGBをそれぞれ法線のXYZ、Aを1で出力
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}