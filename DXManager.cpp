#include "DXManager.h"

// --パイプライン用クラス-- //
#include "Pipeline.h"

// --インスタンスにNULLを代入-- //
DXManager* DXManager::myInstance = nullptr;

// --デバイス-- //
ComPtr<ID3D12Device> DXManager::device = nullptr;

// --コンストラクタ-- //
DXManager::DXManager() : dxgiFactory(nullptr), swapChain(nullptr),
cmdAllocator(nullptr), commandList(nullptr), commandQueue(nullptr), rtvHeap(nullptr),
backBuffers{}, fence(nullptr), fenceVal(0), barrierDesc{}, dsvHeap(nullptr) {}

// --インスタンス読み込み-- //
DXManager* DXManager::GetInstance() {
	// --インスタンスが無かったら生成する-- //
	if (myInstance == nullptr) myInstance = new DXManager();

	// --インスタンスを返す-- //
	return myInstance;
}

// --インスタンス解放-- //
void DXManager::Relese() {
	// --インスタンスが無かったら何もせずに終了する-- //
	if (myInstance == nullptr) return;

	// --インスタンス解放-- //
	delete myInstance;
	myInstance = nullptr;
}

// --DirectXの初期化処理-- //
void DXManager::Initialize(HWND hwnd, int winWidth, int winHeight) {

	// --ウィンドウサイズ保存-- //
	this->winWidth = winWidth;
	this->winHeight = winHeight;

	// --関数が成功したかどうかを判別する用変数-- //
	// ※DirectXの関数は、HRESULT型で成功したかどうかを返すものが多いのでこの変数を作成 //
	HRESULT result;

	/// --DirectX初期化処理-- ///
#pragma region

	/// --デバックレイヤーの有効か -- ///
	/// ※Visual Studioの「出力」ウィンドウで追加のエラーメッセージが表示できるように ///
#pragma region

#ifdef _DEBUG
	//デバッグレイヤーをオンに
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif

#pragma endregion
	/// --END-- ///

	/// --アダプタの列挙-- ///
	/// ※PCにあるグラフィックボードを、仮想的なデバイスを含めて全部リストアップする ///
#pragma region

	// --DXGIファクトリーの生成-- //
	// DXGI = グラフィックスインフラストラクチャ
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(result));

	// --アダプターの列挙用-- //
	std::vector<IDXGIAdapter4*> adapters;

	// --ここに特定の名前を持つアダプターオブジェクトが入る-- //
	IDXGIAdapter4 * tmpAdapter = nullptr;

	// --パフォーマンスが高いものから順に、全てのアダプターを列挙する-- //
	for (UINT i = 0;
		dxgiFactory->EnumAdapterByGpuPreference(i,
			DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
			IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

#pragma endregion
	/// --END-- ///

	/// --アダプタの選別-- //
	/// ※検出されたグラフィックスデバイスの中で性能の低いもの除外して、専用デバイスを採用する ///
#pragma region

	// --妥当なアダプタを選別する-- //
	for (size_t i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc;

		// アダプターの情報を取得する
		adapters[i]->GetDesc3(&adapterDesc);

		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// デバイスを採用してループを抜ける
			tmpAdapter = adapters[i];
			break;
		}
	}

#pragma endregion
	/// --END-- ///

	/// --デバイスの生成-- ///
	/// ※採用したグラフィックスデバイスを操作するためにD3D12Deviceオブジェクトを生成 ///
	/// ※これは普通１ゲームに1つしか作らない ///
#pragma region

	// --対応レベルの配列-- //
	D3D_FEATURE_LEVEL levels[] = {
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};

	// --グラフィックスデバイスを操作する為のオブジェクト生成-- //
	// ※これは普通、１ゲームに１つしか作らない
	D3D_FEATURE_LEVEL featureLevel;

	for (size_t i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter, levels[i],
			IID_PPV_ARGS(&device));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

#pragma endregion
	/// --END-- ///

#ifdef _DEBUG
//デバッグレイヤーをオンに
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		// 抑制するエラー
		D3D12_MESSAGE_ID denyIds[] = {
			/*
			 * Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			 * https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			 */
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE };
		// 抑制する表示レベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 指定したエラーの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);// ->ヤバいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);// -> エラー時に止まる
	}
#endif

	/// --コマンドリスト-- ///
	/// ※GPUに、まとめて命令を送るためのコマンドリストを生成する //
