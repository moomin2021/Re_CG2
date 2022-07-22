#pragma once
#include "Vertex.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl.h>
using namespace Microsoft::WRL;
using namespace DirectX;

// --�萔�o�b�t�@�p�f�[�^�\����-- //
struct ConstBufferDataTransform
{
	// 3D�ϊ��s��
	XMMATRIX mat;
};

class Object3D {

private:
	// --�萔�o�b�t�@�i�s��p�j
	ID3D12Resource * constBuffTransform;

	// --�萔�o�b�t�@�}�b�v�i�s��p�j
	ConstBufferDataTransform * constMapTransform;

	// --���_�f�[�^-- //
	Vertex * vertex;

public:

	// --�A�t�B���ϊ����
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 position;

	// --���[���h�ϊ��s��
	XMMATRIX matWorld;

	// --�e�I�u�W�F�N�g�ւ̃|�C���^
	Object3D* parent;

public:
	// --�R���X�g���N�^-- //
	Object3D();

	// --�f�X�g���N�^-- //
	~Object3D();

	// --����������-- //
	void Initialize(ID3D12Device* device);

	// --�X�V����-- //
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	// --�`�揈��-- //
	void Draw(ID3D12GraphicsCommandList* commandList);
};