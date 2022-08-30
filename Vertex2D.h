#pragma once
// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --���w�֐�-- //
#include <DirectXMath.h>
using namespace DirectX;

// --�V�[�P���X�R���e�i�̈��-- //
#include <vector>

// --���_�f�[�^-- //
struct Vertices {
	// --XYZ���W-- //
	XMFLOAT3 pos;

	// --UV���W-- //
	XMFLOAT2 uv;
};

class Vertex2D {
/// --�����o�ϐ�-- ///
public:
	// --���_�f�[�^-- //
	std::vector<Vertices> vertices;

	// --�C���f�b�N�X-- //
	std::vector<uint16_t> indices;

	// --���_�o�b�t�@�r���[�̍쐬-- //
	D3D12_VERTEX_BUFFER_VIEW vbView;

	// --�C���f�b�N�X�o�b�t�@�r���[�쐬-- //
	D3D12_INDEX_BUFFER_VIEW ibView;

private:

/// --�����o�ϐ�END-- ///
/// --------------- ///
/// --�����o�֐�-- ///
public:
	// --�R���X�g���N�^-- //
	Vertex2D();

	// --����������-- //
	void Initialize();

private:

/// --�����o�֐�END-- ///
};