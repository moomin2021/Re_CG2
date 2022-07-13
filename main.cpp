#include "Input.h"
#include "Window.h"
#include "DXManager.h"
#include "Object3D.h"
#include <wrl.h>
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <DirectXteX.h>

//// --定数バッファ用データ構造体-- //
//struct ConstBufferDataTransform
//{
//	// 3D変換行列
//	XMMATRIX mat;
//};

//// --3Dオブジェクト型-- //
//struct Object3d {
//	// --定数バッファ（行列用）
//	ID3D12Resource* constBuffTransform;
//
//	// --定数バッファマップ（行列用）
//	ConstBufferDataTransform* constMapTransform;
//
//	// --アフィン変換情報
//	XMFLOAT3 scale = { 1, 1, 1 };
//	XMFLOAT3 rotation = { 0, 0, 0 };
//	XMFLOAT3 position = { 0, 0, 0 };
//
//	// --ワールド変換行列
//	XMMATRIX matWorld;
//
//	// --親オブジェクトへのポインタ
//	Object3d* parent = nullptr;
//};
//
//// --3Dオブジェクトの初期化-- //
//void InitializeObject3d(Object3d* object, ID3D12Device* device) {
//	HRESULT result;
//
//	// --定数バッファのヒープ設定
//	D3D12_HEAP_PROPERTIES heapProp{};
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//
//	// --定数バッファのリソース設定
//	D3D12_RESOURCE_DESC resdesc{};
//	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
//	resdesc.Height = 1;
//	resdesc.DepthOrArraySize = 1;
//	resdesc.MipLevels = 1;
//	resdesc.SampleDesc.Count = 1;
//	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	// --定数バッファの生成
//	result = device->CreateCommittedResource(
//		&heapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&resdesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&object->constBuffTransform));
//	assert(SUCCEEDED(result));
//
//	// --定数バッファのマッピング
//	result = object->constBuffTransform->Map(0, nullptr, (void**)&object->constMapTransform);
//	assert(SUCCEEDED(result));
//}
//
//// --3Dオブジェクトの更新処理-- //
//void UpdateObject3d(Object3d* object, XMMATRIX& matView, XMMATRIX& matProjection) {
//	XMMATRIX matScale, matRot, matTrans;
//
//	// --スケール、回転、平行移動行列の計算-- //
//	{
//		matScale = XMMatrixScaling(object->scale.x, object->scale.y, object->scale.z);
//		matRot = XMMatrixIdentity();
//		matRot *= XMMatrixRotationZ(object->rotation.z);
//		matRot *= XMMatrixRotationX(object->rotation.x);
//		matRot *= XMMatrixRotationY(object->rotation.y);
//		matTrans = XMMatrixTranslation(object->position.x, object->position.y, object->position.z);
//	}
//
//	// --ワールド行列の合成-- //
//	{
//		// --変形のリセット
//		object->matWorld = XMMatrixIdentity();
//
//		// --ワールド行列にスケーリングを反映
//		object->matWorld *= matScale;
//
//		// --ワールド行列に回転を反映
//		object->matWorld *= matRot;
//
//		// --ワールド行列に平行移動を反映
//		object->matWorld *= matTrans;
//
//		// --親オブジェクトがあれば
//		if (object->parent != nullptr) {
//			// 親オブジェクトのワールド行列を掛ける
//			object->matWorld *= object->parent->matWorld;
//		}
//	}
//
//	// --定数バッファへデータ転送-- //
//	object->constMapTransform->mat = object->matWorld * matView * matProjection;
//}
//
//// --3Dオブジェクトの描画処理
//void DrawObject3d(Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
//	D3D12_INDEX_BUFFER_VIEW & ibView, UINT numIndices) {
//	// --頂点バッファの設定
//	commandList->IASetVertexBuffers(0, 1, &vbView);
//
//	// --インデックスバッファの設定
//	commandList->IASetIndexBuffer(&ibView);
//
//	// --定数バッファビュー（CBV）の設定コマンド
//	commandList->SetGraphicsRootConstantBufferView(2, object->constBuffTransform->GetGPUVirtualAddress());
//
//	// --描画コマンド
//	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
//}

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// --ウィンドウ生成クラスを生成＆初期化-- //
	ComPtr<Window> win;
	win->WindowInitialize();

	// --DirectX初期化クラスを生成＆初期化-- //
	ComPtr<DXManager> dxMa;
	dxMa->DXInitialize(win->hwnd);

	// --キーボード入力の初期化-- //
	ComPtr<Input> input;
	input->InitializeInput(win->w, win->hwnd);

