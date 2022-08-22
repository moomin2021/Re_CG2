#include "DirectionalLight.h"

// --インスタンスにNULLを代入-- //
DirectionalLight* DirectionalLight::myInstance = nullptr;

// --インスタンス読み込み-- //
DirectionalLight* DirectionalLight::GetInstance() {
	// --インスタンスが無かったら生成する-- //
	if (myInstance == nullptr) myInstance = new DirectionalLight();

	// --インスタンスを返す-- //
	return myInstance;
}

// --インスタンス解放-- //
void DirectionalLight::Relese() {
	// --インスタンスが無かったら何もせずに終了する-- //
	if (myInstance == nullptr) return;

	// --インスタンス解放-- //
	delete myInstance;
	myInstance = nullptr;
}

// --コンストラクタ-- //
DirectionalLight::DirectionalLight() : vec{}, constBuffDirLight(nullptr), constMapDirLight(nullptr) {
	// --DirectX3Dクラスのインスタンス生成-- //
	dxMa = DXManager::GetInstance();
}

void DirectionalLight::Initialize() {
	HRESULT result;

	// --定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// --定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataDirLight) + 0xff) & ~0xff;
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
		IID_PPV_ARGS(&constBuffDirLight));
	assert(SUCCEEDED(result));

	// --定数バッファのマッピング
	result = constBuffDirLight->Map(0, nullptr, (void**)&constMapDirLight);
	assert(SUCCEEDED(result));
}

// --更新処理-- //
void DirectionalLight::Update() {
	constMapDirLight->vec = vec;
}

// --描画処理-- //
void DirectionalLight::Draw() {
	dxMa->commandList->SetGraphicsRootConstantBufferView(3, constBuffDirLight->GetGPUVirtualAddress());
}