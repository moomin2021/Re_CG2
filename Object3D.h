#pragma once
#include <d3d12.h>
#include <DirectXMath.h>

using namespace DirectX;

// --定数バッファ用データ構造体-- //
struct ConstBufferDataTransform
{
	// 3D変換行列
	XMMATRIX mat;
};

class Object3D {

private:
	// --定数バッファ（行列用）
	ID3D12Resource* constBuffTransform;

	// --定数バッファマップ（行列用）
	ConstBufferDataTransform* constMapTransform;

public:

	// --アフィン変換情報
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 position;

	// --ワールド変換行列
	XMMATRIX matWorld;

	// --親オブジェクトへのポインタ
	Object3D* parent;

public:
	// --コンストラクタ-- //
	Object3D();

	// --デストラクタ-- //
	~Object3D();

	// --初期化処理-- //
	void Initialize(ID3D12Device* device);

	// --更新処理-- //
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	// --描画処理-- //
	void Draw(ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
		D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices);
};