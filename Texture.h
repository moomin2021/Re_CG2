#pragma once

// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --数学関数-- //
#include <DirectXMath.h>
using namespace DirectX;

// --DirectXTex-- //
#include <DirectXteX.h>

 // --ComPtr用-- //
#include <wrl.h>
using namespace Microsoft::WRL;

class Texture {
///  --メンバ変数-- ///
public:
	// --SRV用デスクリプタヒープ-- //
	ID3D12DescriptorHeap* srvHeap;

private:
	// --インスタンス-- //
	static Texture* myInstance;

	// --デバイス-- //
	ID3D12Device* device;

	// --SRVヒープの先頭ハンドルを取得-- //
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	// --読み込む画像が何枚目か-- //
	UINT imageCount;

/// --メンバ変数END-- ///
///---------------- ///
/// --メンバ関数-- ///
public:
	// --インスタンス読み込み-- //
	static Texture* GetInstance();

	// --インスタンス解放-- //
	void Relese();

	// --初期化処理-- //
	void Initialize(ID3D12Device* device);

	// --テクスチャの読み込み-- //
	int LoadTexture(const wchar_t* szFile);

private:
	// --コンストラクタ-- //
	Texture();

/// --メンバ関数END-- ///
};