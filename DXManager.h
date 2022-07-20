#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <wrl.h>
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class DXManager {

public:
	// --DirectXの初期化で使う変数-- //

	// --ウィンドウサイズ-- //
	int winWidth, winHeight;

	// --デバイス用-- //
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ID3D12GraphicsCommandList* commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	// --バックバッファのアドレスを入れておく変数-- //
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	// --デスクリプタヒープ-- //
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// --フェンス-- //
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;

	D3D12_RESOURCE_BARRIER barrierDesc;

	ID3D12DescriptorHeap* dsvHeap;

public:
	// --コンストラクタ-- //
	DXManager(int width, int height);

	// --デストラクタ-- //
	~DXManager();

	// --DirectX初期化処理-- //
	void DXInitialize(HWND hwnd);

	// --グラフィックスコマンド開始-- //
	void GraphicsCommandStart();

	// --グラフィックスコマンド終了-- //
	void GraphicsCommandEnd();
};