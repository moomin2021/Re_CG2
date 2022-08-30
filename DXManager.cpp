#include "DXManager.h"

// --�p�C�v���C���p�N���X-- //
#include "Pipeline.h"

// --�C���X�^���X��NULL����-- //
DXManager* DXManager::myInstance = nullptr;

// --�f�o�C�X-- //
ComPtr<ID3D12Device> DXManager::device = nullptr;

// --�R���X�g���N�^-- //
DXManager::DXManager() : dxgiFactory(nullptr), swapChain(nullptr),
cmdAllocator(nullptr), commandList(nullptr), commandQueue(nullptr), rtvHeap(nullptr),
backBuffers{}, fence(nullptr), fenceVal(0), barrierDesc{}, dsvHeap(nullptr) {}

// --�C���X�^���X�ǂݍ���-- //
DXManager* DXManager::GetInstance() {
	// --�C���X�^���X�����������琶������-- //
	if (myInstance == nullptr) myInstance = new DXManager();

	// --�C���X�^���X��Ԃ�-- //
	return myInstance;
}

// --�C���X�^���X���-- //
void DXManager::Relese() {
	// --�C���X�^���X�����������牽�������ɏI������-- //
	if (myInstance == nullptr) return;

	// --�C���X�^���X���-- //
	delete myInstance;
	myInstance = nullptr;
}

// --DirectX�̏���������-- //
void DXManager::Initialize(HWND hwnd, int winWidth, int winHeight) {

	// --�E�B���h�E�T�C�Y�ۑ�-- //
	this->winWidth = winWidth;
	this->winHeight = winHeight;

	// --�֐��������������ǂ����𔻕ʂ���p�ϐ�-- //
	// ��DirectX�̊֐��́AHRESULT�^�Ő����������ǂ�����Ԃ����̂������̂ł��̕ϐ����쐬 //
	HRESULT result;

	/// --DirectX����������-- ///
#pragma region

	/// --�f�o�b�N���C���[�̗L���� -- ///
	/// ��Visual Studio�́u�o�́v�E�B���h�E�Œǉ��̃G���[���b�Z�[�W���\���ł���悤�� ///
#pragma region

#ifdef _DEBUG
	//�f�o�b�O���C���[���I����
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif

#pragma endregion
	/// --END-- ///

	/// --�A�_�v�^�̗�-- ///
	/// ��PC�ɂ���O���t�B�b�N�{�[�h���A���z�I�ȃf�o�C�X���܂߂đS�����X�g�A�b�v���� ///
#pragma region

	// --DXGI�t�@�N�g���[�̐���-- //
	// DXGI = �O���t�B�b�N�X�C���t���X�g���N�`��
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// --�A�_�v�^�[�̗񋓗p-- //
	std::vector<IDXGIAdapter4*> adapters;

	// --�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������-- //
	IDXGIAdapter4 * tmpAdapter = nullptr;

	// --�p�t�H�[�}���X���������̂��珇�ɁA�S�ẴA�_�v�^�[��񋓂���-- //
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		// ���I�z��ɒǉ�����
		adapters.push_back(tmpAdapter);
	}

#pragma endregion
	/// --END-- ///

	/// --�A�_�v�^�̑I��-- //
	/// �����o���ꂽ�O���t�B�b�N�X�f�o�C�X�̒��Ő��\�̒Ⴂ���̏��O���āA��p�f�o�C�X���̗p���� ///
#pragma region

	// --�Ó��ȃA�_�v�^��I�ʂ���-- //
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;

		// �A�_�v�^�[�̏����擾����
		adapters[i]->GetDesc3(&adapterDesc);

		// �\�t�g�E�F�A�f�o�C�X�����
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// �f�o�C�X���̗p���ă��[�v�𔲂���
			tmpAdapter = adapters[i];
			break;
		}
	}

