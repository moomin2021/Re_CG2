#pragma once
// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --DXGI�p-- //
#include <dxgi1_6.h>
#pragma comment(lib, "dxgi.lib")

// --�V�F�[�_�̓ǂݍ��݂ƃR���p�C���p-- //
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

// --�����^�ł��邱�Ƃ𔻒肷��@�\�̒ǉ�-- //
#include <cassert>

// --�V�[�P���X�R���e�i�̈��-- //
#include <vector>

// --������-- //
#include <string>

// --ComPtr�p-- //
#include <wrl.h>
using namespace Microsoft::WRL;

class DXManager {
/// --�����o�ϐ�-- ///
public:

	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
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

private:
	// --�C���X�^���X-- //
	static DXManager* myInstance;

	// --�f�o�C�X�p-- //
	static ComPtr<ID3D12Device> device;

	// --�R�}���h���X�g-- //
	static ID3D12GraphicsCommandList* commandList;

	// --�E�B���h�E�T�C�Y�ۑ��p�ϐ�-- //
	int winWidth, winHeight;

/// --�����o�ϐ�END-- ///
/// --------------- ///
/// --�����o�֐�-- ///
public:
	// --�C���X�^���X�ǂݍ���-- //
	static DXManager* GetInstance();

	// --�C���X�^���X���-- //
	void Relese();

	// --DirectX����������-- //
	void Initialize(HWND hwnd, int winWidth, int winHeight);

	// --�O���t�B�b�N�X�R�}���h�J�n-- //
	void GraphicsCommandStart();

	// --�O���t�B�b�N�X�R�}���h�I��-- //
	void GraphicsCommandEnd();

	// --�f�o�C�X���Q��-- //
	static ID3D12Device* GetDevice();

	// --�R�}���h���X�g���Q��-- //
	static ID3D12GraphicsCommandList* GetCommandList();

private:
	// --�R���X�g���N�^-- //
	DXManager();

/// --�����o�֐�END-- ///
};