#pragma endregion

	/////////////////////
	// --描画初期化処理-- //
	/////////////////////

#pragma region

	HRESULT result;

	// --リソース設定-- //
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = win->GetWidth();// ---> レンダーターゲットに合わせる
	depthResourceDesc.Height = win->GetHeight();// -> レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;// -> 深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;// -> デプスステンシル

	// --深度値用ヒーププロパティ-- //
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	// --深度値のクリア設定-- //
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;// -> 深度値1.0f（最大値）でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;// -> 深度値フォーマット

	// --リソース生成-- //
	ComPtr<ID3D12Resource> depthBuff;
	result = dxMa->device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,// -> 深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	);

	// --深度ビュー用デスクリプタヒープ作成-- //
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;// -> 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;// -> デプスステンシルビュー
	ID3D12DescriptorHeap* dsvHeap = nullptr;
	result = dxMa->device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	// --深度ビュー作成-- //
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;// -> 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dxMa->device->CreateDepthStencilView(
		depthBuff.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

	// --定数バッファに送るデータをまとめた型-- //
	struct ConstBufferDataMaterial
	{
		// 色（RGBA）
		XMFLOAT4 color;
	};

	// -- 頂点データ構造体-- //
	struct Vertex
	{
		XMFLOAT3 pos;// ----> xyz座標
		XMFLOAT3 normal;// -> 法線ベクトル
		XMFLOAT2 uv;// -----> uv座標
	};

	ComPtr<ID3D12Resource> constBuffMaterial;
	ComPtr<ConstBufferDataMaterial> constMapMaterial;

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
		result = dxMa->device->CreateCommittedResource(
			&cbHeapProp,// -> ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,// -> リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial)
		);

		// --マッピング-- //
		result = constBuffMaterial->Map(0, nullptr, (void **)&constMapMaterial);
		assert(SUCCEEDED(result));

		// 値を書き込むと自動的に転送される
		constMapMaterial->color = XMFLOAT4(1, 1, 1, 1.0f);// -> RGBAで半透明の赤
	}

	// --3Dオブジェクトの数-- //
	const size_t kObjectCount = 50;

	// 3Dオブジェクトの配列
	Object3D object3ds[kObjectCount];

	{
		// --ヒープ設定-- //
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;// -> GPUへの転送用

		// --リソース設定-- //
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;// -> 256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// --配列内の全オブジェクトに対して-- //
		for (size_t i = 0; i < _countof(object3ds); i++) {
			// --初期化
			object3ds[i].Initialize(dxMa->device.Get());

			// --ここからは親子構造のサンプル
			// 先頭以外なら
			if (i > 0) {
				// --1つ前のオブジェクトを親オブジェクトとする
				object3ds[i].parent = &object3ds[i - 1];

				// --親オブジェクトの9割の大きさ
				object3ds[i].scale = { 0.9f, 0.9f, 0.9f };

				// --親オブジェクトに対してZ軸まわりに30度回転
				object3ds[i].rotation = { 0.0f, 0.0f, XMConvertToRadians(30.0f) };

				// --親オブジェクトに対してZ方向に-8.0ずらす
				object3ds[i].position = { 0.0f, 0.0f, -8.0f };
			}
		}
	}

	// --透視投影行列の計算-- //
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),// -----------> 上下画角45度
		(float)win->GetWidth() / win->GetHeight(),// -> アスペクト比（画面横幅/画面縦幅）
		0.1f, 1000.0f// ------------------------> 前端、奥端
	);

	// --ビュー変換行列-- //
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -100);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	TexMetadata metadata{};
	ScratchImage scratchImg{};

	// --WICテクスチャのロード-- //
	result = LoadFromWICFile(
		L"Resources/mario.jpg",
		WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	TexMetadata metadata2{};
	ScratchImage scratchImg2{};

	// --WICテクスチャのロード-- //
	result = LoadFromWICFile(
		L"Resources/reimu.png",
		WIC_FLAGS_NONE,
		&metadata2, scratchImg2
	);

	ScratchImage mipChain{};

	// --ミップマップ生成-- //
	result = GenerateMipMaps(
		scratchImg.GetImages(),
		scratchImg.GetImageCount(),
		scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain
	);

	ScratchImage mipChain2{};

	// --ミップマップ生成-- //
	result = GenerateMipMaps(
		scratchImg2.GetImages(),
		scratchImg2.GetImageCount(),
		scratchImg2.GetMetadata(),
		TEX_FILTER_DEFAULT,
		0,
		mipChain2
	);

	// --読み込んだディフューズテクスチャをSRGBとして扱う-- //
	metadata.format = MakeSRGB(metadata.format);
	metadata2.format = MakeSRGB(metadata2.format);

	// --ヒープ設定-- //
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// --リソース設定-- //
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	D3D12_RESOURCE_DESC textureResourceDesc2{};
	textureResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc2.Format = metadata2.format;
	textureResourceDesc2.Width = metadata2.width;
	textureResourceDesc2.Height = (UINT)metadata2.height;
	textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResourceDesc2.SampleDesc.Count = 1;

	// --テクスチャバッファの生成-- //
	ComPtr<ID3D12Resource> texBuff;
	result = dxMa->device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	// --テクスチャバッファの生成-- //
	ComPtr<ID3D12Resource> texBuff2;
	result = dxMa->device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2)
	);

	// --全ミップマップについて-- //
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// ミップマップレベルを指定してイメージを取得
		const ComPtr<Image> img = scratchImg.GetImage(i, 0, 0);

		// テクスチャバッファにデータ転送
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,// --------------> 全領域へコピー
			img->pixels,// ----------> 元データアドレス
			(UINT)img->rowPitch,// --> 1ラインサイズ
			(UINT)img->slicePitch// -> 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// --全ミップマップについて-- //
	for (size_t i = 0; i < metadata2.mipLevels; i++)
	{
		// ミップマップレベルを指定してイメージを取得
		const ComPtr<Image> img = scratchImg2.GetImage(i, 0, 0);

		// テクスチャバッファにデータ転送
		result = texBuff2->WriteToSubresource(
			(UINT)i,
			nullptr,// --------------> 全領域へコピー
			img->pixels,// ----------> 元データアドレス
			(UINT)img->rowPitch,// --> 1ラインサイズ
			(UINT)img->slicePitch// -> 1枚サイズ
		);
		assert(SUCCEEDED(result));
	}

	// --SRVの最大個数-- //
	const size_t kMaxSRVCount = 2056;

	// --デスクリプタヒープの設定-- //
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// --設定をもとにSRV用デスクリプタヒープを生成-- //
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	result = dxMa->device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	// --SRVヒープの先頭ハンドルを取得-- //
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// --シェーダリソースビュー設定-- //
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;

	// --ハンドルの指す①にシェーダーリソースビュー作成-- //
	dxMa->device->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

	// --CBV, SRV, UAVの1個分のサイズを取得-- //
	UINT descriptorSize = dxMa->device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// --ハンドルを1つ進める-- //
	srvHandle.ptr += descriptorSize * 1;

	// --シェーダリソースビュー設定-- //
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = textureResourceDesc2.Format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = textureResourceDesc2.MipLevels;

	// --ハンドルの指す①にシェーダーリソースビュー作成-- //
	dxMa->device->CreateShaderResourceView(texBuff2.Get(), &srvDesc2, srvHandle);

	// --ハンドルを1つ進める-- //
	srvHandle.ptr += descriptorSize * 1;

	// --CBV（コンスタントバッファビュー）の設定-- //
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};

	// --定数バッファビュー設定-- //
	dxMa->device->CreateConstantBufferView(&cbvDesc, srvHandle);

	// --デスクリプタレンジの設定-- //
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;// -> 1度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;// -> テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	/// --頂点データ-- ///
#pragma region

	Vertex vertices[] = {
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

#pragma endregion
	/// --END-- ///

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

	// --頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数-- //
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

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
	ID3D12Resource * vertBuff = nullptr;
	result = dxMa->device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///
	
	// --法線の計算-- //
	for (size_t i = 0; i < _countof(indices) / 3; i++) {
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

	/// --頂点バッファへのデータ転送-- ///
#pragma region

	// --GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得-- //
	//Vertex * vertMap = nullptr;
	ComPtr<Vertex> vertMap;

	// --Map処理でメインメモリとGPUのメモリを紐づける-- //
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	assert(SUCCEEDED(result));

	// --全頂点に対して-- //
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap.Get()[i] = vertices[i]; // 座標をコピー
	}

	// --繋がりを解除-- //
	vertBuff->Unmap(0, nullptr);

#pragma endregion
	/// --END-- ///

	/// --頂点バッファビューの作成-- ///
#pragma region

	// --頂点バッファビューの作成-- //
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// --GPU仮想アドレス-- //
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();

	// --頂点バッファのサイズ-- //
	vbView.SizeInBytes = sizeVB;

	// --頂点1つ分のデータサイズ-- //
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion
	/// --END-- ///

	/// --シェーダーの読み込みとコンパイル-- ///
#pragma region

	ID3DBlob * vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob * psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob * errorBlob = nullptr; // エラーオブジェクト

	// --頂点シェーダの読み込みとコンパイル-- //
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// --エラーなら-- //
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// --ピクセルシェーダの読み込みとコンパイル-- //
	result = D3DCompileFromFile(
		L"BasicPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// --エラーなら-- //
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

#pragma endregion
	/// --END-- ///

	/// --頂点レイアウト-- ///
#pragma region

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",// -> セマンティック名
			0,// -> 同じセマンティック名が複数あるときに使うインデックス（0でよい）
			DXGI_FORMAT_R32G32B32_FLOAT,// -> 要素数とビット数を表す（XYZの3つでfloat型なのでR32G32B32_FLOAT）
			0,// -> 入力スロットインデックス（0でよい）
			D3D12_APPEND_ALIGNED_ELEMENT,// -> データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,// -> 入力データの種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
			0// -> 一度に描画するインスタンス数（0でよい）
		},

		{
			"NORMAL",// -> セマンティック名
			0,// -> 同じセマンティック名が複数あるときに使うインデックス（0でよい）
			DXGI_FORMAT_R32G32B32_FLOAT,// -> 要素数とビット数を表す（XYZの3つでfloat型なのでR32G32B32_FLOAT）
			0,// -> 入力スロットインデックス（0でよい）
			D3D12_APPEND_ALIGNED_ELEMENT,// -> データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,// -> 入力データの種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
			0// -> 一度に描画するインスタンス数（0でよい）
		},

		{
			"TEXCOORD",// -> セマンティック名
			0,// -> 同じセマンティック名が複数あるときに使うインデックス（0でよい）
			DXGI_FORMAT_R32G32_FLOAT,// -> 要素数とビット数を表す（XYZの3つでfloat型なのでR32G32B32_FLOAT）
			0,// -> 入力スロットインデックス（0でよい）
			D3D12_APPEND_ALIGNED_ELEMENT,// -> データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,// -> 入力データの種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
			0// -> 一度に描画するインスタンス数（0でよい）
		}, // (1行で書いたほうが見やすい)
	};