#pragma endregion
	/// --END-- ///

	/// --�f�o�C�X�̐���-- ///
	/// ���̗p�����O���t�B�b�N�X�f�o�C�X�𑀍삷�邽�߂�D3D12Device�I�u�W�F�N�g�𐶐� ///
	/// ������͕��ʂP�Q�[����1�������Ȃ� ///
#pragma region

	// --�Ή����x���̔z��-- //
	D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	// --�O���t�B�b�N�X�f�o�C�X�𑀍삷��ׂ̃I�u�W�F�N�g����-- //
	// ������͕��ʁA�P�Q�[���ɂP�������Ȃ�
	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++)
	{
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion
	/// --END-- ///

#ifdef _DEBUG
//�f�o�b�O���C���[���I����
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		// �}������G���[
		D3D12_MESSAGE_ID denyIds[] = {
			/*
			 * Windows11�ł�DXGI�f�o�b�O���C���[��DX12�f�o�b�O���C���[�̑��ݍ�p�o�O�ɂ��G���[���b�Z�[�W
			 * https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			 */
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE };
		// �}������\�����x��
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// �w�肵���G���[�̕\����}������
		infoQueue->PushStorageFilter(&filter);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);// ->���o���G���[���Ɏ~�܂�
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);// -> �G���[���Ɏ~�܂�
	}
#endif

	/// --�R�}���h���X�g-- ///
	/// ��GPU�ɁA�܂Ƃ߂Ė��߂𑗂邽�߂̃R�}���h���X�g�𐶐����� //
#pragma region

	// --�R�}���h�A���P�[�^�𐶐�-- //
	// ���R�}���h���X�g�̓R�}���h�A���P�[�^���琶������̂ŁA��ɃR�}���h�A���P�[�^����� //
	// ���R�}���h���X�g�Ɋi�[���閽�߂ׂ̈̃��������Ǘ�����I�u�W�F�N�g //
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,// -> �R�}���h�A���P�[�^�̎��
		IID_PPV_ARGS(&cmdAllocator));// -> �e�C���^�[�t�F�C�X�ŗL��GUID
	assert(SUCCEEDED(result));// -> ID3D12CommandAllocator�C���^�[�t�F�C�X�̃|�C���^���i�[����ϐ��A�h���X

	// --�R�}���h���X�g�𐶐�-- //
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --�R�}���h�L���[-- ///
	/// ���R�}���h���X�g��GPU�ɏ��Ɏ��s�����Ă����ׂ̎d�g�� ///
#pragma region

	// --�R�}���h�L���[�̐ݒ�-- //
	// ��{}�����邱�Ƃō\���̂̒��g��0�ŃN���A���Ă���B
	// ���l0���W���l�ɂȂ�悤��Microsoft�����߂Ă���̂ŁA���̏ꍇ�R�}���h�L���[�̐ݒ��W���ɂ��Ă��� //
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	// --�W���ݒ�ŃR�}���h�L���[�𐶐�-- //
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --�X���b�v�`�F�[��-- ///
	/// ���X���b�v�`�F�[���́A�_�u���o�b�t�@�����O��g���v���o�b�t�@�����O���ȒP�Ɏ������邽�߂̎d�g�� ///
	/// ���\�����̉�ʁi�t�����g�o�b�t�@�j�E�`�撆�̉�ʁi�o�b�N�o�b�t�@�j
#pragma region

	// --�X���b�v�`�F�[���̐ݒ�-- //
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// --��ʂ̕����w�肷��
	swapChainDesc.Width = 1280;

	// --��ʂ̍������w�肷��
	swapChainDesc.Height = 720;

	// --�F���̏����i�\���`���j
	//��DXGI_FORMAT_R8G8B8A8_UNORM�̓A���t�@���܂ރ`�����l��������8�r�b�g���T�|�[�g
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// --�s�N�Z��������̃}���`�T���v���̐����w�肷��
	swapChainDesc.SampleDesc.Count = 1;

	// --���\�[�X�̎g�p���@���w��
	// ��DXGI_USAGE_BACK_BUFFER�̓��\�[�X���o�b�N�o�b�t�@�Ƃ��Ďg�p����
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;

	// --�X���b�v�`�F�[�����̃o�b�t�@�[�̐����w�肷��
	swapChainDesc.BufferCount = 2;

	// --��ʂ��X���b�v������̃��\�[�X�̏������@���w��
	// ��DXGI_SWAP_EFFECT_FLIP_DISCARD�̓X���b�v������o�b�N�o�b�t�@�[�̓��e��j������ݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// --�X���b�v�`�F�[������̃I�v�V�������w��
	// ��DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH�̓E�B���h�E�ƃt���X�N���[���̐؂�ւ�����>>
	// >>�𑜓x���E�B���h�E�T�C�Y�Ɉ�v����悤�ɕύX����
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// --IDXGISwapChain1��ComPtr��p��-- //
	ComPtr<IDXGISwapChain1> swapChain1;

	// --�X���b�v�`�F�[���̐���-- //
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain1);
	assert(SUCCEEDED(result));

	// --��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����-- //
	swapChain1.As(&swapChain);

#pragma endregion
	/// --END-- ///

	/// --�����_�[�^�[�Q�b�g�r���[-- ///
	/// ���o�b�N�o�b�t�@��`��L�����p�X�Ƃ��Ĉ����ׂ̃I�u�W�F�N�g //
	/// ���_�u���o�b�t�@�����O�ł̓o�b�t�@���Q����̂łQ��� //
#pragma region

	// �������_�[�^�[�Q�b�g�r���[�̓f�X�N���v�^�q�[�v�ɐ�������̂ŁA��Ƀf�X�N���v�^�q�[�v����� //
	// --�f�X�N���v�^�q�[�v�̐ݒ�-- //
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // ���\��2��

	// --�f�X�N���v�^�q�[�v�̐���-- //
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	// ���X���b�v�`�F�[�����ɐ������ꂽ�o�b�N�o�b�t�@�̃A�h���X�����Ă���
	// --�o�b�N�o�b�t�@-- //
	backBuffers.resize(swapChainDesc.BufferCount);

	// --�X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������-- //
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// --�X���b�v�`�F�[������o�b�t�@���擾
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));

		// --�f�X�N���v�^�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

		// --�����\���ŃA�h���X�������
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

		// --�����_�[�^�[�Q�b�g�r���[�̐ݒ�
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

		// --�V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		// --�����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}

