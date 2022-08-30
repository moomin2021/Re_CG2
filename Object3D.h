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

// --3D�p���_�N���X-- //
#include "Vertex3D.h"

// --DirectX3D�N���X-- //
#include "DXManager.h"

// --�e�N�X�`���N���X-- //
#include "Texture.h"

// --�萔�o�b�t�@�p�f�[�^�\����-- //
struct ConstBufferDataTransform
{
	// 3D�ϊ��s��
	XMMATRIX mat;
	XMMATRIX world;
	//XMFLOAT3 transform;
	//XMMATRIX view;
	//XMMATRIX projection;
};

// --�萔�o�b�t�@�ɑ���f�[�^���܂Ƃ߂��^-- //
struct ConstBufferDataMaterial
{
	// �F�iRGBA�j
	XMFLOAT4 color;
};

class Object {

private:
	// --DirectX3D�N���X-- //
	DXManager* dxMa;

	// --�e�N�X�`���N���X-- //
	Texture* texture;

	// --�萔�o�b�t�@�i�s��p�j
	ComPtr<ID3D12Resource> constBuffTransform;

	// --�萔�o�b�t�@�p�f�[�^�\���i�s��p�j
	ConstBufferDataTransform* constMapTransform;

	// --�萔�o�b�t�@�i�}�e���A���j
	ComPtr<ID3D12Resource> constBuffMaterial;

	// --�萔�o�b�t�@�p�f�[�^�\���i�}�e���A���j
	ConstBufferDataMaterial* constMapMaterial;

	// --���_�f�[�^-- //
	Vertex3D* vertex;

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
	void Initialize();

	// --�X�V����-- //
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	// --���_�f�[�^�ɗ����̂̏���ݒ�-- //
	void CubeSetVertex();

	// --�����̕`�揈��-- //
	void DrawCube(int textureHandle = 0);

	// --���_�f�[�^�ɎO�p�`�̏���ݒ�-- //
	void TriangleSetVertex();

	// --�O�p�`�`�揈��-- //
	void DrawTriangle(int textureHandle = 0);

	// --���_�f�[�^�ɐ��̏���ݒ�-- //
	void LineSetVertex();

	// --���`��-- //
	void DrawLine(int textureHandle = 0);
};