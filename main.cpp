﻿#include "Input.h"
#include "Window.h"
#include "DXManager.h"
#include "Object3D.h"
#include "Vertex.h"
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;

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

	// --ウィンドウ生成クラス-- //
	Window* win = new Window();

	// --ウィンドウの初期化-- //
	win->WindowInitialize();

	// --DirectX初期化クラス-- //
	DXManager* dxMa = new DXManager(win->GetWidth(), win->GetHeight());

	// --DirectX初期化処理-- //
	dxMa->DXInitialize(win->hwnd);

	// --キーボード入力の初期化-- //
	Input* input = new Input();
	input->InitializeInput(win->w, win->hwnd);
#pragma endregion

	/////////////////////
	// --描画初期化処理-- //
	/////////////////////

#pragma region

	HRESULT result;

	//// --定数バッファに送るデータをまとめた型-- //
	//struct ConstBufferDataMaterial
	//{
	//	// 色（RGBA）
	//	XMFLOAT4 color;
	//};

	//ID3D12Resource* constBuffMaterial = nullptr;
	//ConstBufferDataMaterial* constMapMaterial = nullptr;

	//ID3D12Resource* constBuffMaterial2 = nullptr;
	//ConstBufferDataMaterial* constMapMaterial2 = nullptr;

	//{
	//	// --ヒープ設定-- //
	//	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;// -> GPUへの転送用

	//	// --リソース設定-- //
	//	D3D12_RESOURCE_DESC cbResourceDesc{};
	//	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;// -> 256バイトアラインメント
	//	cbResourceDesc.Height = 1;
	//	cbResourceDesc.DepthOrArraySize = 1;
	//	cbResourceDesc.MipLevels = 1;
	//	cbResourceDesc.SampleDesc.Count = 1;
	//	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	// --定数バッファの生成-- //
	//	result = dxMa->device->CreateCommittedResource(
	//		&cbHeapProp,// -> ヒープ設定
	//		D3D12_HEAP_FLAG_NONE,
	//		&cbResourceDesc,// -> リソース設定
	//		D3D12_RESOURCE_STATE_GENERIC_READ,
	//		nullptr,
	//		IID_PPV_ARGS(&constBuffMaterial)
	//	);

	//	// --定数バッファの生成-- //
	//	result = dxMa->device->CreateCommittedResource(
	//		&cbHeapProp,// -> ヒープ設定
	//		D3D12_HEAP_FLAG_NONE,
	//		&cbResourceDesc,// -> リソース設定
	//		D3D12_RESOURCE_STATE_GENERIC_READ,
	//		nullptr,
	//		IID_PPV_ARGS(&constBuffMaterial2)
	//	);

	//	// --マッピング-- //
	//	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);
	//	assert(SUCCEEDED(result));

	//	// --マッピング-- //
	//	result = constBuffMaterial2->Map(0, nullptr, (void**)&constMapMaterial2);
	//	assert(SUCCEEDED(result));
	//}

	//// --定数バッファを生成-- //
	//ID3D12Resource * constBuffTransform0 = nullptr;
	//ConstBufferDataTransform * constMapTransform0 = nullptr;

	//ID3D12Resource * constBuffTransform1 = nullptr;
	//ConstBufferDataTransform * constMapTransform1 = nullptr;

	{
		//// --ヒープ設定-- //
		//D3D12_HEAP_PROPERTIES cbHeapProp{};
		//cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;// -> GPUへの転送用

		//// --リソース設定-- //
		//D3D12_RESOURCE_DESC cbResourceDesc{};
		//cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		//cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;// -> 256バイトアラインメント
		//cbResourceDesc.Height = 1;
		//cbResourceDesc.DepthOrArraySize = 1;
		//cbResourceDesc.MipLevels = 1;
		//cbResourceDesc.SampleDesc.Count = 1;
		//cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		
	}

	//// --単位行列を代入-- //
	//constMapTransform0->mat = XMMatrixIdentity();

	//// --平行投影行列の計算-- //
	//constMapTransform0->mat = XMMatrixOrthographicOffCenterLH(
	//	2.0f / window_width, -1.0f,
	//	1.0f, -2.0f / window_height,
	//	0.0f, 1.0f
	//);

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

	// --自作画像データ-- //
	const size_t textureWidth = 256;
	const size_t textureHeight = 256;
	const size_t imageDataCount = textureWidth * textureHeight;
	XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];

	for (size_t i = 0; i < imageDataCount; i++) {
		imageData[i].x = 1.0f;
		imageData[i].y = 1.0f;
		imageData[i].z = 1.0f;
		imageData[i].w = 1.0f;
	}

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

	D3D12_RESOURCE_DESC textureResourceDesc3{};
	textureResourceDesc3.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc3.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc3.Width = textureWidth;
	textureResourceDesc3.Height = textureHeight;
	textureResourceDesc3.DepthOrArraySize = 1;
	textureResourceDesc3.MipLevels = 1;
	textureResourceDesc3.SampleDesc.Count = 1;
	

	// --テクスチャバッファの生成-- //
	ID3D12Resource* texBuff = nullptr;
	result = dxMa->device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);

	// --テクスチャバッファの生成-- //
	ID3D12Resource* texBuff2 = nullptr;
	result = dxMa->device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2)
	);

	// --テクスチャバッファの生成-- //
	ID3D12Resource* texBuff3 = nullptr;
	result = dxMa->device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc3,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff3)
	);

	// --全ミップマップについて-- //
	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		// ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);

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
		const Image* img = scratchImg2.GetImage(i, 0, 0);

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

	// --テクスチャバッファにデータ転送-- //
	result = texBuff3->WriteToSubresource(
		0,
		nullptr,
		imageData,
		sizeof(XMFLOAT4) * textureWidth,
		sizeof(XMFLOAT4) * imageDataCount
	);
	assert(SUCCEEDED(result));

	delete[] imageData;

	// --SRVの最大個数-- //
	const size_t kMaxSRVCount = 2056;

	// --デスクリプタヒープの設定-- //
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// --設定をもとにSRV用デスクリプタヒープを生成-- //
	ID3D12DescriptorHeap* srvHeap = nullptr;
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
	dxMa->device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);

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
	dxMa->device->CreateShaderResourceView(texBuff2, &srvDesc2, srvHandle);

	// --ハンドルを1つ進める-- //
	srvHandle.ptr += descriptorSize * 1;

	// --シェーダリソースビュー設定-- //
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc3{};
	srvDesc3.Format = textureResourceDesc3.Format;
	srvDesc3.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc3.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc3.Texture2D.MipLevels = textureResourceDesc3.MipLevels;

	// --ハンドルの指す①にシェーダーリソースビュー作成-- //
	dxMa->device->CreateShaderResourceView(texBuff3, &srvDesc3, srvHandle);

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

	/// --シェーダーの読み込みとコンパイル-- ///
