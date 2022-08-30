#pragma once
// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --DXGI用-- //
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")

// --シェーダの読み込みとコンパイル用-- //
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

// --式が真であることを判定する機能の追加-- //
#include <cassert>

// --シーケンスコンテナの一種-- //
#include <vector>

// --文字列-- //
#include <string>

// --ComPtr用-- //
#include <wrl.h>
using namespace Microsoft::WRL;

class DXManager {
/// --メンバ変数-- ///
public:

	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
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

private:
	// --インスタンス-- //
	static DXManager* myInstance;

	// --デバイス用-- //
	static ComPtr<ID3D12Device> device;

	// --コマンドリスト-- //
	static ID3D12GraphicsCommandList* commandList;

	// --ウィンドウサイズ保存用変数-- //
	int winWidth, winHeight;

/// --メンバ変数END-- ///
/// --------------- ///
/// --メンバ関数-- ///
public:
	// --インスタンス読み込み-- //
	static DXManager* GetInstance();

	// --インスタンス解放-- //
	void Relese();

	// --DirectX初期化処理-- //
	void Initialize(HWND hwnd, int winWidth, int winHeight);

	// --グラフィックスコマンド開始-- //
	void GraphicsCommandStart();

	// --グラフィックスコマンド終了-- //
	void GraphicsCommandEnd();

	// --デバイスを参照-- //
	static ID3D12Device* GetDevice();

	// --コマンドリストを参照-- //
	static ID3D12GraphicsCommandList* GetCommandList();

private:
	// --コンストラクタ-- //
	DXManager();

/// --メンバ関数END-- ///
};