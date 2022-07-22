#include "Object3D.h"

// --コンストラクタ-- //
Object3D::Object3D() : constBuffTransform(nullptr), constMapTransform(nullptr),
scale{ 1.0f, 1.0f, 1.0f }, rotation{}, position{}, matWorld{}, parent(nullptr)
{
	vertex = new Vertex();
}

// --デストラクタ-- //
Object3D::~Object3D() {}

// --初期化処理-- //
void Object3D::Initialize(ID3D12Device* device) {
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
	result = device->CreateCommittedResource(
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

	vertex->Initialize(device);

#pragma endregion
	/// --END-- ///
}

// --更新処理-- //
void Object3D::Update(XMMATRIX& matView, XMMATRIX& matProjection) {
	XMMATRIX matScale, matRot, matTrans;

	// --スケール、回転、平行移動行列の計算-- //
	{
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(rotation.z);
		matRot *= XMMatrixRotationX(rotation.x);
		matRot *= XMMatrixRotationY(rotation.y);
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
	constMapTransform->mat = matWorld * matView * matProjection;
}

// --描画処理-- //
void Object3D::Draw(ID3D12GraphicsCommandList* commandList) {
	// --頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vertex->vbView);

	// --インデックスバッファの設定
	commandList->IASetIndexBuffer(&vertex->ibView);

	// --定数バッファビュー（CBV）の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// --描画コマンド
	commandList->DrawIndexedInstanced(vertex->indices.size(), 1, 0, 0, 0);
}