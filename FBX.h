#pragma once
// --FBX SDK-- //
#include <fbxsdk.h>

// --map �A�z�R���e�i�̈��-- //
#include <map>

// --������-- //
#include <string>

// --�V�[�P���X�R���e�i�̈��-- //
#include <vector>

#include "Object3D.h"

class FBX {
/// --�����o�ϐ�-- ///
public:
	// --�A�t�B���ϊ����
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 position;

	// --���[���h�ϊ��s��
	XMMATRIX matWorld;

	// --�I�u�W�F�N�g�̐F-- //
	XMFLOAT4 color;

private:
	// --���_�f�[�^-- //
	std::map<std::string, std::vector<Vertices>> verticesBuff;

	// --�C���f�b�N�X�f�[�^-- //
	std::map<std::string, std::vector<UINT>> indicesBuff;

	// --���_�o�b�t�@�r���[�̍쐬-- //
	//std::vector<D3D12_VERTEX_BUFFER_VIEW> vbView;
	std::map<std::string, std::vector<D3D12_VERTEX_BUFFER_VIEW>> vbView;

	// --�C���f�b�N�X�o�b�t�@�r���[�쐬-- //
	//std::vector<D3D12_INDEX_BUFFER_VIEW> ibView;
	std::map<std::string, std::vector<D3D12_INDEX_BUFFER_VIEW>> ibView;

	// --�萔�o�b�t�@�i�s��p�j
	ComPtr<ID3D12Resource> constBuffTransform;

	// --�萔�o�b�t�@�p�f�[�^�\���i�s��p�j
	ConstBufferDataTransform* constMapTransform;

	// --�萔�o�b�t�@�i�}�e���A���j
	ComPtr<ID3D12Resource> constBuffMaterial;

	// --�萔�o�b�t�@�p�f�[�^�\���i�}�e���A���j
	ConstBufferDataMaterial* constMapMaterial;

/// --�����o�ϐ�END-- ///
/// --------------- ///
/// --�����o�֐�-- ///
public:
	// --�R���X�g���N�^-- //
	FBX();

	// --FBX�t�@�C���ǂݍ���-- //
	bool Load(const char* fileName);

	// --����������-- //
	void Initialize();

	// --�X�V����-- //
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	// --�`�揈��-- //
	void Draw();

private:
	// --�t�@�C���ǂݍ��ݏ���-- //
	bool LoadFbxFile(std::vector<std::string>& outMaterialList, const char* filePath, const char* fileName);

	// --���b�V���m�[�h��T��-- //
	void CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list);

	// --���b�V���쐬-- //
	bool CreateMesh(const char* nodeName, FbxMesh* mesh);

	// --���_�o�b�t�@����-- //
	void CreateVertexBuffer();

	// --�C���f�b�N�X�o�b�t�@����-- //
	void CreateIndexBuffer();

/// --�����o�֐�END-- ///
};