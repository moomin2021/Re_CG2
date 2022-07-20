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
	// --DirectX�̏������Ŏg���ϐ�-- //

	// --�E�B���h�E�T�C�Y-- //
	int winWidth, winHeight;

	// --�f�o�C�X�p-- //
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ID3D12GraphicsCommandList* commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	// --�o�b�N�o�b�t�@�̃A�h���X�����Ă����ϐ�-- //
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	// --�f�X�N���v�^�q�[�v-- //
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};

	// --�t�F���X-- //
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;

	D3D12_RESOURCE_BARRIER barrierDesc;

	ID3D12DescriptorHeap* dsvHeap;

public:
	// --�R���X�g���N�^-- //
	DXManager(int width, int height);

	// --�f�X�g���N�^-- //
	~DXManager();

	// --DirectX����������-- //
	void DXInitialize(HWND hwnd);

	// --�O���t�B�b�N�X�R�}���h�J�n-- //
	void GraphicsCommandStart();

	// --�O���t�B�b�N�X�R�}���h�I��-- //
	void GraphicsCommandEnd();
};