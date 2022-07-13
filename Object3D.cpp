#include "Object3D.h"

// --コンストラクタ-- //
Object3D::Object3D() : constBuffTransform(nullptr), constMapTransform(nullptr),
scale{ 1.0f, 1.0f, 1.0f }, rotation{}, position{}, matWorld{}, parent(nullptr) {}

// --デストラクタ-- //
Object3D::~Object3D() {}

// --初期化処理-- //
void Object3D::Initialize(ID3D12Device* device) {
	HRESULT result;

	// --定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

	// --定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	// --定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));
}

// --更新処理-- //
void Object3D::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
	XMMATRIX matScale, matRot, matTrans;

	// --スケール、回転、平行移動行列の計算-- //
	{
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(rotation.z);
		matRot *= XMMatrixRotationX(rotation.x);
		matRot *= XMMatrixRotationY(rotation.y);
		matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	}

	// --ワールド行列の合成-- //
	{
		// --変形のリセット
		matWorld = XMMatrixIdentity();

		// --ワールド行列にスケーリングを反映
		matWorld *= matScale;

		// --ワールド行列に回転を反映
		matWorld *= matRot;

		// --ワールド行列に平行移動を反映
		matWorld *= matTrans;

		// --親オブジェクトがあれば
		if (parent != nullptr) {
			// 親オブジェクトのワールド行列を掛ける
			matWorld *= parent->matWorld;
		}
	}

	// --定数バッファへデータ転送-- //
	constMapTransform->mat = matWorld * matView * matProjection;
}

// --描画処理-- //
void Object3D::Draw(ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices) {
	// --頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vbView);

	// --インデックスバッファの設定
	commandList->IASetIndexBuffer(&ibView);

	// --定数バッファビュー（CBV）の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --描画コマンド
	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}