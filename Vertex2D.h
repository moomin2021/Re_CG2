#pragma once
// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --数学関数-- //
#include <DirectXMath.h>
using namespace DirectX;

// --シーケンスコンテナの一種-- //
#include <vector>

// --頂点データ-- //
struct Vertices {
	// --XYZ座標-- //
	XMFLOAT3 pos;

	// --UV座標-- //
	XMFLOAT2 uv;
};

class Vertex2D {
/// --メンバ変数-- ///
public:
	// --頂点データ-- //
	std::vector<Vertices> vertices;

	// --インデックス-- //
	std::vector<uint16_t> indices;

	// --頂点バッファビューの作成-- //
	D3D12_VERTEX_BUFFER_VIEW vbView;

	// --インデックスバッファビュー作成-- //
	D3D12_INDEX_BUFFER_VIEW ibView;

private:

/// --メンバ変数END-- ///
/// --------------- ///
/// --メンバ関数-- ///
public:
	// --コンストラクタ-- //
	Vertex2D();

	// --初期化処理-- //
	void Initialize();

private:

/// --メンバ関数END-- ///
};