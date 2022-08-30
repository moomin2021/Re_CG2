#include "Object3D.h"
#include "Util.h"

// --コンストラクタ-- //
Object::Object() : constBuffTransform(nullptr), constMapTransform(nullptr),
constBuffMaterial(nullptr), constMapMaterial(nullptr), vertex(nullptr), dxMa(nullptr), texture(nullptr),
shape(nullptr), scale{ 1.0f, 1.0f, 1.0f }, rotation{}, position{}, matWorld{}, color{ 1.0f, 1.0f, 1.0f, 1.0f }, parent(nullptr)
{

}

// --デストラクタ-- //
Object::~Object() {}

// --初期化処理-- //
void Object::Initialize() {

	// --頂点クラス-- //
	vertex = new Vertex3D();

	// --DirectX3Dクラス-- //
	dxMa = DXManager::GetInstance();

	// --テクスチャクラス-- //
	texture = Texture::GetInstance();

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
	result = dxMa->GetDevice()->CreateCommittedResource(
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
		result = dxMa->GetDevice()->CreateCommittedResource(
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
void Object::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
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

		// --親オブジェクトがあれば
		if (parent != nullptr) {
			// 親オブジェクトのワールド行列を掛ける
			matWorld *= parent->matWorld;
		}
	}

	// --定数バッファへデータ転送-- //
	//constMapTransform->transform = position;
	constMapTransform->world = matWorld;
	constMapTransform->mat = matWorld * matView * matProjection;

	// --色を適用-- //
	constMapMaterial->color = color;
}

// --頂点データに立方体の情報を設定-- //
void Object::CubeSetVertex() {
	/// --立方体になるように頂点を設定-- ///
#pragma region

	Vertices vertices[] = {
		// --前面
		{{ -5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}},// -> 左下 0
		{{ -5.0f,  5.0f, -5.0f}, {}, {0.0f, 0.0f}},// -> 左上 1
		{{  5.0f, -5.0f, -5.0f}, {}, {1.0f, 1.0f}},// -> 右下 2
		{{  5.0f,  5.0f, -5.0f}, {}, {1.0f, 0.0f}},// -> 右上 3

		// --後面
		{{ -5.0f, -5.0f,  5.0f}, {}, {0.0f, 1.0f}},// -> 左下 4
		{{ -5.0f,  5.0f,  5.0f}, {}, {0.0f, 0.0f}},// -> 左上 5
		{{  5.0f, -5.0f,  5.0f}, {}, {1.0f, 1.0f}},// -> 右下 6
		{{  5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}},// -> 右上 7

		// --左面
		{{ -5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}},// -> 左下 8
		{{ -5.0f, -5.0f,  5.0f}, {}, {0.0f, 0.0f}},// -> 左上 9
		{{ -5.0f,  5.0f, -5.0f}, {}, {1.0f, 1.0f}},// -> 右下 10
		{{ -5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}},// -> 右上 11

		// --右面
		{{  5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}},// -> 左下 12
		{{  5.0f, -5.0f,  5.0f}, {}, {0.0f, 0.0f}},// -> 左上 13
		{{  5.0f,  5.0f, -5.0f}, {}, {1.0f, 1.0f}},// -> 右下 14
		{{  5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}},// -> 右上 15

		// --下面
		{{ -5.0f, -5.0f, -5.0f}, {}, {0.0f, 1.0f}},// -> 左下 16
		{{  5.0f, -5.0f, -5.0f}, {}, {0.0f, 0.0f}},// -> 左上 17
		{{ -5.0f, -5.0f,  5.0f}, {}, {1.0f, 1.0f}},// -> 右下 18
		{{  5.0f, -5.0f,  5.0f}, {}, {1.0f, 0.0f}},// -> 右上 19

		// --上面
		{{ -5.0f,  5.0f, -5.0f}, {}, {0.0f, 1.0f}},// -> 左下 20
		{{  5.0f,  5.0f, -5.0f}, {}, {0.0f, 0.0f}},// -> 左上 21
		{{ -5.0f,  5.0f,  5.0f}, {}, {1.0f, 1.0f}},// -> 右下 22
		{{  5.0f,  5.0f,  5.0f}, {}, {1.0f, 0.0f}},// -> 右上 23
	};

	for (size_t i = 0; i < _countof(vertices); i++) {
		vertex->vertices.push_back(vertices[i]);
	}

	uint16_t indices[] = {
		// --前面
		0, 1, 2,// -> 三角形1つ目
		2, 1, 3,// -> 三角形2つ目

		// --後面
		6, 5, 4,// -> 三角形3つ目
		5, 6, 7,// -> 三角形4つ目

		// --左面
		8,  9, 10,// -> 三角形5つ目
		11, 10, 9,// -> 三角形6つ目

		// --右面
		14, 13, 12,// -> 三角形8つ目
		13, 14, 15,// -> 三角形7つ目

		// --下面
		16, 17, 18,// -> 三角形9つ目
		19, 18, 17,// -> 三角形10つ目

		// --上面
		22, 21, 20,// -> 三角形11つ目
		21, 22, 23,// -> 三角形12つ目
	};

	for (size_t i = 0; i < _countof(indices); i++) {
		vertex->indices.push_back(indices[i]);
	}

	vertex->Initialize(dxMa->GetDevice());

	shape = "Cube";

#pragma endregion
	/// --END-- ///
}

// --描画処理-- //
void Object::DrawCube(int textureHandle) {

	// --立方体を設定-- //
	if (shape != "Cube") CubeSetVertex();

	// --SRVヒープのハンドルを-- //
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// --ハンドルを1つ進める-- //
	srvGpuHandle.ptr += textureHandle;

	// --2枚目を指し示すようにしたSRVのハンドルをルートパラメータ1番に設定-- //
	DXManager::GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// --定数バッファビュー（CBV）の設定コマンド-- //
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	// --頂点バッファの設定
	DXManager::GetCommandList()->IASetVertexBuffers(0, 1, &vertex->vbView);

	// --インデックスバッファの設定
	DXManager::GetCommandList()->IASetIndexBuffer(&vertex->ibView);

	// --定数バッファビュー（CBV）の設定コマンド
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --描画コマンド
	DXManager::GetCommandList()->DrawIndexedInstanced((UINT)vertex->indices.size(), 1, 0, 0, 0);
}

// --頂点データに三角形の情報を設定-- //
void Object::TriangleSetVertex() {
	/// --立方体になるように頂点を設定-- ///
#pragma region

	Vertices vertices[] = {
		// --前面
		{{ -5.0f, -2.5f, 0.0f}, {}, {0.0f, 1.0f}},// -> 左下 0
		{{ 0.0f,  5.0f, 0.0f}, {}, {0.0f, 0.0f}},// -> 上 1
		{{  5.0f, -2.5f, 0.0f}, {}, {1.0f, 1.0f}},// -> 右下 2
	};

	for (size_t i = 0; i < _countof(vertices); i++) {
		vertex->vertices.push_back(vertices[i]);
	}

	uint16_t indices[] = {
		// --前面
		0, 1, 2,// -> 三角形1つ目
	};

	for (size_t i = 0; i < _countof(indices); i++) {
		vertex->indices.push_back(indices[i]);
	}

	vertex->Initialize(dxMa->GetDevice());

	shape = "Triangle";

#pragma endregion
	/// --END-- ///
}

// --三角形描画処理-- //
void Object::DrawTriangle(int textureHandle) {
	// --三角形を設定-- //
	if (shape != "Triangle") TriangleSetVertex();

	// --SRVヒープのハンドルを-- //
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// --ハンドルを1つ進める-- //
	srvGpuHandle.ptr += textureHandle;

	// --2枚目を指し示すようにしたSRVのハンドルをルートパラメータ1番に設定-- //
	DXManager::GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// --定数バッファビュー（CBV）の設定コマンド-- //
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	// --頂点バッファの設定
	DXManager::GetCommandList()->IASetVertexBuffers(0, 1, &vertex->vbView);

	// --インデックスバッファの設定
	DXManager::GetCommandList()->IASetIndexBuffer(&vertex->ibView);

	// --定数バッファビュー（CBV）の設定コマンド
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --描画コマンド
	DXManager::GetCommandList()->DrawIndexedInstanced((UINT)vertex->indices.size(), 1, 0, 0, 0);
}

// --頂点データに線の情報を設定-- //
void Object::LineSetVertex() {
	/// --立方体になるように頂点を設定-- ///
#pragma region

	Vertices vertices[] = {
		// --前面
		{{ 5.0f, 0.0f, 0.0f}, {}, {0.0f, 1.0f}},// -> 左 0
		{{ -5.0f, 0.0f, 0.0f}, {}, {0.0f, 0.0f}},// -> 右 1
	};

	for (size_t i = 0; i < _countof(vertices); i++) {
		vertex->vertices.push_back(vertices[i]);
	}

	uint16_t indices[] = {
		// --前面
		0, 1, // -> 三角形1つ目
	};

	for (size_t i = 0; i < _countof(indices); i++) {
		vertex->indices.push_back(indices[i]);
	}

	vertex->Initialize(dxMa->GetDevice());

	shape = "Triangle";

#pragma endregion
	/// --END-- ///
}

// --線描画-- //
void Object::DrawLine(int textureHandle) {
	// --線を設定-- //
	if (shape != "Line") TriangleSetVertex();

	// --SRVヒープのハンドルを-- //
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = Texture::GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();

	// --ハンドルを1つ進める-- //
	srvGpuHandle.ptr += textureHandle;

	// --2枚目を指し示すようにしたSRVのハンドルをルートパラメータ1番に設定-- //
	DXManager::GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// --定数バッファビュー（CBV）の設定コマンド-- //
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

	// --頂点バッファの設定
	DXManager::GetCommandList()->IASetVertexBuffers(0, 1, &vertex->vbView);

	// --インデックスバッファの設定
	DXManager::GetCommandList()->IASetIndexBuffer(&vertex->ibView);

	// --定数バッファビュー（CBV）の設定コマンド
	DXManager::GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --描画コマンド
	DXManager::GetCommandList()->DrawIndexedInstanced((UINT)vertex->indices.size(), 1, 0, 0, 0);
}