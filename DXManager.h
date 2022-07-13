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
	// --DirectX�̏������Ŏg���ϐ�-- //

	// --�f�o�C�X�p-- //
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	// --�o�b�N�o�b�t�@�̃A�h���X�����Ă����ϐ�-- //
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	// --�f�X�N���v�^�q�[�v-- //
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// --�t�F���X-- //
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;

public:
	// --�R���X�g���N�^-- //
	DXManager();

	// --�f�X�g���N�^-- //
	~DXManager();

	// --DirectX����������-- //
	void DXInitialize(HWND hwnd);
};