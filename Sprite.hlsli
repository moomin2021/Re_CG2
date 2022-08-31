cbuffer ConstBufferData : register(b0) {
	// --�F�iRBGA�j-- //
	float4 color;

	// --�s��-- //
	matrix mat;
}

// --���_�b�V�F�[�_�[�̏o�͍\����-- //
struct VSOutput {
	// --�V�X�e���p���_���W-- //
	float4 svpos : SV_POSITION;

	// --uv�l-- //
	float2 uv : TEXCOORD;
};