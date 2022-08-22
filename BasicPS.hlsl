#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);// -> 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);// ------> 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

//float4 main(VSOutput input) : SV_TARGET
//{
//	float4 texcolor = float4(tex.Sample(smp, input.uv));
//
//	// --�E�����@�����̃��C�g
//	float3 light = normalize(vec);
//	//float3 light = normalize(float3(1, -1, 1));
//
//	// --diffuce(�g�U���ˌ�)��[0, 1]�͈̔͂�Clamp����
//	float diffuse = saturate(dot(-light, input.normal));
//
//	// �A���r�G���g����0.3�Ƃ��Čv�Z
//	float brightness = diffuse + 0.3f;
//
//	// --�P�x��RGB�ɑ�����ďo��
//	return float4(texcolor.rgb * brightness, texcolor.a) * color;
//
//	// ��Z����ƐF���ς��
//	//return float4(input.normal, 1);// -> RGB�����ꂼ��@����XYZ�AA��1�ŏo��
//	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp, input.uv));

	// --�_����-- //
	{
		// --�_�����̕���-- //
		float3 dir = ptLightPos - input.posw;

		// --�_�����̋���-- //
		float len = length(dir);

		// --�g�U-- //
		float colD = saturate(dot(normalize(input.norw), dir));

		// --����-- //
		float colA = saturate(1.0f / (attenuation.x + attenuation.y * len + attenuation.z * len * len));

		// --�g�U�ƌ�������������-- //
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
//    //�_�����̕���
//    dir = float3(10.0f, 0.0f, 1.0f) - input.svpos.xyz;
//
//    //�_�����̋���
//    len = length(dir);
//
//    //�_�����̕�����normalize
//    dir = dir / len;
//
//    //�g�U
//    colD = saturate(dot(normalize(input.normal), dir));
//    //����
//    colA = saturate(1.0f / (Attenuation.x + Attenuation.y * len + Attenuation.z * len * len));
//
//    col = colD * colA;
//    return float4(col, col, col, 1.0f);
//}