#pragma endregion
	/// --END-- ///

	/// --グラフィックスパイプライン-- ///
#pragma region

	// --グラフィックスパイプライン設定-- //
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// --シェーダーの設定-- //
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// --サンプルマスクの設定-- //
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// --ラスタライザの設定-- //
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // 背面を科リング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// --デプスステンシルステートの設定-- //
	pipelineDesc.DepthStencilState.DepthEnable = true;// -> 深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;// -> 書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;// -> 小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;// -> 深度値フォーマット

	// --ブレンドステート-- //
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC & blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// --アルファ値共通設定-- //
	blenddesc.BlendEnable = true;// ----------------> ブレンドを有効する
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;// -> 加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;// ---> ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;// -> デストの値を0%使う

	//// --加算合成-- //
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// -> 加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;// ---> ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;// --> デストの値を100%使う

	//// 減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;// -> デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;// ------------> ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;// -----------> デストの値を100%使う

	//// 色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// ----------> 加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;// -> 1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;// ----------> 使わない

	// 半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// ----------> 加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;// ------> ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// -> 1.0f-ソースのアルファ値

	// --頂点レイアウトの設定-- //
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// --図形の形状設定-- //
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// --その他の設定-- //
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// --ルートパラメータの設定-- //
	D3D12_ROOT_PARAMETER rootParams[3] = {};

	// --定数バッファ0番-- //
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;// --> 定数のバッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;// ------------------> 定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;// -------------------> デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -> 全てのシェーダーから見える

	// -- テクスチャレジスタ0番-- //
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;// -> 種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;// -------> デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;// --------------------> デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -------------> 全てのシェーダから見える

	// --定数バッファ1番-- //
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;// --> 定数のバッファビュー
	rootParams[2].Descriptor.ShaderRegister = 1;// ------------------> 定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;// -------------------> デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -> 全てのシェーダーから見える

	// --テクスチャサンプラーの設定-- //
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> 横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> 縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> 奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;// -> ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;// ------------------> 全てリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;// --------------------------------> ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;// ---------------------------------------------> ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;// -> ピクセルシェーダーからのみ使用可能

	// --ルートシグネチャ-- //
	ID3D12RootSignature * rootSignature;

	// --ルートシグネチャの設定-- //
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;// -------------> ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);// -> ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// --ルートシグネチャのシリアライズ-- //
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dxMa->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	// --パイプラインにルートシグネチャをセット-- //
	pipelineDesc.pRootSignature = rootSignature;

	// --パイプランステートの生成-- //
	ID3D12PipelineState * pipelineState = nullptr;
	result = dxMa->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	// --インデックスデータ全体のサイズ-- //
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	// --リソース設定-- //
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// --インデックスバッファの生成-- //
	ID3D12Resource * indexBuff = nullptr;
	result = dxMa->device->CreateCommittedResource(
		&heapProp,// -> ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,// -> リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	// --インデックスバッファをマッピング-- //
	uint16_t * indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void **)&indexMap);

	// --全インデックスに対して-- //
	for (size_t i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}

	// --マッピング解除-- //
	indexBuff->Unmap(0, nullptr);

	// --インデックスバッファビュー作成-- //
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

