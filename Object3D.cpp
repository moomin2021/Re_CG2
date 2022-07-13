#include "Object3D.h"

// --�R���X�g���N�^-- //
Object3D::Object3D() : constBuffTransform(nullptr), constMapTransform(nullptr),
scale{ 1.0f, 1.0f, 1.0f }, rotation{}, position{}, matWorld{}, parent(nullptr) {}

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
void Object3D::Draw(ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices) {
	// --���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vbView);

	// --�C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&ibView);

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --�`��R�}���h
	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}