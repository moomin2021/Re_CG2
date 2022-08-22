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

// --�萔�o�b�t�@-- //
struct ConstBufferDataPointLight {
	// --�������W-- //
	XMFLOAT3 ptLightPos;

	// --���������p�����[�^-- //
	XMFLOAT3 attenuation;
};

class PointLight {
/// --�����o�ϐ�-- ///
public:
	// --�������W-- //
	XMFLOAT3 ptLightPos;

	// --���������p�����[�^�[-- //
	XMFLOAT3 attenuation;

private:
	// --DirectX3D�N���X-- //
	DXManager* dxMa;

	// --�萔�o�b�t�@�i�_�����p�j
	ComPtr<ID3D12Resource> constBuffPointLight;

	// --�萔�o�b�t�@�p�f�[�^�\���i���s�����̃x�N�g���j
	ConstBufferDataPointLight* constMapPointLight;

/// --�����o�ϐ�END-- ///
/// --------------- ///
/// --�����o�֐�-- ///
public:
	// --�R���X�g���N�^-- //
	PointLight();

	// --����������-- //
	void Initialize();

	// --�X�V����-- //
	void Update();

	// --�`�揈��-- //
	void Draw();

private:

/// --�����o�֐�END-- ///
};