#pragma endregion

	// --初期化-- //
	float angle = 0.0f;

	// --スケーリング倍率
	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

	// --回転角
	XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };

	// --座標
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };

	// --ゲームループ-- //
	while (true)
	{
		// --メッセージがあるか-- //
		if (PeekMessage(&win->msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&win->msg); // キー入力メッセージの処理
			DispatchMessage(&win->msg); // プロシージャにメッセージを送る
		}

		// --✖ボタンで終了メッセージが来たらゲームループを抜ける-- //
		if (win->msg.message == WM_QUIT)
		{
			break;
		}

		////////////////////////////
		// --DirectX毎フレーム処理-- //
		////////////////////////////

#pragma region


		// --キーボード入力の更新処理-- //
		input->UpdateInput();

		// --数字の0キーが押されていたら-- //
		if (input->PushKey(DIK_0))
		{
			// 出力ウィンドウに「Hit 0」と表示
			OutputDebugStringA("Hit 0\n");
		}

		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			angle += XMConvertToRadians(input->PushKey(DIK_D) - input->PushKey(DIK_A));
			//if (input->PushKey(DIK_D)) angle += XMConvertToRadians(1.0f);
			//else if (input->PushKey(DIK_A)) angle -= XMConvertToRadians(1.0f);

			eye.x = -100 * sinf(angle);
			eye.z = -100 * cosf(angle);

			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		}

		// --いずれかのキーを押していれば
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{
			//// 座標を移動する処理（Z座標）
			//if      (input->PushKey(DIK_UP)) position.z += 1.0f;
			//else if (input->PushKey(DIK_DOWN)) position.z -= 1.0f;

			//// 座標を移動する処理（X座標）
			//if      (input->PushKey(DIK_RIGHT)) position.x += 1.0f;
			//else if (input->PushKey(DIK_LEFT)) position.x -= 1.0f;

			rotation.z += (input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT));
			rotation.y += (input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN));
		}

		for (size_t i = 0; i < _countof(object3ds); i++) {
			object3ds[i].Update(matView, matProjection);
		}

		//// --ワールド行列に単位行列を代入
		//matWorld0 = XMMatrixIdentity();

		//// --スケーリング行列
		//XMMATRIX matScale;
		//matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

		//// --回転行列
		//XMMATRIX matRot;
		//matRot = XMMatrixIdentity();

		//// Z軸まわりに45度回転
		//matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.x));
		//matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.y));
		//matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.z));

		//// --平行移動行列
		//XMMATRIX matTrans;
		//matTrans = XMMatrixTranslation(position.x, position.y, position.z);

		//// --変形のリセット
		//matWorld0 = XMMatrixIdentity();

		//// --ワールド行列に各要素を反映
		//matWorld0 *= matScale;
		//matWorld0 *= matRot;
		//matWorld0 *= matTrans;

		//// --定数バッファに転送-- //
		//constMapTransform0->mat = matWorld0 * matView * matProjection;
		//constMapTransform1->mat = matWorld1 * matView * matProjection;

		//FLOAT clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // 青っぽい色

		//// --スペースキーが押されていたら-- //
		//if (input->PushKey(DIK_SPACE))
		//{
		//	clearColor[0] = 1.0f;
		//}

		//D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID;

		//if (input->PushKey(DIK_SPACE))
		//{
		//	fillMode = D3D12_FILL_MODE_WIREFRAME;
		//}

		// --バックバッファの番号を取得(2つなので0番か1番)-- //
		UINT bbIndex = dxMa->swapChain->GetCurrentBackBufferIndex();

		/// --1.リソースバリアで書き込み可能に変更-- ///
