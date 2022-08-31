#include "Pipeline.h"

// --DirectX3D�N���X-- //
#include "DXManager.h"

// --2D�p�p�C�v���C���ݒ�-- //
PipelineSet Pipeline::pipelineSet2D = { nullptr, nullptr };

// --3D�p�p�C�v���C���ݒ�-- //
PipelineSet Pipeline::pipelineSet3D = { nullptr, nullptr };

// --2D�I�u�W�F�N�g�i�X�v���C�g�j�p�p�C�v���C������-- //
void Pipeline::PipelineGenerate2D() {
	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
	HRESULT result;

	// --���̊֐��ōŏI�I�ɕԂ��ϐ�-- //
	PipelineSet pipelineSet;

	/// --�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��-- ///
#pragma region

	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// --���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��-- //
	result = D3DCompileFromFile(
		L"SpriteVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		vsBlob.GetAddressOf(), errorBlob.GetAddressOf());

	// --�G���[�Ȃ�-- //
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// --�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-- //
	result = D3DCompileFromFile(
		L"SpritePS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		psBlob.GetAddressOf(), errorBlob.GetAddressOf());

	// --�G���[�Ȃ�-- //
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

#pragma endregion
	/// --END-- ///

		/// --���_���C�A�E�g-- ///
#pragma region

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",// -> �Z�}���e�B�b�N��
			0,// -> �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X�i0�ł悢�j
			DXGI_FORMAT_R32G32B32_FLOAT,// -> �v�f���ƃr�b�g����\���iXYZ��3��float�^�Ȃ̂�R32G32B32_FLOAT�j
			0,// -> ���̓X���b�g�C���f�b�N�X�i0�ł悢�j
			D3D12_APPEND_ALIGNED_ELEMENT,// -> �f�[�^�̃I�t�Z�b�g�l�iD3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�j
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,// -> ���̓f�[�^�̎�ʁi�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA�j
			0// -> ��x�ɕ`�悷��C���X�^���X���i0�ł悢�j
		},

		{
			"TEXCOORD",// -> �Z�}���e�B�b�N��
			0,// -> �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X�i0�ł悢�j
			DXGI_FORMAT_R32G32_FLOAT,// -> �v�f���ƃr�b�g����\���iXYZ��3��float�^�Ȃ̂�R32G32B32_FLOAT�j
			0,// -> ���̓X���b�g�C���f�b�N�X�i0�ł悢�j
			D3D12_APPEND_ALIGNED_ELEMENT,// -> �f�[�^�̃I�t�Z�b�g�l�iD3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�j
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,// -> ���̓f�[�^�̎�ʁi�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA�j
			0// -> ��x�ɕ`�悷��C���X�^���X���i0�ł悢�j
		},
	};

#pragma endregion
	/// --END-- ///

	/// --�O���t�B�b�N�X�p�C�v���C��-- ///
#pragma region

	// --�O���t�B�b�N�X�p�C�v���C���ݒ�-- //
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// --�V�F�[�_�[�̐ݒ�-- //
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// --�T���v���}�X�N�̐ݒ�-- //
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// --���X�^���C�U�̐ݒ�-- //
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	// --�u�����h�X�e�[�g-- //
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// --�A���t�@�l���ʐݒ�-- //
	blenddesc.BlendEnable = true;// ----------------> �u�����h��L������
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;// -> ���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;// ---> �\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;// -> �f�X�g�̒l��0%�g��

	// --���Z����-- //
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// -> ���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;// ---> �\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// --> 1.0f-�\�[�X�̃A���t�@�l

	// --���_���C�A�E�g�̐ݒ�-- //
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// --�}�`�̌`��ݒ�-- //
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// --���̑��̐ݒ�-- //
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O
	
	// --�f�X�N���v�^�����W�̐ݒ�-- //
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;// -> 1�x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;// -> �e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// --���[�g�p�����[�^�̐ݒ�-- //
	D3D12_ROOT_PARAMETER rootParams[2] = {};
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;// --> �萔�̃o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;// ------------------> �萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;// -------------------> �f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -> �S�ẴV�F�[�_�[���猩����

	// -- �e�N�X�`�����W�X�^0��-- //
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;// -> ���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;// -------> �f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;// --------------------> �f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -------------> �S�ẴV�F�[�_���猩����

	// --�e�N�X�`���T���v���[�̐ݒ�-- //
	// ���e�N�X�`�����I�u�W�F�N�g�ɒ���t���Ƃ��̊g��k���̕�ԕ��@�Ȃǂ��w�肷����� //
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> ���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> �c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> ���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;// -> �{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;// ------------------> �S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;// --------------------------------> �~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;// ---------------------------------------------> �~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;// -> �s�N�Z���V�F�[�_�[����̂ݎg�p�\

	// --���[�g�V�O�l�`���̐ݒ�-- //
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;// -------------> ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// --���[�g�V�O�l�`���̃V���A���C�Y-- //
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = DXManager::GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootSignature));
	assert(SUCCEEDED(result));

	// --�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g-- //
	pipelineDesc.pRootSignature = pipelineSet.rootSignature.Get();

	// --�p�C�v�����X�e�[�g�̐���-- //
	result = DXManager::GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineSet.pipelineState));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	// --�p�C�v���C�����Z�b�g-- //
	pipelineSet2D = pipelineSet;
};