#pragma endregion
	/// --END-- ///

	/// --�t�F���X�̐���-- ///
	/// ��CPU��GPU�œ������Ƃ邽�߂�DirectX�̎d�g�� ///
#pragma region

	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion
	/// --END-- ///

#pragma endregion
	/// --DirectX������END-- ///

	/// --�`�揉��������-- ///
#pragma region

	// --���\�[�X�ݒ�-- //
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winWidth;// ---> �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.Height = winHeight;// -> �����_�[�^�[�Q�b�g�ɍ��킹��
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;// -> �[�x�l�t�H�[�}�b�g
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;// -> �f�v�X�X�e���V��

	// --�[�x�l�p�q�[�v�v���p�e�B-- //
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	// --�[�x�l�̃N���A�ݒ�-- //
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;// -> �[�x�l1.0f�i�ő�l�j�ŃN���A
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;// -> �[�x�l�t�H�[�}�b�g

	// --���\�[�X����-- //
	ID3D12Resource * depthBuff = nullptr;
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,// -> �[�x�l�������݂Ɏg�p
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	);

	// --�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬-- //
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;// -> �[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;// -> �f�v�X�X�e���V���r���[
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	// --�[�x�r���[�쐬-- //
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;// -> �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

	// --3D�p�p�C�v���C���ݒ�-- //
	Pipeline::PipelineGenerate3D();
}