#pragma region

		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = dxMa->backBuffers[bbIndex].Get(); // バックバッファを指定
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
		dxMa->commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion
		/// --END-- ///

		/// --2.描画先の変更-- ///
#pragma region
		
		// レンダーターゲットビューのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = dxMa->rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * dxMa->device->GetDescriptorHandleIncrementSize(dxMa->rtvHeapDesc.Type);

#pragma endregion
		/// ※これ以降の描画コマンドでは、ここで指定した描画キャンパスに絵を描いていくことになる ///
		/// --END-- ///

		// --深度ステンシルビュー用デスクリプタヒープのハンドルを取得-- //
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		dxMa->commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

		/// --3.画面クリア R G B A-- ///
		/// ※バックバッファには前回に描いた絵がそのまま残っているので、一旦指定色で塗りつぶす ///
#pragma region

		FLOAT clearColor[] = { 0.1f, 0.25, 0.5f, 0.0f }; // 青っぽい色
		dxMa->commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		dxMa->commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#pragma endregion
		/// --END-- ///

		/// --4.描画コマンド-- ///
#pragma region

		/// --ビューポート設定-- ///
#pragma region

		// --ビューポート設定コマンド-- //
		D3D12_VIEWPORT viewport{};
		viewport.Width = win->GetWidth();
		viewport.Height = win->GetHeight();
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		
		// --ビューポート設定コマンドを、コマンドリストに積む-- //
		dxMa->commandList->RSSetViewports(1, &viewport);