// --3D�I�u�W�F�N�g�p�p�C�v���C������-- //
void Pipeline::PipelineGenerate3D() {
	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
	HRESULT result;

	// --���̊֐��ōŏI�I�ɕԂ��ϐ�-- //
	PipelineSet pipelineSet;

	// --�f�X�N���v�^�����W�̐ݒ�-- //
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;// -> 1�x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;// -> �e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	/// --�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��-- ///
#pragma region

	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// --���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��-- //
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		vsBlob.GetAddressOf(), errorBlob.GetAddressOf());

	// --�G���[�Ȃ�-- //
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// --�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��-- //
	result = D3DCompileFromFile(
		L"BasicPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		psBlob.GetAddressOf(), errorBlob.GetAddressOf());

	// --�G���[�Ȃ�-- //
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

#pragma endregion
	/// --END-- ///

	/// --���_���C�A�E�g-- ///
#pragma region

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",// -> �Z�}���e�B�b�N��
			0,// -> �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X�i0�ł悢�j
			DXGI_FORMAT_R32G32B32_FLOAT,// -> �v�f���ƃr�b�g����\���iXYZ��3��float�^�Ȃ̂�R32G32B32_FLOAT�j
			0,// -> ���̓X���b�g�C���f�b�N�X�i0�ł悢�j
			D3D12_APPEND_ALIGNED_ELEMENT,// -> �f�[�^�̃I�t�Z�b�g�l�iD3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�j
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,// -> ���̓f�[�^�̎�ʁi�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA�j
			0// -> ��x�ɕ`�悷��C���X�^���X���i0�ł悢�j
		},

		{
			"NORMAL",// -> �Z�}���e�B�b�N��
			0,// -> �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X�i0�ł悢�j
			DXGI_FORMAT_R32G32B32_FLOAT,// -> �v�f���ƃr�b�g����\���iXYZ��3��float�^�Ȃ̂�R32G32B32_FLOAT�j
			0,// -> ���̓X���b�g�C���f�b�N�X�i0�ł悢�j
			D3D12_APPEND_ALIGNED_ELEMENT,// -> �f�[�^�̃I�t�Z�b�g�l�iD3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�j
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,// -> ���̓f�[�^�̎�ʁi�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA�j
			0// -> ��x�ɕ`�悷��C���X�^���X���i0�ł悢�j
		},

		{
			"TEXCOORD",// -> �Z�}���e�B�b�N��
			0,// -> �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X�i0�ł悢�j
			DXGI_FORMAT_R32G32_FLOAT,// -> �v�f���ƃr�b�g����\���iXYZ��3��float�^�Ȃ̂�R32G32B32_FLOAT�j
			0,// -> ���̓X���b�g�C���f�b�N�X�i0�ł悢�j
			D3D12_APPEND_ALIGNED_ELEMENT,// -> �f�[�^�̃I�t�Z�b�g�l�iD3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�j
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,// -> ���̓f�[�^�̎�ʁi�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA�j
			0// -> ��x�ɕ`�悷��C���X�^���X���i0�ł悢�j
		}, // (1�s�ŏ������ق������₷��)
	};

#pragma endregion
	/// --END-- ///

	/// --�O���t�B�b�N�X�p�C�v���C��-- ///
