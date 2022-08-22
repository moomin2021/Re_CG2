#include "DirectionalLight.h"

// --�C���X�^���X��NULL����-- //
DirectionalLight* DirectionalLight::myInstance = nullptr;

// --�C���X�^���X�ǂݍ���-- //
DirectionalLight* DirectionalLight::GetInstance() {
	// --�C���X�^���X�����������琶������-- //
	if (myInstance == nullptr) myInstance = new DirectionalLight();

	// --�C���X�^���X��Ԃ�-- //
	return myInstance;
}

// --�C���X�^���X���-- //
void DirectionalLight::Relese() {
	// --�C���X�^���X�����������牽�������ɏI������-- //
	if (myInstance == nullptr) return;

	// --�C���X�^���X���-- //
	delete myInstance;
	myInstance = nullptr;
}

// --�R���X�g���N�^-- //
DirectionalLight::DirectionalLight() : vec{}, constBuffDirLight(nullptr), constMapDirLight(nullptr) {
	// --DirectX3D�N���X�̃C���X�^���X����-- //
	dxMa = DXManager::GetInstance();
}

void DirectionalLight::Initialize() {
	HRESULT result;

	// --�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// --�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataDirLight) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&constBuffDirLight));
	assert(SUCCEEDED(result));

	// --�萔�o�b�t�@�̃}�b�s���O
	result = constBuffDirLight->Map(0, nullptr, (void**)&constMapDirLight);
	assert(SUCCEEDED(result));
}

// --�X�V����-- //
void DirectionalLight::Update() {
	constMapDirLight->vec = vec;
}

// --�`�揈��-- //
void DirectionalLight::Draw() {
	dxMa->commandList->SetGraphicsRootConstantBufferView(3, constBuffDirLight->GetGPUVirtualAddress());
}