#pragma endregion
		/// --END-- ///

		/// --シザー矩形-- ///
#pragma region

		// --シザー矩形-- //
		D3D12_RECT scissorRect{};
		scissorRect.left = 0; // 切り抜き座標左
		scissorRect.right = scissorRect.left + win->GetWidth(); // 切り抜き座標右
		scissorRect.top = 0; // 切り抜き座標上
		scissorRect.bottom = scissorRect.top + win->GetHeight(); // 切り抜き座標下
		
		// --シザー矩形設定コマンドを、コマンドリストに積む-- //
		dxMa->commandList->RSSetScissorRects(1, &scissorRect);

#pragma endregion
		/// --END-- ///

		// --パイプラインステートとルートシグネチャの設定コマンド-- //
		dxMa->commandList->SetPipelineState(pipelineState);
		dxMa->commandList->SetGraphicsRootSignature(rootSignature);

		// --プリミティブ形状の設定コマンド-- //
		dxMa->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		//// --頂点バッファビューの設定コマンド-- //
		//commandList->IASetVertexBuffers(0, 1, &vbView);

		// --定数バッファビュー（CBV）の設定コマンド-- //
		dxMa->commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		// --SRVヒープの設定コマンド-- //
		dxMa->commandList->SetDescriptorHeaps(1, &srvHeap);

		// --SRVヒープの先頭ハンドルを取得（SRVを指しているはず）-- //
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

		// --SRVヒープの先頭にあるSRVをルートパラメータ1番に設定-- //
		//commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		// --2枚目を指し示すようにしたSRVのハンドルをルートパラメータ1番に設定-- //
		srvGpuHandle.ptr += descriptorSize;
		dxMa->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		// --全オブジェクトについての処理-- //
		for (size_t i = 0; i < _countof(object3ds); i++) {
			object3ds[i].Draw(dxMa->commandList.Get(), vbView, ibView, _countof(indices));
		}

