#pragma once
// --FBX SDK-- //
#include <fbxsdk.h>

// --map 連想コンテナの一種-- //
#include <map>

// --文字列-- //
#include <string>

// --シーケンスコンテナの一種-- //
#include <vector>

#include "Object3D.h"

class FBX {
/// --メンバ変数-- ///
public:
	// --アフィン変換情報
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 position;

	// --ワールド変換行列
	XMMATRIX matWorld;

	// --オブジェクトの色-- //
	XMFLOAT4 color;

private:
	// --頂点データ-- //
	std::map<std::string, std::vector<Vertices>> verticesBuff;

	// --インデックスデータ-- //
	std::map<std::string, std::vector<UINT>> indicesBuff;

	// --頂点バッファビューの作成-- //
	//std::vector<D3D12_VERTEX_BUFFER_VIEW> vbView;
	std::map<std::string, std::vector<D3D12_VERTEX_BUFFER_VIEW>> vbView;

	// --インデックスバッファビュー作成-- //
	//std::vector<D3D12_INDEX_BUFFER_VIEW> ibView;
	std::map<std::string, std::vector<D3D12_INDEX_BUFFER_VIEW>> ibView;

	// --定数バッファ（行列用）
	ComPtr<ID3D12Resource> constBuffTransform;

	// --定数バッファ用データ構造（行列用）
	ConstBufferDataTransform* constMapTransform;

	// --定数バッファ（マテリアル）
	ComPtr<ID3D12Resource> constBuffMaterial;

	// --定数バッファ用データ構造（マテリアル）
	ConstBufferDataMaterial* constMapMaterial;

/// --メンバ変数END-- ///
/// --------------- ///
/// --メンバ関数-- ///
public:
	// --コンストラクタ-- //
	FBX();

	// --FBXファイル読み込み-- //
	bool Load(const char* fileName);

	// --初期化処理-- //
	void Initialize();

	// --更新処理-- //
	void Update(XMMATRIX& matView, XMMATRIX& matProjection);

	// --描画処理-- //
	void Draw();

private:
	// --ファイル読み込み処理-- //
	bool LoadFbxFile(std::vector<std::string>& outMaterialList, const char* filePath, const char* fileName);

	// --メッシュノードを探す-- //
	void CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list);

	// --メッシュ作成-- //
	bool CreateMesh(const char* nodeName, FbxMesh* mesh);

	// --頂点バッファ生成-- //
	void CreateVertexBuffer();

	// --インデックスバッファ生成-- //
	void CreateIndexBuffer();

/// --メンバ関数END-- ///
};