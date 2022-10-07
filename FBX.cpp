#include "FBX.h"

// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --DirectX3Dクラス-- //
#include "DXManager.h"

// --3D用頂点データ-- //
#include "Vertex3D.h"

// --便利系関数-- //
#include "Util.h"

// --コンストラクタ-- //
FBX::FBX() : verticesBuff{}, indicesBuff{}, constBuffTransform(nullptr),
constMapTransform(nullptr), constBuffMaterial(nullptr), constMapMaterial(nullptr),
scale{ 1.0f, 1.0f, 1.0f }, rotation{ 0.0f, 0.0f, 0.0f }, position{ 0.0f, 0.0f, 0.0f },
matWorld{}, color{ 1.0f, 1.0f, 1.0f, 1.0f } {}

// --FBXファイル読み込み-- //
bool FBX::Load(const char* fileName)
{
	char filePath[256];
	std::vector<std::string> outMaterialList;
	int len = strlen(fileName);
	int pathTailPoint = 0;

	for (int i = len - 1; i >= 0; i--)
	{
		if (fileName[i] == '/')
		{
			pathTailPoint = i;
			break;
		}
	}

	strncpy_s(filePath, fileName, pathTailPoint + 1);

	if (LoadFbxFile(outMaterialList, filePath, fileName) == false) {
		return false;
	}

	// --頂点バッファ生成-- //
	CreateVertexBuffer();

	// --インデックスバッファ生成-- //
	CreateIndexBuffer();
}

// --初期化処理-- //
void FBX::Initialize() {
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
	result = DXManager::GetDevice()->CreateCommittedResource(
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

	{
		// --ヒープ設定-- //
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;// -> GPUへの転送用

		// --リソース設定-- //
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;// -> 256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// --定数バッファの生成-- //
		result = DXManager::GetDevice()->CreateCommittedResource(
			&cbHeapProp,// -> ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,// -> リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial)
		);

		// --マッピング-- //
		result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
		assert(SUCCEEDED(result));
	}
}

// --更新処理-- //
void FBX::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
	XMMATRIX matScale, matRot, matTrans;

	// --スケール、回転、平行移動行列の計算-- //
	{
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(Util::Degree2Radian(rotation.z));
		matRot *= XMMatrixRotationX(Util::Degree2Radian(rotation.x));
		matRot *= XMMatrixRotationY(Util::Degree2Radian(rotation.y));
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
	}

	// --定数バッファへデータ転送-- //
	//constMapTransform->transform = position;
	constMapTransform->world = matWorld;
	constMapTransform->mat = matWorld * matView * matProjection;

	// --色を適用-- //
	constMapMaterial->color = color;
}

// --描画処理-- //
void FBX::Draw() {

	// --定数バッファビュー（CBV）の設定コマンド
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --定数バッファビュー（CBV）の設定コマンド-- //
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	for (auto index : indicesBuff) {

		// --頂点バッファの設定
		DXManager::GetCommandList()->IASetVertexBuffers(0, 1, vbView[index.first].data());

		// --インデックスバッファの設定
		DXManager::GetCommandList()->IASetIndexBuffer(ibView[index.first].data());

		// --描画コマンド
		DXManager::GetCommandList()->DrawIndexedInstanced((UINT)index.second.size(), 1, 0, 0, 0);
	}
}

// --ファイル読み込み処理-- //
bool FBX::LoadFbxFile(std::vector<std::string>& outMaterialList, const char* filePath, const char* fileName) {
	// --FbxManager作成-- //
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr) return false;

	// --FbxImporter作成-- //
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	if (fbxImporter == nullptr) {
		fbxManager->Destroy();
		return false;
	}

	// --FbxScene作成-- //
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "");
	if (fbxScene == nullptr) {
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}

	// --fileの初期化-- //
	fbxImporter->Initialize(fileName);

	// --sceneにインポート-- //
	fbxImporter->Import(fbxScene);

	// --ポリゴンを三角形にする-- //
	FbxGeometryConverter converter(fbxManager);
	converter.Triangulate(fbxScene, true);

	std::map<std::string, FbxNode*> meshNodeList;

	// --メッシュNodeを探す-- //
	CollectMeshNode(fbxScene->GetRootNode(), meshNodeList);

	// --メッシュ作成-- //
	for (auto data : meshNodeList) {
		CreateMesh(data.first.c_str(), data.second->GetMesh());
	}

	fbxImporter->Destroy();
	fbxScene->Destroy();
	fbxManager->Destroy();

	return true;
}

// --メッシュノードを探す-- //
void FBX::CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list) {
	// --メッシュNodeを探す-- //
	for (int i = 0; i < node->GetNodeAttributeCount(); i++)
	{
		FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);

		// --Attributeがメッシュなら追加-- //
		if (attribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh) {
			list[node->GetName()] = node;
			break;
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++) {
		CollectMeshNode(node->GetChild(i), list);
	}
}

