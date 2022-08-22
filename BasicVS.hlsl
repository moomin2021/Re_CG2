#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv:TEXCOORD)
{
	// �s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	//matrix mat;

	//mat = world * view * projection;

	// ���W�ɍs�����Z
	//output.svpos = mul(world, pos);
	//output.svpos = mul(output.svpos, view);
	//output.svpos = mul(output.svpos, projection);
	output.svpos = mul(mat, pos);
	output.normal = normal;
	output.uv = uv;
	//output.norw = mul(normal, world);
	return output;
}