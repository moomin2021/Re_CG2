#pragma once
#include <DirectXMath.h>
//#include <d3dcompiler.h>
#include <d3d12.h>
//#include <dxgi1_6.h>

//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")
#include <vector>
#include <wrl.h>
using namespace Microsoft::WRL;
using namespace DirectX;

struct Vertices {
	// --XYZ���W-- //
	XMFLOAT3 pos;

	// --�@���x�N�g��-- //
	XMFLOAT3 normal;

	// --UV���W-- //
	XMFLOAT2 uv;
};

class Vertex3D {
public:
	// --���_�f�[�^-- //
	std::vector<Vertices> vertices;

	// --�C���f�b�N�X-- //
	std::vector<uint16_t> indices;

	// --���_�o�b�t�@�r���[�̍쐬-- //
	D3D12_VERTEX_BUFFER_VIEW vbView;

	// --�C���f�b�N�X�o�b�t�@�r���[�쐬-- //
	D3D12_INDEX_BUFFER_VIEW ibView;

public:
	// --�R���X�g���N�^-- //
	Vertex3D();

	// --����������-- //
	void Initialize(ID3D12Device * device);
};