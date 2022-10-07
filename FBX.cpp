#include "FBX.h"

// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --DirectX3D�N���X-- //
#include "DXManager.h"

// --3D�p���_�f�[�^-- //
#include "Vertex3D.h"

// --�֗��n�֐�-- //
#include "Util.h"

// --�R���X�g���N�^-- //
FBX::FBX() : verticesBuff{}, indicesBuff{}, constBuffTransform(nullptr),
constMapTransform(nullptr), constBuffMaterial(nullptr), constMapMaterial(nullptr),
scale{ 1.0f, 1.0f, 1.0f }, rotation{ 0.0f, 0.0f, 0.0f }, position{ 0.0f, 0.0f, 0.0f },
matWorld{}, color{ 1.0f, 1.0f, 1.0f, 1.0f } {}

// --FBX�t�@�C���ǂݍ���-- //
bool FBX::Load(const char* fileName)
{
	char filePath[256];
	std::vector<std::string> outMaterialList;
	int len = strlen(fileName);
	int pathTailPoint = 0;

	for (int i = len - 1; i >= 0; i--)
	{
		if (fileName[i] == '/')
		{
			pathTailPoint = i;
			break;
		}
	}

	strncpy_s(filePath, fileName, pathTailPoint + 1);

	if (LoadFbxFile(outMaterialList, filePath, fileName) == false) {
		return false;
	}

	// --���_�o�b�t�@����-- //
	CreateVertexBuffer();

	// --�C���f�b�N�X�o�b�t�@����-- //
	CreateIndexBuffer();
}

// --����������-- //
void FBX::Initialize() {
	HRESULT result;

	// --�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// --�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --�萔�o�b�t�@�̐���
	result = DXManager::GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	// --�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	{
		// --�q�[�v�ݒ�-- //
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;// -> GPU�ւ̓]���p

		// --���\�[�X�ݒ�-- //
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;// -> 256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// --�萔�o�b�t�@�̐���-- //
		result = DXManager::GetDevice()->CreateCommittedResource(
			&cbHeapProp,// -> �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,// -> ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial)
		);

		// --�}�b�s���O-- //
		result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
		assert(SUCCEEDED(result));
	}
}

// --�X�V����-- //
void FBX::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
	XMMATRIX matScale, matRot, matTrans;

	// --�X�P�[���A��]�A���s�ړ��s��̌v�Z-- //
	{
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(Util::Degree2Radian(rotation.z));
		matRot *= XMMatrixRotationX(Util::Degree2Radian(rotation.x));
		matRot *= XMMatrixRotationY(Util::Degree2Radian(rotation.y));
		matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	}

	// --���[���h�s��̍���-- //
	{
		// --�ό`�̃��Z�b�g
		matWorld = XMMatrixIdentity();

		// --���[���h�s��ɃX�P�[�����O�𔽉f
		matWorld *= matScale;

		// --���[���h�s��ɉ�]�𔽉f
		matWorld *= matRot;

		// --���[���h�s��ɕ��s�ړ��𔽉f
		matWorld *= matTrans;
	}

	// --�萔�o�b�t�@�փf�[�^�]��-- //
	//constMapTransform->transform = position;
	constMapTransform->world = matWorld;
	constMapTransform->mat = matWorld * matView * matProjection;

	// --�F��K�p-- //
	constMapMaterial->color = color;
}

// --�`�揈��-- //
void FBX::Draw() {

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	for (auto index : indicesBuff) {

		// --���_�o�b�t�@�̐ݒ�
		DXManager::GetCommandList()->IASetVertexBuffers(0, 1, vbView[index.first].data());

		// --�C���f�b�N�X�o�b�t�@�̐ݒ�
		DXManager::GetCommandList()->IASetIndexBuffer(ibView[index.first].data());

		// --�`��R�}���h
		DXManager::GetCommandList()->DrawIndexedInstanced((UINT)index.second.size(), 1, 0, 0, 0);
	}
}

// --�t�@�C���ǂݍ��ݏ���-- //
bool FBX::LoadFbxFile(std::vector<std::string>& outMaterialList, const char* filePath, const char* fileName) {
	// --FbxManager�쐬-- //
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr) return false;

	// --FbxImporter�쐬-- //
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr) {
		fbxManager->Destroy();
		return false;
	}

	// --FbxScene�쐬-- //
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "");
	if (fbxScene == nullptr) {
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}

	// --file�̏�����-- //
	fbxImporter->Initialize(fileName);

	// --scene�ɃC���|�[�g-- //
	fbxImporter->Import(fbxScene);

	// --�|���S�����O�p�`�ɂ���-- //
	FbxGeometryConverter converter(fbxManager);
	converter.Triangulate(fbxScene, true);

	std::map<std::string, FbxNode*> meshNodeList;

	// --���b�V��Node��T��-- //
	CollectMeshNode(fbxScene->GetRootNode(), meshNodeList);

	// --���b�V���쐬-- //
	for (auto data : meshNodeList) {
		CreateMesh(data.first.c_str(), data.second->GetMesh());
	}

	fbxImporter->Destroy();
	fbxScene->Destroy();
	fbxManager->Destroy();

	return true;
}

// --���b�V���m�[�h��T��-- //
void FBX::CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list) {
	// --���b�V��Node��T��-- //
	for (int i = 0; i < node->GetNodeAttributeCount(); i++)
	{
		FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);

		// --Attribute�����b�V���Ȃ�ǉ�-- //
		if (attribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh) {
			list[node->GetName()] = node;
			break;
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++) {
		CollectMeshNode(node->GetChild(i), list);
	}
}

