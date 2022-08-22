#include "PointLight.h"

// --�R���X�g���N�^-- //
PointLight::PointLight() : ptLightPos{}, attenuation{}, constBuffPointLight(nullptr), constMapPointLight(nullptr) {
	dxMa = DXManager::GetInstance();
}

// --����������-- //
void PointLight::Initialize() {
	HRESULT result;

	// --�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// --�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataPointLight) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&constBuffPointLight));
	assert(SUCCEEDED(result));

	// --�萔�o�b�t�@�̃}�b�s���O
	result = constBuffPointLight->Map(0, nullptr, (void**)&constMapPointLight);
	assert(SUCCEEDED(result));
}

// --�X�V����-- //
void PointLight::Update() {
	constMapPointLight->ptLightPos = ptLightPos;
	constMapPointLight->attenuation = attenuation;
}

// --�`�揈��-- //
void PointLight::Draw() {
	dxMa->commandList->SetGraphicsRootConstantBufferView(4, constBuffPointLight->GetGPUVirtualAddress());
}