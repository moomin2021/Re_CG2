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
struct Vertices2D {
	// --XYZ���W-- //
	XMFLOAT3 pos;

	// --uv���W-- //
	XMFLOAT2 uv;
};

class Vertex2D {
/// --�����o�ϐ�-- ///
public:
	// --���_�f�[�^-- //
	std::vector<Vertices2D> vertices;

	// --�C���f�b�N�X�f�[�^-- //
	std::vector<uint16_t> indices;

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