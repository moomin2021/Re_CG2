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
struct Vertices2D {
	// --XYZ座標-- //
	XMFLOAT3 pos;

	// --uv座標-- //
	XMFLOAT2 uv;
};

class Vertex2D {
/// --メンバ変数-- ///
public:
	// --頂点データ-- //
	std::vector<Vertices2D> vertices;

	// --インデックスデータ-- //
	std::vector<uint16_t> indices;

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