#pragma endregion
		/// --END-- ///

		/// --5.リソースバリアを戻す-- ///
#pragma region

		// --バックバッファを書き込み可能状態から画面表示状態に変更する
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
		dxMa->commandList->ResourceBarrier(1, &barrierDesc);

		// --ここまでため込んだコマンドを実行し描画する処理-- //
		{
			// --命令のクローズ
			result = dxMa->commandList->Close();
			assert(SUCCEEDED(result));

			// --コマンドリストの実行
			ID3D12CommandList* commandLists[] = { dxMa->commandList.Get()};
			dxMa->commandQueue->ExecuteCommandLists(1, commandLists);

			// --画面に表示するバッファをフリップ(裏表の入替え)
			result = dxMa->swapChain->Present(1, 0);
			assert(SUCCEEDED(result));
		}
		// --END-- //

		// --コマンドの実行完了を待つ-- //
		dxMa->commandQueue->Signal(dxMa->fence.Get(), ++dxMa->fenceVal);
		if (dxMa->fence->GetCompletedValue() != dxMa->fenceVal)
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			dxMa->fence->SetEventOnCompletion(dxMa->fenceVal, event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		// --キューをクリア-- //
		// ※次の使用に備えてコマンドアロケータとコマンドリストをリセットしておく //
		result = dxMa->cmdAllocator->Reset();
		assert(SUCCEEDED(result));

		// --再びコマンドリストを貯める準備-- //
		result = dxMa->commandList->Reset(dxMa->cmdAllocator.Get(), nullptr);
		assert(SUCCEEDED(result));

#pragma endregion
		/// --END-- ///

#pragma endregion
	}

	return 0;
}