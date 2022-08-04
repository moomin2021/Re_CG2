#pragma once
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <DirectXteX.h>

#include <wrl.h>
using namespace Microsoft::WRL;

class Texture {
///  --�����o�ϐ�-- ///
public:
	// --SRV�p�f�X�N���v�^�q�[�v-- //
	ID3D12DescriptorHeap* srvHeap;

private:
	// --�C���X�^���X-- //
	static Texture* myInstance;

	// --�f�o�C�X-- //
	ComPtr<ID3D12Device> device;

	// --SRV�q�[�v�̐擪�n���h�����擾-- //
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	// --�ǂݍ��މ摜�������ڂ�-- //
	UINT imageCount;

/// --�����o�ϐ�END-- ///
///---------------- ///
/// --�����o�֐�-- ///
public:
	// --�R���X�g���N�^-- //
	Texture();

	// --�C���X�^���X�ǂݍ���-- //
	static Texture* GetInstance();

	// --�C���X�^���X���-- //
	void ReleseTexture();

	// --����������-- //
	void Initialize(ID3D12Device* device);

	// --�e�N�X�`���̓ǂݍ���-- //
	int LoadTexture(const wchar_t* szFile);

private:

/// --�����o�֐�END-- ///
};