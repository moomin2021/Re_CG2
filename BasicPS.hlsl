#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);// -> 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);// ------> 0番スロットに設定されたサンプラー

//float4 main(VSOutput input) : SV_TARGET
//{
//	float4 texcolor = float4(tex.Sample(smp, input.uv));
//
//	// --右下億　向きのライト
//	float3 light = normalize(vec);
//	//float3 light = normalize(float3(1, -1, 1));
//
//	// --diffuce(拡散反射光)を[0, 1]の範囲にClampする
//	float diffuse = saturate(dot(-light, input.normal));
//
//	// アンビエント項を0.3として計算
//	float brightness = diffuse + 0.3f;
//
//	// --輝度をRGBに代入して出力
//	return float4(texcolor.rgb * brightness, texcolor.a) * color;
//
//	// 乗算すると色が変わる
//	//return float4(input.normal, 1);// -> RGBをそれぞれ法線のXYZ、Aを1で出力
//	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp, input.uv));

	// --点光源-- //
	{
		// --点光源の方向-- //
		float3 dir = ptLightPos - input.posw;

		// --点光源の距離-- //
		float len = length(dir);

		// --拡散-- //
		float colD = saturate(dot(normalize(input.norw), dir));

		// --減衰-- //
		float colA = saturate(1.0f / (attenuation.x + attenuation.y * len + attenuation.z * len * len));

		// --拡散と減衰を合成する-- //
		float col = colD * colA;
	}

	return float4(texcolor.rgb * col, texcolor.a) * color;
}

//float4 main(VSOutput input) : SV_TARGET
//{
//    float3 dir;
//    float  len;
//    float  colD;
//    float  colA;
//    float  col;
//
//    //点光源の方向
//    dir = float3(10.0f, 0.0f, 1.0f) - input.svpos.xyz;
//
//    //点光源の距離
//    len = length(dir);
//
//    //点光源の方向をnormalize
//    dir = dir / len;
//
//    //拡散
//    colD = saturate(dot(normalize(input.normal), dir));
//    //減衰
//    colA = saturate(1.0f / (Attenuation.x + Attenuation.y * len + Attenuation.z * len * len));
//
//    col = colD * colA;
//    return float4(col, col, col, 1.0f);
//}