// --メッシュ作成-- //
bool FBX::CreateMesh(const char* nodeName, FbxMesh* mesh) {
	// --頂点バッファの取得-- //
	FbxVector4* vertices = mesh->GetControlPoints();

	// --インデックスバッファの取得-- //
	int* indices = mesh->GetPolygonVertices();

	// --頂点座標の数の取得-- //
	int polygonVertexCount = mesh->GetPolygonVertexCount();

	// GetPolygonVertexCount => 頂点数-- //
	for (int i = 0; i < polygonVertexCount; i++) {
		// --頂点データ-- //
		Vertices vertex;

		// --インデックスバッファから頂点番号を取得-- //
		int index = indices[i];

		// --頂点座標リストから座標を取得する-- //
		vertex.pos.x = (float)-vertices[index][0];
		vertex.pos.y = (float)vertices[index][1];
		vertex.pos.z = (float)vertices[index][2];

		// --追加-- //
		verticesBuff[nodeName].push_back(vertex);
	}

	// --法線-- //
	FbxArray<FbxVector4> normals;

	// --法線リストの取得-- //
	mesh->GetPolygonVertexNormals(normals);

	// --法線設定-- //
	for (int i = 0; i < normals.Size(); i++) {
		verticesBuff[nodeName][i].normal.x = (float)-normals[i][0];
		verticesBuff[nodeName][i].normal.y = (float)-normals[i][1];
		verticesBuff[nodeName][i].normal.z = (float)-normals[i][2];
	}

	// --ポリゴンの数の取得-- //
	int polygonCount = mesh->GetPolygonCount();

	// --ポリゴンの数だけ連番として保存する-- //
	for (int i = 0; i < polygonCount; i++) {
		indicesBuff[nodeName].push_back(i * 3 + 2);
		indicesBuff[nodeName].push_back(i * 3 + 1);
		indicesBuff[nodeName].push_back(i * 3);
	}

	return true;
}

// --頂点バッファ生成-- //
void FBX::CreateVertexBuffer() {
	// --関数が成功したかどうかを判別する用変数-- //
	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成 //
	HRESULT result;

	/// --頂点バッファの確保-- ///
#pragma region
		// --頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数-- //
	UINT sizeVB = /*static_cast<UINT>(sizeof(Vertices) * */verticesBuff.size();

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
	// --頂点バッファ-- //
	ID3D12Resource* vertBuff = nullptr;
	result = DXManager::GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --頂点バッファへのデータ転送-- ///
#pragma region

	// --GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得-- //
	std::vector<Vertices>* vertMap{};

	// --Map処理でメインメモリとGPUのメモリを紐づける-- //
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// --全頂点に対して-- //
	//vertMap[0] = vertexBuffer.second;

	// --繋がりを解除-- //
	vertBuff->Unmap(0, nullptr);

#pragma endregion
	/// --END-- ///

	/// --頂点バッファビューの作成-- ///
#pragma region

	//vbView[vertexBuffer.first].push_back({
	//	// --GPU仮想アドレス-- //
	//	vertBuff->GetGPUVirtualAddress(),

	//	// --頂点バッファのサイズ-- //
	//	sizeVB,

	//	// --頂点1つ分のデータサイズ-- //
	//	sizeof(vertexBuffer.second[0])
	//	});

#pragma endregion
	/// --END-- ///
}

// --インデックスバッファ生成-- //
void FBX::CreateIndexBuffer() {
	// --関数が成功したかどうかを判別する用変数-- //
	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成 //
	HRESULT result;

	for (auto indexBuffer : indicesBuff) {
		// --インデックスデータ全体のサイズ-- //
		UINT sizeIB = static_cast<UINT>(sizeof(indexBuffer.second[0]) * indexBuffer.second.size());

		// --インデックスバッファの設定-- //
		D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

		// --リソース設定-- //
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// --インデックスバッファの生成-- //
		// --インデックスバッファ-- //
		ID3D12Resource* indexBuff = nullptr;
		result = DXManager::GetDevice()->CreateCommittedResource(
			&heapProp,// -> ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&resDesc,// -> リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff)
		);

		// --インデックスバッファをマッピング-- //
		//std::map<std::string, std::vector<UINT>> indexMap;
		std::vector<UINT>* indexMap{};
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);

		// --全インデックスに対して-- //
		indexMap[0] = indexBuffer.second;

		// --マッピング解除-- //
		indexBuff->Unmap(0, nullptr);

		// --インデックスバッファビュー作成-- //
		ibView[indexBuffer.first].push_back({
			indexBuff->GetGPUVirtualAddress(),
			sizeIB,
			DXGI_FORMAT_R16_UINT
			});
	}
}