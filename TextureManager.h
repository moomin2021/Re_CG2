//#pragma once
////#include <d3dcompiler.h>
//#include <d3d12.h>
////#include <dxgi1_6.h>
////#include <cassert>
////#include <vector>
////#include <string>
////#include <DirectXMath.h>
//using namespace DirectX;
//
////#pragma comment(lib, "d3dcompiler.lib")
////#pragma comment(lib, "d3d12.lib")
////#pragma comment(lib, "dxgi.lib")
//
//#include <DirectXteX.h>
//
//class Texture {
//	// --デバイス用-- //
//	ID3D12Device * device;
//
//	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
//
//	ID3D12DescriptorHeap * srvHeap;
//
//public:
//	// --画像読み込み-- //
//	int Load(const wchar_t* textureName);
//
//	// --シングルトンインスタンス-- //
//	static Texture * GetInstance();
//	
//	// --初期化-- //
//	void Initialize(ID3D12Device* device);
//};