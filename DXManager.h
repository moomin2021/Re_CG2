#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class DXManager {

public:
	// --DirectXの初期化で使う変数-- //

	// --デバイス用-- //
	ID3D12Device* device;
	IDXGIFactory7* dxgiFactory;
	IDXGISwapChain4* swapChain;
	ID3D12CommandAllocator* cmdAllocator;
	ID3D12GraphicsCommandList* commandList;
	ID3D12CommandQueue* commandQueue;
	ID3D12DescriptorHeap* rtvHeap;

	// --バックバッファのアドレスを入れておく変数-- //
	std::vector<ID3D12Resource*> backBuffers;

	// --デスクリプタヒープ-- //
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// --フェンス-- //
	ID3D12Fence* fence;
	UINT64 fenceVal;

public:
	// --コンストラクタ-- //
	DXManager();

	// --デストラクタ-- //
	~DXManager();

	// --DirectX初期化処理-- //
	void DXInitialize(HWND hwnd);
};