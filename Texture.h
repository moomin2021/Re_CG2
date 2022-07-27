#pragma once
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
//#include <cassert>
//#include <vector>
//#include <string>
#include <DirectXMath.h>
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <DirectXteX.h>

#include <wrl.h>
using namespace Microsoft::WRL;

class Texture {
private:
	// --�f�o�C�X-- //
	ComPtr<ID3D12Device> device;

	// --SRV�q�[�v�̐擪�n���h�����擾-- //
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	// --�ǂݍ��މ摜�������ڂ�-- //
	UINT imageCount;

public:

	// --SRV�p�f�X�N���v�^�q�[�v-- //
	ID3D12DescriptorHeap* srvHeap;

public:
	// --�R���X�g���N�^-- //
	Texture(ComPtr<ID3D12Device> device);

	// --�e�N�X�`���̓ǂݍ���-- //
	int LoadTexture(const wchar_t * szFile);
};