#pragma region

	ID3DBlob* vsBlob = nullptr; // 頂点シェーダオブジェクト
	ID3DBlob* psBlob = nullptr; // ピクセルシェーダオブジェクト
	ID3DBlob* errorBlob = nullptr; // エラーオブジェクト

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
		std::copy_n((char*)errorBlob->GetBufferPointer(),
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
		std::copy_n((char*)errorBlob->GetBufferPointer(),
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
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// --アルファ値共通設定-- //
	blenddesc.BlendEnable = false;// ----------------> ブレンドを有効する
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;// -> 加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;// ---> ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;// -> デストの値を0%使う

	// --加算合成-- //
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// -> 加算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;// ---> ソースの値を100%使う
	blenddesc.DestBlend = D3D12_BLEND_ONE;// --> デストの値を100%使う

	//// 減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;// -> デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;// ------------> ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;// -----------> デストの値を100%使う

	//// 色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// ----------> 加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;// -> 1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;// ----------> 使わない

	//// 半透明合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// ----------> 加算
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;// ------> ソースのアルファ値
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// -> 1.0f-ソースのアルファ値

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
	// ※テクスチャがオブジェクトに張り付くときの拡大縮小の補間方法などを指定するもの //
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
	ComPtr<ID3D12RootSignature> rootSignature;

	// --ルートシグネチャの設定-- //
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;// -------------> ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);// -> ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// --ルートシグネチャのシリアライズ-- //
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dxMa->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	// --パイプラインにルートシグネチャをセット-- //
	pipelineDesc.pRootSignature = rootSignature.Get();

	// --パイプランステートの生成-- //
	ID3D12PipelineState* pipelineState = nullptr;
	result = dxMa->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	// --初期化-- //
	float angle = 0.0f;

	// --3Dオブジェクトの数-- //
	const size_t kObjectCount = 3;

	// 3Dオブジェクトの配列
	Object * object3ds = new Object[kObjectCount];

	object3ds[0].position = { 0, 10, 0 };
	object3ds[1].position = { 2, -5, 0 };
	object3ds[2].position = { -2, -5, 0 };

	// --配列内の全オブジェクトに対して-- //
	for (size_t i = 0; i < kObjectCount; i++) {
		// --初期化
		object3ds[i].Initialize(dxMa->device.Get());
	}

	XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 0.5f };

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

			// --クラス解放-- //
			delete win;

			break;
		}

		////////////////////////////
		// --DirectX毎フレーム処理-- //
		////////////////////////////

