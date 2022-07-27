#pragma once
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
//#include <cassert>
//#include <vector>
//#include <string>
#include <DirectXMath.h>
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <DirectXteX.h>

#include <wrl.h>
using namespace Microsoft::WRL;

class Texture {
private:
	// --デバイス-- //
	ComPtr<ID3D12Device> device;

	// --SRVヒープの先頭ハンドルを取得-- //
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;

	// --読み込む画像が何枚目か-- //
	UINT imageCount;

public:

	// --SRV用デスクリプタヒープ-- //
	ID3D12DescriptorHeap* srvHeap;

public:
	// --コンストラクタ-- //
	Texture(ComPtr<ID3D12Device> device);

	// --テクスチャの読み込み-- //
	int LoadTexture(const wchar_t * szFile);
};