#pragma region

	// --コマンドアロケータを生成-- //
	// ※コマンドリストはコマンドアロケータから生成するので、先にコマンドアロケータを作る //
	// ※コマンドリストに格納する命令の為のメモリを管理するオブジェクト //
	result = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,// -> コマンドアロケータの種類
		IID_PPV_ARGS(&cmdAllocator));// -> 各インターフェイス固有のGUID
	assert(SUCCEEDED(result));// -> ID3D12CommandAllocatorインターフェイスのポインタを格納する変数アドレス

	// --コマンドリストを生成-- //
	result = device->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --コマンドキュー-- ///
	/// ※コマンドリストをGPUに順に実行させていく為の仕組み ///
#pragma region

	// --コマンドキューの設定-- //
	// ※{}をつけることで構造体の中身を0でクリアしている。
	// ※値0が標準値になるようにMicrosoftが決めているので、この場合コマンドキューの設定を標準にしている //
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	// --標準設定でコマンドキューを生成-- //
	result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --スワップチェーン-- ///
	/// ※スワップチェーンは、ダブルバッファリングやトリプルバッファリングを簡単に実装するための仕組み ///
	/// ※表示中の画面（フロントバッファ）・描画中の画面（バックバッファ）
#pragma region

	// --スワップチェーンの設定-- //
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	// --画面の幅を指定する
	swapChainDesc.Width = 1280;

	// --画面の高さを指定する
	swapChainDesc.Height = 720;

	// --色情報の書式（表示形式）
	//※DXGI_FORMAT_R8G8B8A8_UNORMはアルファを含むチャンネルあたり8ビットをサポート
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// --ピクセルあたりのマルチサンプルの数を指定する
	swapChainDesc.SampleDesc.Count = 1;

	// --リソースの使用方法を指定
	// ※DXGI_USAGE_BACK_BUFFERはリソースをバックバッファとして使用する
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;

	// --スワップチェーン内のバッファーの数を指定する
	swapChainDesc.BufferCount = 2;

	// --画面をスワップした後のリソースの処理方法を指定
	// ※DXGI_SWAP_EFFECT_FLIP_DISCARDはスワップした後バックバッファーの内容を破棄する設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// --スワップチェーン動作のオプションを指定
	// ※DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCHはウィンドウとフルスクリーンの切り替え時に>>
	// >>解像度がウィンドウサイズに一致するように変更する
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// --IDXGISwapChain1のComPtrを用意-- //
	ComPtr<IDXGISwapChain1> swapChain1;

	// --スワップチェーンの生成-- //
	result = dxgiFactory->CreateSwapChainForHwnd(
		commandQueue.Get(), hwnd, &swapChainDesc, nullptr, nullptr,
		(IDXGISwapChain1**)&swapChain1);
	assert(SUCCEEDED(result));

	// --生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する-- //
	swapChain1.As(&swapChain);

#pragma endregion
	/// --END-- ///

	/// --レンダーターゲットビュー-- ///
	/// ※バックバッファを描画キャンパスとして扱う為のオブジェクト //
	/// ※ダブルバッファリングではバッファが２つあるので２つ作る //
#pragma region

	// ※レンダーターゲットビューはデスクリプタヒープに生成するので、先にデスクリプタヒープを作る //
	// --デスクリプタヒープの設定-- //
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // 裏表の2つ

	// --デスクリプタヒープの生成-- //
	device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));

	// ※スワップチェーン内に生成されたバックバッファのアドレスを入れておく
	// --バックバッファ-- //
	backBuffers.resize(swapChainDesc.BufferCount);

	// --スワップチェーンの全てのバッファについて処理する-- //
	for (size_t i = 0; i < backBuffers.size(); i++)
	{
		// --スワップチェーンからバッファを取得
		swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));

		// --デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

		// --裏か表かでアドレスがずれる
		rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

		// --レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};

		// --シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		// --レンダーターゲットビューの生成
		device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}

#pragma endregion
	/// --END-- ///

	/// --フェンスの生成-- ///
	/// ※CPUとGPUで同期をとるためのDirectXの仕組み ///
#pragma region

	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

#pragma endregion
	/// --END-- ///

#pragma endregion
	/// --DirectX初期化END-- ///

	/// --描画初期化処理-- ///
#pragma region

	// --リソース設定-- //
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = winWidth;// ---> レンダーターゲットに合わせる
	depthResourceDesc.Height = winHeight;// -> レンダーターゲットに合わせる
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;// -> 深度値フォーマット
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;// -> デプスステンシル

	// --深度値用ヒーププロパティ-- //
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;

	// --深度値のクリア設定-- //
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;// -> 深度値1.0f（最大値）でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;// -> 深度値フォーマット

	// --リソース生成-- //
	ID3D12Resource * depthBuff = nullptr;
	result = device->CreateCommittedResource(
		&depthHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,// -> 深度値書き込みに使用
		&depthClearValue,
		IID_PPV_ARGS(&depthBuff)
	);

	// --深度ビュー用デスクリプタヒープ作成-- //
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;// -> 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;// -> デプスステンシルビュー
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	// --深度ビュー作成-- //
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;// -> 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(
		depthBuff,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

	// --3D用パイプライン設定-- //
	Pipeline::PipelineGenerate3D();
}

