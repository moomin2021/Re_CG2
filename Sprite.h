#pragma once
// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --数学関数-- //
#include <DirectXMath.h>
using namespace DirectX;

// --2D用の頂点データクラス-- //
#include "Vertex2D.h"

// --定数バッファ用データ構造体（マテリアル）-- //
struct ConstBufferData {
	// --色（RBGA）-- //
	XMFLOAT4 color;
};

class Sprite {
/// --メンバ変数-- ///
public:

private:
	// --頂点データ-- //
	Vertices2D vertices[4];

	// --インデックスデータ-- //
	uint16_t indices[6];

	// --頂点バッファビュー-- //
	D3D12_VERTEX_BUFFER_VIEW vbView;

	// --インデックスバッファビュー-- //
	D3D12_INDEX_BUFFER_VIEW ibView;

	// --定数バッファ-- //
	ID3D12Resource* constBuff;

	// --定数バッファマッピング処理用-- //
	ConstBufferData* constMap;

/// --メンバ変数END-- ///
/// --------------- ///
/// --メンバ関数-- ///
public:
	// --コンストラクタ-- //
	Sprite();

	// --初期化処理-- //
	void Initialize();

	// --描画処理-- //
	void Draw(int textureHandle = 0);

private:

/// --メンバ関数END-- ///
};