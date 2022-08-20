#pragma once
#include "Vertex.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl.h>
using namespace Microsoft::WRL;
using namespace DirectX;

// --定数バッファ用データ構造体-- //
struct ConstBufferDataTransform
{
	// 3D変換行列
	XMMATRIX mat;
};

// --定数バッファに送るデータをまとめた型-- //
struct ConstBufferDataMaterial
{
	// 色（RGBA）
	XMFLOAT4 color;
};

class Object {

private:
	// --定数バッファ（行列用）
	ComPtr<ID3D12Resource> constBuffTransform;

	// --定数バッファ用データ構造（行列用）
	ConstBufferDataTransform* constMapTransform;

	// --定数バッファ（マテリアル）
	ComPtr<ID3D12Resource> constBuffMaterial;

	// --定数バッファ用データ構造（マテリアル）
	ConstBufferDataMaterial* constMapMaterial;

	// --頂点データ-- //
	Vertex* vertex;

	// --デバイス-- //
	ComPtr<ID3D12Device> device;

	// --SRV用デスクリプタヒープ-- //
	ComPtr<ID3D12DescriptorHeap> srvHeap;

	// --オブジェクトの形-- //
	const char* shape;

public:

	// --アフィン変換情報
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 position;

	// --ワールド変換行列
	XMMATRIX matWorld;

	// --オブジェクトの色-- //
	XMFLOAT4 color;

	// --親オブジェクトへのポインタ
	Object* parent;

public:
	// --コンストラクタ-- //
	Object();

	// --デストラクタ-- //
	~Object();

	// --初期化処理-- //
	void Initialize(ID3D12Device* device, ID3D12DescriptorHeap* srvHeap);

	// --更新処理-- //
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	// --頂点データに立方体の情報を設定-- //
	void CubeSetVertex();

	// --立方体描画処理-- //
	void DrawCube(ID3D12GraphicsCommandList* commandList, int textureHandle = 0);

	// --頂点データに三角形の情報を設定-- //
	void TriangleSetVertex();

	// --三角形描画処理-- //
	void DrawTriangle(ID3D12GraphicsCommandList* commandList, int textureHandle = 0);

	// --頂点データに線の情報を設定-- //
	void LineSetVertex();

	// --線描画-- //
	void DrawLine(ID3D12GraphicsCommandList* commandList, int textureHandle = 0);
};