// --グラフィックスコマンド開始-- //
void DXManager::GraphicsCommandStart(ID3D12DescriptorHeap* srvHeap) {
	HRESULT result;

	/// --1.リソースバリアで書き込み可能に変更-- ///
#pragma region
	// --バックバッファの番号を取得(2つなので0番か1番)-- //
	UINT bbIndex = swapChain->GetCurrentBackBufferIndex();

	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // バックバッファを指定
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // 表示状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態へ
	commandList->ResourceBarrier(1, &barrierDesc);

#pragma endregion
	/// --END-- ///

	/// --2.描画先の変更-- ///
#pragma region

		// レンダーターゲットビューのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);

#pragma endregion
	/// ※これ以降の描画コマンドでは、ここで指定した描画キャンパスに絵を描いていくことになる ///
	/// --END-- ///

	// --深度ステンシルビュー用デスクリプタヒープのハンドルを取得-- //
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	/// --3.画面クリア R G B A-- ///
	/// ※バックバッファには前回に描いた絵がそのまま残っているので、一旦指定色で塗りつぶす ///
#pragma region

	FLOAT clearColor[] = { 0.1f, 0.25, 0.5f, 0.0f }; // 青っぽい色
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#pragma endregion
	/// --END-- ///

			/// --ビューポート設定-- ///
#pragma region

		// --ビューポート設定コマンド-- //
	D3D12_VIEWPORT viewport{};
	viewport.Width = winWidth;
	viewport.Height = winHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// --ビューポート設定コマンドを、コマンドリストに積む-- //
	commandList->RSSetViewports(1, &viewport);

#pragma endregion
	/// --END-- ///

	/// --シザー矩形-- ///
#pragma region

		// --シザー矩形-- //
	D3D12_RECT scissorRect{};
	scissorRect.left = 0; // 切り抜き座標左
	scissorRect.right = scissorRect.left + winWidth; // 切り抜き座標右
	scissorRect.top = 0; // 切り抜き座標上
	scissorRect.bottom = scissorRect.top + winHeight; // 切り抜き座標下

	// --シザー矩形設定コマンドを、コマンドリストに積む-- //
	commandList->RSSetScissorRects(1, &scissorRect);

	// --パイプラインステートとルートシグネチャの設定コマンド-- //
	commandList->SetPipelineState(Pipeline::GetPipelineSet3D().pipelineState.Get());
	commandList->SetGraphicsRootSignature(Pipeline::GetPipelineSet3D().rootSignature.Get());

	// --プリミティブ形状の設定コマンド-- //
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP); // 線のストリップ

	// --プリミティブ形状の設定コマンド-- //
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// --SRVヒープの設定コマンド-- //
	commandList->SetDescriptorHeaps(1, &srvHeap);

#pragma endregion
	/// --END-- ///
}

// --グラフィックスコマンド終了-- //
void DXManager::GraphicsCommandEnd() {
	HRESULT result;

	/// --5.リソースバリアを戻す-- ///
#pragma region

		// --バックバッファを書き込み可能状態から画面表示状態に変更する
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // 描画状態から
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // 表示状態へ
	commandList->ResourceBarrier(1, &barrierDesc);

	// --ここまでため込んだコマンドを実行し描画する処理-- //
	{
		// --命令のクローズ
		result = commandList->Close();
		assert(SUCCEEDED(result));

		// --コマンドリストの実行
		ID3D12CommandList* commandLists[] = { commandList };
		commandQueue->ExecuteCommandLists(1, commandLists);

		// --画面に表示するバッファをフリップ(裏表の入替え)
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));
	}
	// --END-- //

	// --コマンドの実行完了を待つ-- //
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// --キューをクリア-- //
	// ※次の使用に備えてコマンドアロケータとコマンドリストをリセットしておく //
	result = cmdAllocator->Reset();
	assert(SUCCEEDED(result));

	// --再びコマンドリストを貯める準備-- //
	result = commandList->Reset(cmdAllocator.Get(), nullptr);
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///
}

// --デバイスを参照-- //
ID3D12Device* DXManager::GetDevice() { return device.Get(); }