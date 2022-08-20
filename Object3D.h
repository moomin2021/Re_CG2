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

// --�萔�o�b�t�@�ɑ���f�[�^���܂Ƃ߂��^-- //
struct ConstBufferDataMaterial
{
	// �F�iRGBA�j
	XMFLOAT4 color;
};

class Object {

private:
	// --�萔�o�b�t�@�i�s��p�j
	ComPtr<ID3D12Resource> constBuffTransform;

	// --�萔�o�b�t�@�p�f�[�^�\���i�s��p�j
	ConstBufferDataTransform* constMapTransform;

	// --�萔�o�b�t�@�i�}�e���A���j
	ComPtr<ID3D12Resource> constBuffMaterial;

	// --�萔�o�b�t�@�p�f�[�^�\���i�}�e���A���j
	ConstBufferDataMaterial* constMapMaterial;

	// --���_�f�[�^-- //
	Vertex* vertex;

	// --�f�o�C�X-- //
	ComPtr<ID3D12Device> device;

	// --SRV�p�f�X�N���v�^�q�[�v-- //
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	// --�I�u�W�F�N�g�̌`-- //
	const char* shape;

public:

	// --�A�t�B���ϊ����
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 position;

	// --���[���h�ϊ��s��
	XMMATRIX matWorld;

	// --�I�u�W�F�N�g�̐F-- //
	XMFLOAT4 color;

	// --�e�I�u�W�F�N�g�ւ̃|�C���^
	Object* parent;

public:
	// --�R���X�g���N�^-- //
	Object();

	// --�f�X�g���N�^-- //
	~Object();

	// --����������-- //
	void Initialize(ID3D12Device* device, ID3D12DescriptorHeap* srvHeap);

	// --�X�V����-- //
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	// --���_�f�[�^�ɗ����̂̏���ݒ�-- //
	void CubeSetVertex();

	// --�����̕`�揈��-- //
	void DrawCube(ID3D12GraphicsCommandList* commandList, int textureHandle = 0);

	// --���_�f�[�^�ɎO�p�`�̏���ݒ�-- //
	void TriangleSetVertex();

	// --�O�p�`�`�揈��-- //
	void DrawTriangle(ID3D12GraphicsCommandList* commandList, int textureHandle = 0);

	// --���_�f�[�^�ɐ��̏���ݒ�-- //
	void LineSetVertex();

	// --���`��-- //
	void DrawLine(ID3D12GraphicsCommandList* commandList, int textureHandle = 0);
};