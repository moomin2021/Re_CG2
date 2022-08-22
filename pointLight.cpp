#include "PointLight.h"

// --コンストラクタ-- //
PointLight::PointLight() : ptLightPos{}, attenuation{}, constBuffPointLight(nullptr), constMapPointLight(nullptr) {
	dxMa = DXManager::GetInstance();
}

// --初期化処理-- //
void PointLight::Initialize() {
	HRESULT result;

	// --定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// --定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataPointLight) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --定数バッファの生成
	result = dxMa->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffPointLight));
	assert(SUCCEEDED(result));

	// --定数バッファのマッピング
	result = constBuffPointLight->Map(0, nullptr, (void**)&constMapPointLight);
	assert(SUCCEEDED(result));
}

// --更新処理-- //
void PointLight::Update() {
	constMapPointLight->ptLightPos = ptLightPos;
	constMapPointLight->attenuation = attenuation;
}

// --描画処理-- //
void PointLight::Draw() {
	dxMa->commandList->SetGraphicsRootConstantBufferView(4, constBuffPointLight->GetGPUVirtualAddress());
}