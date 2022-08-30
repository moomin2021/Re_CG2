#pragma once
// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --���w�֐�-- //
#include <DirectXMath.h>
using namespace DirectX;

// --2D�p�̒��_�f�[�^�N���X-- //
#include "Vertex2D.h"

// --�萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j-- //
struct ConstBufferData {
	// --�F�iRBGA�j-- //
	XMFLOAT4 color;
};

class Sprite {
/// --�����o�ϐ�-- ///
public:

private:
	// --���_�f�[�^-- //
	Vertices2D vertices[4];

	// --�C���f�b�N�X�f�[�^-- //
	uint16_t indices[6];

	// --���_�o�b�t�@�r���[-- //
	D3D12_VERTEX_BUFFER_VIEW vbView;

	// --�C���f�b�N�X�o�b�t�@�r���[-- //
	D3D12_INDEX_BUFFER_VIEW ibView;

	// --�萔�o�b�t�@-- //
	ID3D12Resource* constBuff;

	// --�萔�o�b�t�@�}�b�s���O�����p-- //
	ConstBufferData* constMap;

/// --�����o�ϐ�END-- ///
/// --------------- ///
/// --�����o�֐�-- ///
public:
	// --�R���X�g���N�^-- //
	Sprite();

	// --����������-- //
	void Initialize();

	// --�`�揈��-- //
	void Draw(int textureHandle = 0);

private:

/// --�����o�֐�END-- ///
};