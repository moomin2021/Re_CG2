#include "Vertex3D.h"

// --コンストラクタ-- //
Vertex3D::Vertex3D() {

}

// --初期化処理-- //
void Vertex3D::Initialize(ID3D12Device* device) {
	// --関数が成功したかどうかを判別する用変数-- //
	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成 //
	HRESULT result;

	// --頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数-- //
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());

	/// --頂点バッファの確保-- ///
#pragma region

	// --頂点バッファの設定-- //
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	// --リソース設定-- //
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --頂点バッファの生成-- //
	ID3D12Resource* vertBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --法線の計算-- ///
#pragma region

	for (size_t i = 0; i < indices.size() / 3; i++) {
		// --三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		// --三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		// --p0->p1ベクトル、p0->p2ベクトルを計算（ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		// --外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);

		// --正規化
		normal = XMVector3Normalize(normal);

		// --求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}

#pragma endregion
	/// --END-- ///

	/// --頂点バッファへのデータ転送-- ///
#pragma region

	// --GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得-- //
	Vertices* vertMap = nullptr;

	// --Map処理でメインメモリとGPUのメモリを紐づける-- //
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// --全頂点に対して-- //
	for (int i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i]; // 座標をコピー
	}

	// --繋がりを解除-- //
	vertBuff->Unmap(0, nullptr);

#pragma endregion
	/// --END-- ///

	/// --頂点バッファビューの作成-- ///
#pragma region

	// --GPU仮想アドレス-- //
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	// --頂点バッファのサイズ-- //
	vbView.SizeInBytes = sizeVB;

	// --頂点1つ分のデータサイズ-- //
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion
	/// --END-- ///

	// --インデックスデータ全体のサイズ-- //
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());

	// --リソース設定-- //
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --インデックスバッファの生成-- //
	ID3D12Resource* indexBuff = nullptr;
	result = device->CreateCommittedResource(
		&heapProp,// -> ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,// -> リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	// --インデックスバッファをマッピング-- //
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);

	// --全インデックスに対して-- //
	for (size_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];
	}

	// --マッピング解除-- //
	indexBuff->Unmap(0, nullptr);

	// --インデックスバッファビュー作成-- //
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}