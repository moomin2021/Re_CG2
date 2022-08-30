#pragma once
#include <DirectXMath.h>
//#include <d3dcompiler.h>
#include <d3d12.h>
//#include <dxgi1_6.h>

//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")
#include <vector>
#include <wrl.h>
using namespace Microsoft::WRL;
using namespace DirectX;

struct Vertices {
	// --XYZ座標-- //
	XMFLOAT3 pos;

	// --法線ベクトル-- //
	XMFLOAT3 normal;

	// --UV座標-- //
	XMFLOAT2 uv;
};

class Vertex3D {
public:
	// --頂点データ-- //
	std::vector<Vertices> vertices;

	// --インデックス-- //
	std::vector<uint16_t> indices;

	// --頂点バッファビューの作成-- //
	D3D12_VERTEX_BUFFER_VIEW vbView;

	// --インデックスバッファビュー作成-- //
	D3D12_INDEX_BUFFER_VIEW ibView;

public:
	// --コンストラクタ-- //
	Vertex3D();

	// --初期化処理-- //
	void Initialize(ID3D12Device * device);
};