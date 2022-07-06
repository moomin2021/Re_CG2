#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class DXManager {

public:
	// --DirectX�̏������Ŏg���ϐ�-- //

	// --�f�o�C�X�p-- //
	ID3D12Device* device;
	IDXGIFactory7* dxgiFactory;
	IDXGISwapChain4* swapChain;
	ID3D12CommandAllocator* cmdAllocator;
	ID3D12GraphicsCommandList* commandList;
	ID3D12CommandQueue* commandQueue;
	ID3D12DescriptorHeap* rtvHeap;

	// --�o�b�N�o�b�t�@�̃A�h���X�����Ă����ϐ�-- //
	std::vector<ID3D12Resource*> backBuffers;

	// --�f�X�N���v�^�q�[�v-- //
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// --�t�F���X-- //
	ID3D12Fence* fence;
	UINT64 fenceVal;

public:
	// --�R���X�g���N�^-- //
	DXManager();

	// --�f�X�g���N�^-- //
	~DXManager();

	// --DirectX����������-- //
	void DXInitialize(HWND hwnd);
};