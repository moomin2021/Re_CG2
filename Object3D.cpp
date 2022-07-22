#include "Object3D.h"

// --�R���X�g���N�^-- //
Object3D::Object3D() : constBuffTransform(nullptr), constMapTransform(nullptr),
scale{ 1.0f, 1.0f, 1.0f }, rotation{}, position{}, matWorld{}, parent(nullptr)
{
	vertex = new Vertex();
}

// --�f�X�g���N�^-- //
Object3D::~Object3D() {}

// --����������-- //
void Object3D::Initialize(ID3D12Device* device) {
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
	result = device->CreateCommittedResource(
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

	vertex->Initialize(device);

#pragma endregion
	/// --END-- ///
}

// --�X�V����-- //
void Object3D::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
	XMMATRIX matScale, matRot, matTrans;

	// --�X�P�[���A��]�A���s�ړ��s��̌v�Z-- //
	{
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(rotation.z);
		matRot *= XMMatrixRotationX(rotation.x);
		matRot *= XMMatrixRotationY(rotation.y);
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
	constMapTransform->mat = matWorld * matView * matProjection;
}

// --�`�揈��-- //
void Object3D::Draw(ID3D12GraphicsCommandList* commandList) {
	// --���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vertex->vbView);

	// --�C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&vertex->ibView);

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --�`��R�}���h
	commandList->DrawIndexedInstanced(vertex->indices.size(), 1, 0, 0, 0);
}