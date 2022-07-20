//#include "TextureManager.h"
//
//int Texture::Load(const wchar_t * textureName) {
//
//	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
//	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
//	HRESULT result;
//
//	TexMetadata metadata{};
//	ScratchImage scratchImg{};
//
//	// --WIC�e�N�X�`���̃��[�h
//	result = LoadFromWICFile(
//		textureName,
//		WIC_FLAGS_NONE,
//		&metadata, scratchImg
//	);
//
//	// --�~�b�v�}�b�v����-- //
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
//	// --�ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���-- //
//	metadata.format = MakeSRGB(metadata.format);
//
//	// --�q�[�v�ݒ�-- //
//	D3D12_HEAP_PROPERTIES textureHeapProp{};
//	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
//	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//
//	// --���\�[�X�ݒ�-- //
//	D3D12_RESOURCE_DESC textureResourceDesc{};
//	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
//	textureResourceDesc.Format = metadata.format;
//	textureResourceDesc.Width = metadata.width;
//	textureResourceDesc.Height = (UINT)metadata.height;
//	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
//	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
//	textureResourceDesc.SampleDesc.Count = 1;
//
//	// --�e�N�X�`���o�b�t�@�̐���-- //
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
//	// --�S�~�b�v�}�b�v�ɂ���-- //
//	for (size_t i = 0; i < metadata.mipLevels; i++)
//	{
//		// �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
//		const Image* img = scratchImg.GetImage(i, 0, 0);
//
//		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
//		result = texBuff->WriteToSubresource(
//			(UINT)i,
//			nullptr,// --------------> �S�̈�փR�s�[
//			img->pixels,// ----------> ���f�[�^�A�h���X
//			(UINT)img->rowPitch,// --> 1���C���T�C�Y
//			(UINT)img->slicePitch// -> 1���T�C�Y
//		);
//		assert(SUCCEEDED(result));
//	}
//
//	// --�V�F�[�_���\�[�X�r���[�ݒ�-- //
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
//	srvDesc.Format = textureResourceDesc.Format;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = textureResourceDesc.MipLevels;
//
//	// --�n���h���̎w���@�ɃV�F�[�_�[���\�[�X�r���[�쐬-- //
//	device->CreateShaderResourceView(texBuff, &srvDesc, srvHandle);
//
//	// --CBV, SRV, UAV��1���̃T�C�Y���擾-- //
//	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//
//	// --�n���h����1�i�߂�-- //
//	srvHandle.ptr += descriptorSize * 1;
//
//	// --CBV�i�R���X�^���g�o�b�t�@�r���[�j�̐ݒ�-- //
//	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
//
//	// --�萔�o�b�t�@�r���[�ݒ�-- //
//	device->CreateConstantBufferView(&cbvDesc, srvHandle);
//
//	// --�f�X�N���v�^�����W�̐ݒ�-- //
//	D3D12_DESCRIPTOR_RANGE descriptorRange{};
//	descriptorRange.NumDescriptors = 1;// -> 1�x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
//	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
//	descriptorRange.BaseShaderRegister = 0;// -> �e�N�X�`�����W�X�^0��
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
//	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
//	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
//	HRESULT result;
//
//	// --�f�o�C�X��ǂݍ���-- //
//	this->device = device;
//
//	// --SRV�̍ő��-- //
//	const size_t kMaxSRVCount = 2056;
//
//	// --�f�X�N���v�^�q�[�v�̐ݒ�-- //
//	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
//	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//	srvHeapDesc.NumDescriptors = kMaxSRVCount;
//
//	// --�ݒ�����Ƃ�SRV�p�f�X�N���v�^�q�[�v�𐶐�-- //
//	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
//	assert(SUCCEEDED(result));
//
//	srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
//}