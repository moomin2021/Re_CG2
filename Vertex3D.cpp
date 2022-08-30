#include "Vertex3D.h"

// --�R���X�g���N�^-- //
Vertex3D::Vertex3D() {

}

// --����������-- //
void Vertex3D::Initialize(ID3D12Device* device) {
	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
	HRESULT result;

	// --���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��-- //
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	/// --���_�o�b�t�@�̊m��-- ///
#pragma region

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
	ID3D12Resource* vertBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --�@���̌v�Z-- ///
#pragma region

	for (size_t i = 0; i < indices.size() / 3; i++) {
		// --�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		// --�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		// --p0->p1�x�N�g���Ap0->p2�x�N�g�����v�Z�i�x�N�g���̌��Z�j
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		// --�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);

		// --���K��
		normal = XMVector3Normalize(normal);

		// --���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}

#pragma endregion
	/// --END-- ///

	/// --���_�o�b�t�@�ւ̃f�[�^�]��-- ///
#pragma region

	// --GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾-- //
	Vertices* vertMap = nullptr;

	// --Map�����Ń��C����������GPU�̃�������R�Â���-- //
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// --�S���_�ɑ΂���-- //
	for (int i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}

	// --�q���������-- //
	vertBuff->Unmap(0, nullptr);

#pragma endregion
	/// --END-- ///

	/// --���_�o�b�t�@�r���[�̍쐬-- ///
#pragma region

	// --GPU���z�A�h���X-- //
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	// --���_�o�b�t�@�̃T�C�Y-- //
	vbView.SizeInBytes = sizeVB;

	// --���_1���̃f�[�^�T�C�Y-- //
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion
	/// --END-- ///

	// --�C���f�b�N�X�f�[�^�S�̂̃T�C�Y-- //
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	// --���\�[�X�ݒ�-- //
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --�C���f�b�N�X�o�b�t�@�̐���-- //
	ID3D12Resource* indexBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp,// -> �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,// -> ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	// --�C���f�b�N�X�o�b�t�@���}�b�s���O-- //
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);

	// --�S�C���f�b�N�X�ɑ΂���-- //
	for (size_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];
	}

	// --�}�b�s���O����-- //
	indexBuff->Unmap(0, nullptr);

	// --�C���f�b�N�X�o�b�t�@�r���[�쐬-- //
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}