#pragma region

	// --�O���t�B�b�N�X�p�C�v���C���ݒ�-- //
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// --�V�F�[�_�[�̐ݒ�-- //
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// --�T���v���}�X�N�̐ݒ�-- //
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// --���X�^���C�U�̐ݒ�-- //
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��ȃ����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	// --�f�v�X�X�e���V���X�e�[�g�̐ݒ�-- //
	pipelineDesc.DepthStencilState.DepthEnable = true;// -> �[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;// -> �������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;// -> ��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;// -> �[�x�l�t�H�[�}�b�g

	// --�u�����h�X�e�[�g-- //
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// --�A���t�@�l���ʐݒ�-- //
	blenddesc.BlendEnable = false;// ----------------> �u�����h��L������
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;// -> ���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;// ---> �\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;// -> �f�X�g�̒l��0%�g��

	// --���Z����-- //
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// -> ���Z
	blenddesc.SrcBlend = D3D12_BLEND_ONE;// ---> �\�[�X�̒l��100%�g��
	blenddesc.DestBlend = D3D12_BLEND_ONE;// --> �f�X�g�̒l��100%�g��

	// --���_���C�A�E�g�̐ݒ�-- //
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// --�}�`�̌`��ݒ�-- //
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// --���̑��̐ݒ�-- //
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// --���[�g�p�����[�^�̐ݒ�-- //
	D3D12_ROOT_PARAMETER rootParams[5] = {};

	// --�萔�o�b�t�@0��-- //
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;// --> �萔�̃o�b�t�@�r���[
	rootParams[0].Descriptor.ShaderRegister = 0;// ------------------> �萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;// -------------------> �f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -> �S�ẴV�F�[�_�[���猩����

	// -- �e�N�X�`�����W�X�^0��-- //
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;// -> ���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;// -------> �f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;// --------------------> �f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -------------> �S�ẴV�F�[�_���猩����

	// --�萔�o�b�t�@1��-- //
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;// --> �萔�̃o�b�t�@�r���[
	rootParams[2].Descriptor.ShaderRegister = 1;// ------------------> �萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;// -------------------> �f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -> �S�ẴV�F�[�_�[���猩����

	// --�萔�o�b�t�@2��-- //
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;// --> �萔�̃o�b�t�@�r���[
	rootParams[3].Descriptor.ShaderRegister = 2;// ------------------> �萔�o�b�t�@�ԍ�
	rootParams[3].Descriptor.RegisterSpace = 0;// -------------------> �f�t�H���g�l
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -> �S�ẴV�F�[�_�[���猩����

	// --�萔�o�b�t�@2��-- //
	rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;// --> �萔�̃o�b�t�@�r���[
	rootParams[4].Descriptor.ShaderRegister = 3;// ------------------> �萔�o�b�t�@�ԍ�
	rootParams[4].Descriptor.RegisterSpace = 0;// -------------------> �f�t�H���g�l
	rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -> �S�ẴV�F�[�_�[���猩����

	// --�e�N�X�`���T���v���[�̐ݒ�-- //
	// ���e�N�X�`�����I�u�W�F�N�g�ɒ���t���Ƃ��̊g��k���̕�ԕ��@�Ȃǂ��w�肷����� //
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> ���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> �c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> ���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;// -> �{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;// ------------------> �S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;// --------------------------------> �~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;// ---------------------------------------------> �~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;// -> �s�N�Z���V�F�[�_�[����̂ݎg�p�\

	// --���[�g�V�O�l�`���̐ݒ�-- //
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;// -------------> ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);// -> ���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// --���[�g�V�O�l�`���̃V���A���C�Y-- //
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = DXManager::GetDevice()->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootSignature));
	assert(SUCCEEDED(result));

	// --�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g-- //
	pipelineDesc.pRootSignature = pipelineSet.rootSignature.Get();

	// --�p�C�v�����X�e�[�g�̐���-- //
	result = DXManager::GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineSet.pipelineState));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

#pragma endregion
	/// --�`�揉��������END-- ///

	// --�p�C�v���C�����Z�b�g-- //
	pipelineSet3D = pipelineSet;
}

// --2D�p�p�C�v���C���ݒ���Q��-- //
PipelineSet Pipeline::GetPipelineSet2D() { return pipelineSet2D; }

// --3D�p�p�C�v���C���ݒ���Q��-- //
PipelineSet Pipeline::GetPipelineSet3D() { return pipelineSet3D; }