// --�O���t�B�b�N�X�R�}���h�J�n-- //
void DXManager::GraphicsCommandStart(ID3D12DescriptorHeap* srvHeap) {
	HRESULT result;

	/// --1.���\�[�X�o���A�ŏ������݉\�ɕύX-- ///
#pragma region
	// --�o�b�N�o�b�t�@�̔ԍ����擾(2�Ȃ̂�0�Ԃ�1��)-- //
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // �o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
	commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion
	/// --END-- ///

	/// --2.�`���̕ύX-- ///
#pragma region

		// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

#pragma endregion
	/// ������ȍ~�̕`��R�}���h�ł́A�����Ŏw�肵���`��L�����p�X�ɊG��`���Ă������ƂɂȂ� ///
	/// --END-- ///

	// --�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾-- //
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	/// --3.��ʃN���A R G B A-- ///
	/// ���o�b�N�o�b�t�@�ɂ͑O��ɕ`�����G�����̂܂܎c���Ă���̂ŁA��U�w��F�œh��Ԃ� ///
#pragma region

	FLOAT clearColor[] = { 0.1f, 0.25, 0.5f, 0.0f }; // ���ۂ��F
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#pragma endregion
	/// --END-- ///

			/// --�r���[�|�[�g�ݒ�-- ///
#pragma region

		// --�r���[�|�[�g�ݒ�R�}���h-- //
	D3D12_VIEWPORT viewport{};
	viewport.Width = winWidth;
	viewport.Height = winHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// --�r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�-- //
	commandList->RSSetViewports(1, &viewport);

#pragma endregion
	/// --END-- ///

	/// --�V�U�[��`-- ///
#pragma region

		// --�V�U�[��`-- //
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // �؂蔲�����W��
	scissorRect.right = scissorRect.left + winWidth; // �؂蔲�����W�E
	scissorRect.top = 0; // �؂蔲�����W��
	scissorRect.bottom = scissorRect.top + winHeight; // �؂蔲�����W��

	// --�V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�-- //
	commandList->RSSetScissorRects(1, &scissorRect);

	// --�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h-- //
	commandList->SetPipelineState(Pipeline::GetPipelineSet3D().pipelineState.Get());
	commandList->SetGraphicsRootSignature(Pipeline::GetPipelineSet3D().rootSignature.Get());

	// --�v���~�e�B�u�`��̐ݒ�R�}���h-- //
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP); // ���̃X�g���b�v

	// --�v���~�e�B�u�`��̐ݒ�R�}���h-- //
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	// --SRV�q�[�v�̐ݒ�R�}���h-- //
	commandList->SetDescriptorHeaps(1, &srvHeap);

#pragma endregion
	/// --END-- ///
}

// --�O���t�B�b�N�X�R�}���h�I��-- //
void DXManager::GraphicsCommandEnd() {
	HRESULT result;

	/// --5.���\�[�X�o���A��߂�-- ///
#pragma region

		// --�o�b�N�o�b�t�@���������݉\��Ԃ����ʕ\����ԂɕύX����
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ���
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ�
	commandList->ResourceBarrier(1, &barrierDesc);

	// --�����܂ł��ߍ��񂾃R�}���h�����s���`�悷�鏈��-- //
	{
		// --���߂̃N���[�Y
		result = commandList->Close();
		assert(SUCCEEDED(result));

		// --�R�}���h���X�g�̎��s
		ID3D12CommandList* commandLists[] = { commandList };
		commandQueue->ExecuteCommandLists(1, commandLists);

		// --��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));
	}
	// --END-- //

	// --�R�}���h�̎��s������҂�-- //
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// --�L���[���N���A-- //
	// �����̎g�p�ɔ����ăR�}���h�A���P�[�^�ƃR�}���h���X�g�����Z�b�g���Ă��� //
	result = cmdAllocator->Reset();
	assert(SUCCEEDED(result));

	// --�ĂуR�}���h���X�g�𒙂߂鏀��-- //
	result = commandList->Reset(cmdAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///
}

// --�f�o�C�X���Q��-- //
ID3D12Device* DXManager::GetDevice() { return device.Get(); }