#pragma region

		// --キーボード入力の更新処理-- //
		input->UpdateInput();

		// --座標0, 0, 0をターゲットにカメラを回転-- //
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			angle += XMConvertToRadians(input->PushKey(DIK_D) - input->PushKey(DIK_A));

			eye.x = -100 * sinf(angle);
			eye.z = -100 * cosf(angle);

			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		}

		// --各オブジェクトの色の変更-- //
		object3ds[0].color.x -= 0.01f;
		if (object3ds[0].color.x <= 0.0f) object3ds[0].color.x = 1.0f;
		object3ds[1].color.y -= 0.01f;
		if (object3ds[1].color.y <= 0.0f) object3ds[1].color.y = 1.0f;
		object3ds[2].color.z -= 0.01f;
		if (object3ds[2].color.z <= 0.0f) object3ds[2].color.z = 1.0f;

		// --オブジェクトの操作-- //
		object3ds[0].position.x += input->PushKey(DIK_RIGHT) - input->PushKey(DIK_LEFT);
		object3ds[0].position.y += input->PushKey(DIK_UP) - input->PushKey(DIK_DOWN);

		// --全オブジェクトの更新処理-- //
		for (size_t i = 0; i < kObjectCount; i++) {
			object3ds[i].Update(matView, matProjection);
		}

		// --スペースを押すとソリッドモードからワイヤーフレームモードになる
		pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし

		if (input->TriggerMouseButton(M_CENTER)) {
			pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ポリゴン内を塗りつぶさない
		}

		result = dxMa->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
		assert(SUCCEEDED(result));

		// --ブレンドモードの切り替え-- //
		if (input->TriggerKey(DIK_1)) {
			if (blenddesc.BlendEnable == true) blenddesc.BlendEnable = false;
			else blenddesc.BlendEnable = true;
			result = dxMa->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
			assert(SUCCEEDED(result));
		}



		dxMa->GraphicsCommandStart();

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
		dxMa->commandList->SetGraphicsRootSignature(rootSignature.Get());

		// --プリミティブ形状の設定コマンド-- //
		dxMa->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP); // 線のストリップ



		// --プリミティブ形状の設定コマンド-- //
		dxMa->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		//// --頂点バッファビューの設定コマンド-- //
		//commandList->IASetVertexBuffers(0, 1, &vbView);

		// --SRVヒープの設定コマンド-- //
		dxMa->commandList->SetDescriptorHeaps(1, &srvHeap);

		// --SRVヒープの先頭ハンドルを取得（SRVを指しているはず）-- //
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();

		// --SRVヒープの先頭にあるSRVをルートパラメータ1番に設定-- //
		//commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		// --2枚目を指し示すようにしたSRVのハンドルをルートパラメータ1番に設定-- //
		dxMa->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		// --全オブジェクトについての処理-- //
		//for (size_t i = 0; i < kObjectCount; i++) {
		//	object3ds[i].DrawCube(dxMa->commandList);
		//}

		object3ds[2].DrawTriangle(dxMa->commandList);

		srvGpuHandle.ptr += descriptorSize;
		dxMa->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		object3ds[1].DrawTriangle(dxMa->commandList);

		srvGpuHandle.ptr += descriptorSize;
		dxMa->commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

		object3ds[0].DrawCube(dxMa->commandList);


#pragma endregion
		/// --END-- ///

		dxMa->GraphicsCommandEnd();

#pragma endregion
		/// --END-- ///

#pragma endregion
	}

	return 0;
}