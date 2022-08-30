#include "Object3D.h"
#include "Util.h"

// --�R���X�g���N�^-- //
Object::Object() : constBuffTransform(nullptr), constMapTransform(nullptr),
constBuffMaterial(nullptr), constMapMaterial(nullptr), vertex(nullptr), dxMa(nullptr), texture(nullptr),
shape(nullptr), scale{ 1.0f, 1.0f, 1.0f }, rotation{}, position{}, matWorld{}, color{ 1.0f, 1.0f, 1.0f, 1.0f }, parent(nullptr)
{

}

// --�f�X�g���N�^-- //
Object::~Object() {}

// --����������-- //
void Object::Initialize() {

	// --���_�N���X-- //
	vertex = new Vertex3D();

	// --DirectX3D�N���X-- //
	dxMa = DXManager::GetInstance();

	// --�e�N�X�`���N���X-- //
	texture = Texture::GetInstance();

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
	result = dxMa->GetDevice()->CreateCommittedResource(
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
		result = dxMa->GetDevice()->CreateCommittedResource(
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
void Object::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
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

		// --�e�I�u�W�F�N�g�������
		if (parent != nullptr) {
			// �e�I�u�W�F�N�g�̃��[���h�s����|����
			matWorld *= parent->matWorld;
		}
	}

	// --�萔�o�b�t�@�փf�[�^�]��-- //
	//constMapTransform->transform = position;
	constMapTransform->world = matWorld;
	constMapTransform->mat = matWorld * matView * matProjection;

	// --�F��K�p-- //
	constMapMaterial->color = color;
}

// --���_�f�[�^�ɗ����̂̏���ݒ�-- //
void Object::CubeSetVertex() {
	/// --�����̂ɂȂ�悤�ɒ��_��ݒ�-- ///
#pragma region

	Vertices vertices[] = {
		// --�O��
		{{ -5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}},// -> ���� 0
		{{ -5.0f,  5.0f, -5.0f}, {}, {0.0f, 0.0f}},// -> ���� 1
		{{  5.0f, -5.0f, -5.0f}, {}, {1.0f, 1.0f}},// -> �E�� 2
		{{  5.0f,  5.0f, -5.0f}, {}, {1.0f, 0.0f}},// -> �E�� 3

		// --���
		{{ -5.0f, -5.0f,  5.0f}, {}, {0.0f, 1.0f}},// -> ���� 4
		{{ -5.0f,  5.0f,  5.0f}, {}, {0.0f, 0.0f}},// -> ���� 5
		{{  5.0f, -5.0f,  5.0f}, {}, {1.0f, 1.0f}},// -> �E�� 6
		{{  5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}},// -> �E�� 7

		// --����
		{{ -5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}},// -> ���� 8
		{{ -5.0f, -5.0f,  5.0f}, {}, {0.0f, 0.0f}},// -> ���� 9
		{{ -5.0f,  5.0f, -5.0f}, {}, {1.0f, 1.0f}},// -> �E�� 10
		{{ -5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}},// -> �E�� 11

		// --�E��
		{{  5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}},// -> ���� 12
		{{  5.0f, -5.0f,  5.0f}, {}, {0.0f, 0.0f}},// -> ���� 13
		{{  5.0f,  5.0f, -5.0f}, {}, {1.0f, 1.0f}},// -> �E�� 14
		{{  5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}},// -> �E�� 15

		// --����
		{{ -5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}},// -> ���� 16
		{{  5.0f, -5.0f, -5.0f}, {}, {0.0f, 0.0f}},// -> ���� 17
		{{ -5.0f, -5.0f,  5.0f}, {}, {1.0f, 1.0f}},// -> �E�� 18
		{{  5.0f, -5.0f,  5.0f}, {}, {1.0f, 0.0f}},// -> �E�� 19

		// --���
		{{ -5.0f,  5.0f, -5.0f}, {}, {0.0f, 1.0f}},// -> ���� 20
		{{  5.0f,  5.0f, -5.0f}, {}, {0.0f, 0.0f}},// -> ���� 21
		{{ -5.0f,  5.0f,  5.0f}, {}, {1.0f, 1.0f}},// -> �E�� 22
		{{  5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}},// -> �E�� 23
	};

	for (size_t i = 0; i < _countof(vertices); i++) {
		vertex->vertices.push_back(vertices[i]);
	}

	uint16_t indices[] = {
		// --�O��
		0, 1, 2,// -> �O�p�`1��
		2, 1, 3,// -> �O�p�`2��

		// --���
		6, 5, 4,// -> �O�p�`3��
		5, 6, 7,// -> �O�p�`4��

		// --����
		8,  9, 10,// -> �O�p�`5��
		11, 10, 9,// -> �O�p�`6��

		// --�E��
		14, 13, 12,// -> �O�p�`8��
		13, 14, 15,// -> �O�p�`7��

		// --����
		16, 17, 18,// -> �O�p�`9��
		19, 18, 17,// -> �O�p�`10��

		// --���
		22, 21, 20,// -> �O�p�`11��
		21, 22, 23,// -> �O�p�`12��
	};

	for (size_t i = 0; i < _countof(indices); i++) {
		vertex->indices.push_back(indices[i]);
	}

	vertex->Initialize(dxMa->GetDevice());

	shape = "Cube";

#pragma endregion
	/// --END-- ///
}

// --�`�揈��-- //
void Object::DrawCube(int textureHandle) {

	// --�����̂�ݒ�-- //
	if (shape != "Cube") CubeSetVertex();

	// --SRV�q�[�v�̃n���h����-- //
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// --�n���h����1�i�߂�-- //
	srvGpuHandle.ptr += textureHandle;

	// --2���ڂ��w�������悤�ɂ���SRV�̃n���h�������[�g�p�����[�^1�Ԃɐݒ�-- //
	DXManager::GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	// --���_�o�b�t�@�̐ݒ�
	DXManager::GetCommandList()->IASetVertexBuffers(0, 1, &vertex->vbView);

	// --�C���f�b�N�X�o�b�t�@�̐ݒ�
	DXManager::GetCommandList()->IASetIndexBuffer(&vertex->ibView);

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --�`��R�}���h
	DXManager::GetCommandList()->DrawIndexedInstanced((UINT)vertex->indices.size(), 1, 0, 0, 0);
}

// --���_�f�[�^�ɎO�p�`�̏���ݒ�-- //
void Object::TriangleSetVertex() {
	/// --�����̂ɂȂ�悤�ɒ��_��ݒ�-- ///
#pragma region

	Vertices vertices[] = {
		// --�O��
		{{ -5.0f, -2.5f, 0.0f}, {}, {0.0f, 1.0f}},// -> ���� 0
		{{ 0.0f,  5.0f, 0.0f}, {}, {0.0f, 0.0f}},// -> �� 1
		{{  5.0f, -2.5f, 0.0f}, {}, {1.0f, 1.0f}},// -> �E�� 2
	};

	for (size_t i = 0; i < _countof(vertices); i++) {
		vertex->vertices.push_back(vertices[i]);
	}

	uint16_t indices[] = {
		// --�O��
		0, 1, 2,// -> �O�p�`1��
	};

	for (size_t i = 0; i < _countof(indices); i++) {
		vertex->indices.push_back(indices[i]);
	}

	vertex->Initialize(dxMa->GetDevice());

	shape = "Triangle";

#pragma endregion
	/// --END-- ///
}

// --�O�p�`�`�揈��-- //
void Object::DrawTriangle(int textureHandle) {
	// --�O�p�`��ݒ�-- //
	if (shape != "Triangle") TriangleSetVertex();

	// --SRV�q�[�v�̃n���h����-- //
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// --�n���h����1�i�߂�-- //
	srvGpuHandle.ptr += textureHandle;

	// --2���ڂ��w�������悤�ɂ���SRV�̃n���h�������[�g�p�����[�^1�Ԃɐݒ�-- //
	DXManager::GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	// --���_�o�b�t�@�̐ݒ�
	DXManager::GetCommandList()->IASetVertexBuffers(0, 1, &vertex->vbView);

	// --�C���f�b�N�X�o�b�t�@�̐ݒ�
	DXManager::GetCommandList()->IASetIndexBuffer(&vertex->ibView);

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --�`��R�}���h
	DXManager::GetCommandList()->DrawIndexedInstanced((UINT)vertex->indices.size(), 1, 0, 0, 0);
}

// --���_�f�[�^�ɐ��̏���ݒ�-- //
void Object::LineSetVertex() {
	/// --�����̂ɂȂ�悤�ɒ��_��ݒ�-- ///
#pragma region

	Vertices vertices[] = {
		// --�O��
		{{ 5.0f, 0.0f, 0.0f}, {}, {0.0f, 1.0f}},// -> �� 0
		{{ -5.0f, 0.0f, 0.0f}, {}, {0.0f, 0.0f}},// -> �E 1
	};

	for (size_t i = 0; i < _countof(vertices); i++) {
		vertex->vertices.push_back(vertices[i]);
	}

	uint16_t indices[] = {
		// --�O��
		0, 1, // -> �O�p�`1��
	};

	for (size_t i = 0; i < _countof(indices); i++) {
		vertex->indices.push_back(indices[i]);
	}

	vertex->Initialize(dxMa->GetDevice());

	shape = "Triangle";

#pragma endregion
	/// --END-- ///
}

// --���`��-- //
void Object::DrawLine(int textureHandle) {
	// --����ݒ�-- //
	if (shape != "Line") TriangleSetVertex();

	// --SRV�q�[�v�̃n���h����-- //
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// --�n���h����1�i�߂�-- //
	srvGpuHandle.ptr += textureHandle;

	// --2���ڂ��w�������悤�ɂ���SRV�̃n���h�������[�g�p�����[�^1�Ԃɐݒ�-- //
	DXManager::GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	// --���_�o�b�t�@�̐ݒ�
	DXManager::GetCommandList()->IASetVertexBuffers(0, 1, &vertex->vbView);

	// --�C���f�b�N�X�o�b�t�@�̐ݒ�
	DXManager::GetCommandList()->IASetIndexBuffer(&vertex->ibView);

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --�`��R�}���h
	DXManager::GetCommandList()->DrawIndexedInstanced((UINT)vertex->indices.size(), 1, 0, 0, 0);
}