#pragma once

// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --���w�֐�-- //
#include <DirectXMath.h>
using namespace DirectX;

// --DirectXTex-- //
#include <DirectXteX.h>

 // --ComPtr�p-- //
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
	ID3D12Device* device;

	// --SRV�q�[�v�̐擪�n���h�����擾-- //
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	// --�ǂݍ��މ摜�������ڂ�-- //
	UINT imageCount;

/// --�����o�ϐ�END-- ///
///---------------- ///
/// --�����o�֐�-- ///
public:
	// --�C���X�^���X�ǂݍ���-- //
	static Texture* GetInstance();

	// --�C���X�^���X���-- //
	void Relese();

	// --����������-- //
	void Initialize(ID3D12Device* device);

	// --�e�N�X�`���̓ǂݍ���-- //
	int LoadTexture(const wchar_t* szFile);

private:
	// --�R���X�g���N�^-- //
	Texture();

/// --�����o�֐�END-- ///
};