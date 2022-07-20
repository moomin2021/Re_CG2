//#include "TextureManager.h"
//
//int Texture::Load(const wchar_t * textureName) {
//
//	// --関数が成功したかどうかを判別する用変数-- //
//	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成 //
//	HRESULT result;
//
//	TexMetadata metadata{};
//	ScratchImage scratchImg{};
//
//	// --WICテクスチャのロード
//	result = LoadFromWICFile(
//		textureName,
//		WIC_FLAGS_NONE,
//		&metadata, scratchImg
//	);
//
//	// --ミップマップ生成-- //
//#pragma region
//
//	ScratchImage mipChain{};
//
//	result = GenerateMipMaps(
//		scratchImg.GetImages(),
//		scratchImg.GetImageCount(),
//		scratchImg.GetMetadata(),
//		TEX_FILTER_DEFAULT,
//		0,
//		mipChain
//	);
//
//#pragma endregion
//	// --END-- //
//
//	// --読み込んだディフューズテクスチャをSRGBとして扱う-- //
//	metadata.format = MakeSRGB(metadata.format);
//
//	// --ヒープ設定-- //
//	D3D12_HEAP_PROPERTIES textureHeapProp{};
//	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//
//	// --リソース設定-- //
//	D3D12_RESOURCE_DESC textureResourceDesc{};
//	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResourceDesc.Format = metadata.format;
//	textureResourceDesc.Width = metadata.width;
//	textureResourceDesc.Height = (UINT)metadata.height;
//	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
//	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
//	textureResourceDesc.SampleDesc.Count = 1;
//
//	// --テクスチャバッファの生成-- //
//	ID3D12Resource* texBuff = nullptr;
//	result = device->CreateCommittedResource(
//		&textureHeapProp,
//		D3D12_HEAP_FLAG_NONE,
//		&textureResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&texBuff)
//	);
//
//	// --全ミップマップについて-- //
//	for (size_t i = 0; i < metadata.mipLevels; i++)
//	{
//		// ミップマップレベルを指定してイメージを取得
//		const Image* img = scratchImg.GetImage(i, 0, 0);
//
//		// テクスチャバッファにデータ転送
//		result = texBuff->WriteToSubresource(
//			(UINT)i,
//			nullptr,// --------------> 全領域へコピー
//			img->pixels,// ----------> 元データアドレス
//			(UINT)img->rowPitch,// --> 1ラインサイズ
//			(UINT)img->slicePitch// -> 1枚サイズ
//		);
//		assert(SUCCEEDED(result));
//	}
//
//	// --シェーダリソースビュー設定-- //
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
//	srvDesc.Format = textureResourceDesc.Format;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;
//
//	// --ハンドルの指す①にシェーダーリソースビュー作成-- //
//	device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
//
//	// --CBV, SRV, UAVの1個分のサイズを取得-- //
//	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//
//	// --ハンドルを1つ進める-- //
//	srvHandle.ptr += descriptorSize * 1;
//
//	// --CBV（コンスタントバッファビュー）の設定-- //
//	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
//
//	// --定数バッファビュー設定-- //
//	device->CreateConstantBufferView(&cbvDesc, srvHandle);
//
//	// --デスクリプタレンジの設定-- //
//	D3D12_DESCRIPTOR_RANGE descriptorRange{};
//	descriptorRange.NumDescriptors = 1;// -> 1度の描画に使うテクスチャが1枚なので1
//	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
//	descriptorRange.BaseShaderRegister = 0;// -> テクスチャレジスタ0番
//	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
//
//	return srvHandle.ptr;
//
//}
//
//Texture* Texture::GetInstance() {
//	static Texture instance;
//	return &instance;
//}
//
//void Texture::Initialize(ID3D12Device* device) {
//	// --関数が成功したかどうかを判別する用変数-- //
//	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成 //
//	HRESULT result;
//
//	// --デバイスを読み込む-- //
//	this->device = device;
//
//	// --SRVの最大個数-- //
//	const size_t kMaxSRVCount = 2056;
//
//	// --デスクリプタヒープの設定-- //
//	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
//	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//	srvHeapDesc.NumDescriptors = kMaxSRVCount;
//
//	// --設定をもとにSRV用デスクリプタヒープを生成-- //
//	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
//	assert(SUCCEEDED(result));
//
//	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
//}