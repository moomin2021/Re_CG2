#pragma once

// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --���w�֐�-- //
#include <DirectXMath.h>
using namespace DirectX;

// --ComPtr�p-- //
#include <wrl.h>
using namespace Microsoft::WRL;

// --DirectX3D�N���X-- //
#include "DXManager.h"

struct ConstBufferDataDirLight {
	XMFLOAT3 vec;
};

class DirectionalLight {
/// --�����o�ϐ�-- ///
public:
	// --���������̃x�N�g��-- //
	XMFLOAT3 vec;

private:
	// --�C���X�^���X-- //
	static DirectionalLight* myInstance;

	// --DirectX3D�N���X-- //
	DXManager* dxMa;

	// --�萔�o�b�t�@�i���C�g�̍��W�p�j
	ComPtr<ID3D12Resource> constBuffDirLight;

	// --�萔�o�b�t�@�p�f�[�^�\���i���s�����̃x�N�g���j
	ConstBufferDataDirLight* constMapDirLight;

/// --�����o�ϐ�END-- ///
/// --------------- ///
/// �����o�֐�-- ///
public:
	// --�C���X�^���X�ǂݍ���-- //
	static DirectionalLight* GetInstance();

	// --�C���X�^���X���-- //
	void Relese();

	// --����������-- //
	void Initialize();

	// --�X�V����-- //
	void Update();

	// --�`�揈��-- //
	void Draw();

private:
	// --�R���X�g���N�^-- //
	DirectionalLight();

/// --�����o�֐�END-- ///
};