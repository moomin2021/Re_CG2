#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

class DXManager {

public:
	// --DirectXの初期化で使う変数-- //

	// --デバイス用-- //
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	// --バックバッファのアドレスを入れておく変数-- //
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	// --デスクリプタヒープ-- //
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// --フェンス-- //
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;

public:
	// --コンストラクタ-- //
	DXManager();

	// --デストラクタ-- //
	~DXManager();

	// --DirectX初期化処理-- //
	void DXInitialize(HWND hwnd);
};