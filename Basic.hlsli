// --�萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j-- //
cbuffer ConstBufferDataMaterial : register(b0)
{
	// --�F�iRGBA�j
	float4 color;
}

// --�萔�o�b�t�@�p�f�[�^�\���i3D�ϊ��s��j-- //
cbuffer ConstBufferDataTransform : register(b1)
{
	// --3D�ϊ��s��
	matrix mat;
}

// --���_�V�F�[�_�[�̏o�͍\����-- //
// --���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����-- //
struct VSOutput
{
	// --�V�X�e���p���_���W
	float4 svpos : SV_POSITION;

	// --�@���x�N�g��
	float3 normal : NORMAL;

	// --uv�l
	float2 uv : TEXCOORD;
};