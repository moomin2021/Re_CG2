#pragma once
// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --数学関数-- //
#include <DirectXMath.h>
using namespace DirectX;

// --ComPtr用-- //
#include <wrl.h>
using namespace Microsoft::WRL;

// --3D用頂点クラス-- //
#include "Vertex3D.h"

// --DirectX3Dクラス-- //
#include "DXManager.h"

// --テクスチャクラス-- //
#include "Texture.h"

// --定数バッファ用データ構造体-- //
struct ConstBufferDataTransform
{
	// 3D変換行列
	XMMATRIX mat;
	XMMATRIX world;
	//XMFLOAT3 transform;
	//XMMATRIX view;
	//XMMATRIX projection;
};

// --定数バッファに送るデータをまとめた型-- //
struct ConstBufferDataMaterial
{
	// 色（RGBA）
	XMFLOAT4 color;
};

class Object {

private:
	// --DirectX3Dクラス-- //
	DXManager* dxMa;

	// --テクスチャクラス-- //
	Texture* texture;

	// --定数バッファ（行列用）
	ComPtr<ID3D12Resource> constBuffTransform;

	// --定数バッファ用データ構造（行列用）
	ConstBufferDataTransform* constMapTransform;

	// --定数バッファ（マテリアル）
	ComPtr<ID3D12Resource> constBuffMaterial;

	// --定数バッファ用データ構造（マテリアル）
	ConstBufferDataMaterial* constMapMaterial;

	// --頂点データ-- //
	Vertex3D* vertex;

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
	void Initialize();

	// --更新処理-- //
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	// --頂点データに立方体の情報を設定-- //
	void CubeSetVertex();

	// --立方体描画処理-- //
	void DrawCube(int textureHandle = 0);

	// --頂点データに三角形の情報を設定-- //
	void TriangleSetVertex();

	// --三角形描画処理-- //
	void DrawTriangle(int textureHandle = 0);

	// --頂点データに線の情報を設定-- //
	void LineSetVertex();

	// --線描画-- //
	void DrawLine(int textureHandle = 0);
};