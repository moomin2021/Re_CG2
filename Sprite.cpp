#include "Sprite.h"

// --DirectX3D�N���X-- //
#include "DXManager.h"

// --Window�N���X-- //
#include "Window.h"

// --�e�N�X�`���N���X-- //
#include "Texture.h"

// --�R���X�g���N�^-- //
Sprite::Sprite() : vbView{}, ibView{}, constBuff(nullptr), constMap(nullptr), vertBuff(nullptr), vertMap(nullptr),
	color{1.0f, 1.0f, 1.0f, 1.0f}, scale{1.0f, 1.0f} {}

// --����������-- //
void Sprite::Initialize() {
	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
	HRESULT result;

	// --���_�f�[�^-- //
	vertices[0] = { {   0.0f, 100.0f * scale.y, 0.0f }, {0.0f, 1.0f} };// -> ����
	vertices[1] = { {   0.0f,   0.0f, 0.0f }, {0.0f, 0.0f} };// -> ����
	vertices[2] = { { 100.0f * scale.x, 100.0f * scale.y, 0.0f }, {1.0f, 1.0f} };// -> �E��
	vertices[3] = { { 100.0f * scale.x,   0.0f, 0.0f }, {1.0f, 0.0f} };// -> �E��

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	// --���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	/// --���_�o�b�t�@�̊m��-- ///
#pragma region

	// --���_�o�b�t�@�̐ݒ�-- //
	D3D12_HEAP_PROPERTIES heapVSProp{}; // �q�[�v�ݒ�
	heapVSProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

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
	result = DXManager::GetDevice()->CreateCommittedResource(
		&heapVSProp, // �q�[�v�ݒ�
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
	//Vertices* vertMap = nullptr;

	// --Map�����Ń��C����������GPU�̃�������R�Â���-- //
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// --�S���_�ɑ΂���-- //
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}

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

	// --�C���f�b�N�X�f�[�^�S�̃T�C�Y-- //
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	// --�����[�X�ݒ�-- //
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // -> �C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --�C���f�b�N�X�o�b�t�@�̐���-- //
	ID3D12Resource* indexBuff = nullptr;
	result = DXManager::GetDevice()->CreateCommittedResource(
		&heapVSProp,// -> �q�[�v�ݒ�
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
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}

	// --�}�b�s���O����-- //
	indexBuff->Unmap(0, nullptr);

	// --�C���f�b�N�X�o�b�t�@�r���[�̍쐬-- //
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	/// --�萔�o�b�t�@-- ///
#pragma region

	// --�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// --�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	// --�萔�o�b�t�@�̃}�b�s���O
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	// --�X�v���C�g�̐F��ς���-- //
	constMap->color = color;

	// --���s���e�s��-- //
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)Window::GetWidth(), (float)Window::GetHeight(), 0.0f, 0.0f, 1.0f);

	// --�s�񏉊���-- //
	constMap->mat = XMMatrixIdentity();

#pragma endregion

}

// --�X�V����-- //
void Sprite::Update() {
	// --���[���h�s��X�V-- //
	matWorld = XMMatrixIdentity();

	// --Z����]-- //
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));

	// --���s�ړ�-- //
	matWorld *= XMMatrixTranslation(position.x, position.y, position.z);

	// --�萔�o�b�t�@�̓]��-- //

	// --�s��v�Z-- //
	constMap->mat = matWorld * matProjection;

	// --�F�K�p-- //
	constMap->color = color;

	// --���_�o�b�t�@�]��-- //
	// --���_�f�[�^-- //
	vertices[0] = { {   0.0f, 100.0f * scale.y, 0.0f }, {0.0f, 1.0f} };// -> ����
	vertices[1] = { {   0.0f,   0.0f, 0.0f }, {0.0f, 0.0f} };// -> ����
	vertices[2] = { { 100.0f * scale.x, 100.0f * scale.y, 0.0f }, {1.0f, 1.0f} };// -> �E��
	vertices[3] = { { 100.0f * scale.x,   0.0f, 0.0f }, {1.0f, 0.0f} };// -> �E��

	// --�S���_�ɑ΂���-- //
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
}

// --�`�揈��-- //
void Sprite::Draw(int textureHandle) {
	// --SRV�q�[�v�̃n���h�����擾-- //
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// --�n���h�����w�肳�ꂽ���܂Ői�߂�-- //
	srvGpuHandle.ptr += textureHandle;

	// --�w�肳�ꂽSRV�����[�g�p�����[�^1�Ԃɐݒ�-- //
	DXManager::GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// --���_�o�b�t�@�r���[�̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->IASetVertexBuffers(0, 1, &vbView);

	// --�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	// --�C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->IASetIndexBuffer(&ibView);

	// --�`��R�}���h-- //
	DXManager::GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
}