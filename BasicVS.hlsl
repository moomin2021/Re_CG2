#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv:TEXCOORD)
{
	// �s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;

	// ���W�ɍs�����Z
	output.svpos = mul(mat, pos);
	output.uv = uv;
	return output;
}