// --���b�V���쐬-- //
bool FBX::CreateMesh(const char* nodeName, FbxMesh* mesh) {
	// --���_�o�b�t�@�̎擾-- //
	FbxVector4* vertices = mesh->GetControlPoints();

	// --�C���f�b�N�X�o�b�t�@�̎擾-- //
	int* indices = mesh->GetPolygonVertices();

	// --���_���W�̐��̎擾-- //
	int polygonVertexCount = mesh->GetPolygonVertexCount();

	// GetPolygonVertexCount => ���_��-- //
	for (int i = 0; i < polygonVertexCount; i++) {
		// --���_�f�[�^-- //
		Vertices vertex;

		// --�C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾-- //
		int index = indices[i];

		// --���_���W���X�g������W���擾����-- //
		vertex.pos.x = (float)-vertices[index][0];
		vertex.pos.y = (float)vertices[index][1];
		vertex.pos.z = (float)vertices[index][2];

		// --�ǉ�-- //
		verticesBuff[nodeName].push_back(vertex);
	}

	// --�@��-- //
	FbxArray<FbxVector4> normals;

	// --�@�����X�g�̎擾-- //
	mesh->GetPolygonVertexNormals(normals);

	// --�@���ݒ�-- //
	for (int i = 0; i < normals.Size(); i++) {
		verticesBuff[nodeName][i].normal.x = (float)-normals[i][0];
		verticesBuff[nodeName][i].normal.y = (float)-normals[i][1];
		verticesBuff[nodeName][i].normal.z = (float)-normals[i][2];
	}

	// --�|���S���̐��̎擾-- //
	int polygonCount = mesh->GetPolygonCount();

	// --�|���S���̐������A�ԂƂ��ĕۑ�����-- //
	for (int i = 0; i < polygonCount; i++) {
		indicesBuff[nodeName].push_back(i * 3 + 2);
		indicesBuff[nodeName].push_back(i * 3 + 1);
		indicesBuff[nodeName].push_back(i * 3);
	}

	return true;
}

// --���_�o�b�t�@����-- //
void FBX::CreateVertexBuffer() {
	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
	HRESULT result;

	/// --���_�o�b�t�@�̊m��-- ///
#pragma region
		// --���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��-- //
	UINT sizeVB = /*static_cast<UINT>(sizeof(Vertices) * */verticesBuff.size();

	// --���_�o�b�t�@�̐ݒ�-- //
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	// --���\�[�X�ݒ�-- //
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --���_�o�b�t�@�̐���-- //
	// --���_�o�b�t�@-- //
	ID3D12Resource* vertBuff = nullptr;
	result = DXManager::GetDevice()->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --���_�o�b�t�@�ւ̃f�[�^�]��-- ///
#pragma region

	// --GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾-- //
	std::vector<Vertices>* vertMap{};

	// --Map�����Ń��C����������GPU�̃�������R�Â���-- //
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// --�S���_�ɑ΂���-- //
	//vertMap[0] = vertexBuffer.second;

	// --�q���������-- //
	vertBuff->Unmap(0, nullptr);

#pragma endregion
	/// --END-- ///

	/// --���_�o�b�t�@�r���[�̍쐬-- ///
#pragma region

	//vbView[vertexBuffer.first].push_back({
	//	// --GPU���z�A�h���X-- //
	//	vertBuff->GetGPUVirtualAddress(),

	//	// --���_�o�b�t�@�̃T�C�Y-- //
	//	sizeVB,

	//	// --���_1���̃f�[�^�T�C�Y-- //
	//	sizeof(vertexBuffer.second[0])
	//	});

#pragma endregion
	/// --END-- ///
}

// --�C���f�b�N�X�o�b�t�@����-- //
void FBX::CreateIndexBuffer() {
	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
	HRESULT result;

	for (auto indexBuffer : indicesBuff) {
		// --�C���f�b�N�X�f�[�^�S�̂̃T�C�Y-- //
		UINT sizeIB = static_cast<UINT>(sizeof(indexBuffer.second[0]) * indexBuffer.second.size());

		// --�C���f�b�N�X�o�b�t�@�̐ݒ�-- //
		D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

		// --���\�[�X�ݒ�-- //
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// --�C���f�b�N�X�o�b�t�@�̐���-- //
		// --�C���f�b�N�X�o�b�t�@-- //
		ID3D12Resource* indexBuff = nullptr;
		result = DXManager::GetDevice()->CreateCommittedResource(
			&heapProp,// -> �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&resDesc,// -> ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff)
		);

		// --�C���f�b�N�X�o�b�t�@���}�b�s���O-- //
		//std::map<std::string, std::vector<UINT>> indexMap;
		std::vector<UINT>* indexMap{};
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);

		// --�S�C���f�b�N�X�ɑ΂���-- //
		indexMap[0] = indexBuffer.second;

		// --�}�b�s���O����-- //
		indexBuff->Unmap(0, nullptr);

		// --�C���f�b�N�X�o�b�t�@�r���[�쐬-- //
		ibView[indexBuffer.first].push_back({
			indexBuff->GetGPUVirtualAddress(),
			sizeIB,
			DXGI_FORMAT_R16_UINT
			});
	}
}