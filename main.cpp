#include "Input.h"
#include "Window.h"
#include "DXManager.h"
#include "Object3D.h"
#include "Vertex.h"
#include "Texture.h"
#include <d3dcompiler.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <DirectXteX.h>

#include "Util.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// --ウィンドウ生成クラス-- //
	ComPtr<Window> win = new Window();

	// --ウィンドウの初期化-- //
	win->WindowInitialize();

	// --DirectX初期化クラス-- //
	ComPtr<DXManager> dxMa = new DXManager(win->GetWidth(), win->GetHeight());

	// --DirectX初期化処理-- //
	dxMa->DXInitialize(win->hwnd);

	// --キーボード入力の初期化-- //
	ComPtr<Input> input = new Input();
	input->InitializeInput(win->w, win->hwnd);

	// --テクスチャクラス-- //
	ComPtr<Texture> texture = new Texture(dxMa->device);
#pragma endregion

	/////////////////////
	// --描画初期化処理-- //
	/////////////////////

#pragma region

	HRESULT result;

	// --透視投影行列の計算-- //
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),// -----------> 上下画角45度
		(float)win->GetWidth() / win->GetHeight(),// -> アスペクト比（画面横幅/画面縦幅）
		0.1f, 1000.0f// ------------------------> 前端、奥端
	);

	// --ビュー変換行列-- //
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -100);
	XMFLOAT3 target(0, 0, 0);
	XMFLOAT3 up(0, 1, 0);
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	// --デスクリプタレンジの設定-- //
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;// -> 1度の描画に使うテクスチャが1枚なので1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;// -> テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	/// --シェーダーの読み込みとコンパイル-- ///
#pragma region

	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// --頂点シェーダの読み込みとコンパイル-- //
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// --エラーなら-- //
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// --ピクセルシェーダの読み込みとコンパイル-- //
	result = D3DCompileFromFile(
		L"BasicPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	// --エラーなら-- //
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}

#pragma endregion
	/// --END-- ///

	/// --頂点レイアウト-- ///
#pragma region

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION",// -> セマンティック名
			0,// -> 同じセマンティック名が複数あるときに使うインデックス（0でよい）
			DXGI_FORMAT_R32G32B32_FLOAT,// -> 要素数とビット数を表す（XYZの3つでfloat型なのでR32G32B32_FLOAT）
			0,// -> 入力スロットインデックス（0でよい）
			D3D12_APPEND_ALIGNED_ELEMENT,// -> データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,// -> 入力データの種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
			0// -> 一度に描画するインスタンス数（0でよい）
		},

		{
			"NORMAL",// -> セマンティック名
			0,// -> 同じセマンティック名が複数あるときに使うインデックス（0でよい）
			DXGI_FORMAT_R32G32B32_FLOAT,// -> 要素数とビット数を表す（XYZの3つでfloat型なのでR32G32B32_FLOAT）
			0,// -> 入力スロットインデックス（0でよい）
			D3D12_APPEND_ALIGNED_ELEMENT,// -> データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,// -> 入力データの種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
			0// -> 一度に描画するインスタンス数（0でよい）
		},

		{
			"TEXCOORD",// -> セマンティック名
			0,// -> 同じセマンティック名が複数あるときに使うインデックス（0でよい）
			DXGI_FORMAT_R32G32_FLOAT,// -> 要素数とビット数を表す（XYZの3つでfloat型なのでR32G32B32_FLOAT）
			0,// -> 入力スロットインデックス（0でよい）
			D3D12_APPEND_ALIGNED_ELEMENT,// -> データのオフセット値（D3D12_APPEND_ALIGNED_ELEMENTだと自動設定）
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,// -> 入力データの種別（標準はD3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA）
			0// -> 一度に描画するインスタンス数（0でよい）
		}, // (1行で書いたほうが見やすい)
	};

#pragma endregion
	/// --END-- ///

	/// --グラフィックスパイプライン-- ///
#pragma region

	// --グラフィックスパイプライン設定-- //
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// --シェーダーの設定-- //
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	// --サンプルマスクの設定-- //
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定

	// --ラスタライザの設定-- //
	//pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // 背面を科リング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
	pipelineDesc.RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

	// --デプスステンシルステートの設定-- //
	pipelineDesc.DepthStencilState.DepthEnable = true;// -> 深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;// -> 書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;// -> 小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;// -> 深度値フォーマット

	// --ブレンドステート-- //
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
	//	= D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA全てのチャンネルを描画
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// --アルファ値共通設定-- //
	blenddesc.BlendEnable = false;// ----------------> ブレンドを有効する
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;// -> 加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;// ---> ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;// -> デストの値を0%使う

	// --加算合成-- //
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// -> 加算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;// ---> ソースの値を100%使う
	blenddesc.DestBlend = D3D12_BLEND_ONE;// --> デストの値を100%使う

	//// 減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;// -> デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;// ------------> ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;// -----------> デストの値を100%使う

	//// 色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// ----------> 加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;// -> 1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;// ----------> 使わない

	//// 半透明合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;// ----------> 加算
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;// ------> ソースのアルファ値
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// -> 1.0f-ソースのアルファ値

	// --頂点レイアウトの設定-- //
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// --図形の形状設定-- //
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// --その他の設定-- //
	pipelineDesc.NumRenderTargets = 1; // 描画対象は1つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// --ルートパラメータの設定-- //
	D3D12_ROOT_PARAMETER rootParams[3] = {};

	// --定数バッファ0番-- //
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;// --> 定数のバッファビュー
	rootParams[0].Descriptor.ShaderRegister = 0;// ------------------> 定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;// -------------------> デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -> 全てのシェーダーから見える

	// -- テクスチャレジスタ0番-- //
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;// -> 種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;// -------> デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;// --------------------> デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -------------> 全てのシェーダから見える

	// --定数バッファ1番-- //
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;// --> 定数のバッファビュー
	rootParams[2].Descriptor.ShaderRegister = 1;// ------------------> 定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;// -------------------> デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;// -> 全てのシェーダーから見える

	// --テクスチャサンプラーの設定-- //
	// ※テクスチャがオブジェクトに張り付くときの拡大縮小の補間方法などを指定するもの //
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> 横繰り返し（タイリング）
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> 縦繰り返し（タイリング）
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;// -> 奥行繰り返し（タイリング）
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;// -> ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;// ------------------> 全てリニア補間
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;// --------------------------------> ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;// ---------------------------------------------> ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;// -> ピクセルシェーダーからのみ使用可能

	// --ルートシグネチャ-- //
	ComPtr<ID3D12RootSignature> rootSignature;

	// --ルートシグネチャの設定-- //
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;// -------------> ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);// -> ルートパラメータ数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// --ルートシグネチャのシリアライズ-- //
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dxMa->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	// --パイプラインにルートシグネチャをセット-- //
	pipelineDesc.pRootSignature = rootSignature.Get();

	// --パイプランステートの生成-- //
	ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	result = dxMa->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

#pragma endregion
	/// --END-- ///

	/// --更新処理初期化-- ///
#pragma region

	// --テクスチャの読み込み-- //
	int marioGraph = texture->LoadTexture(L"Resources/mario.jpg");
	int reimuGraph = texture->LoadTexture(L"Resources/reimu.png");

	// --床オブジェクトの初期化-- //
	ComPtr<Object> floor = new Object(texture->srvHeap);
	floor->scale = { 10.0f, 0.05f, 10.0f };
	floor->Initialize(dxMa->device.Get());

	// --カメラの角度-- //
	float angleX = 90.0f, angleY = 90.0f;

	// --カメラの回転速度-- //
	float cameraRotaSpeed = 0.05f;

	// --カメラと注視点の距離-- //
	float length = 10.0f;

#pragma endregion
	/// --END-- ///

	// --ゲームループ-- //
	while (true)
	{

		// --メッセージがあるか-- //
		if (PeekMessage(&win->msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&win->msg); // キー入力メッセージの処理
			DispatchMessage(&win->msg); // プロシージャにメッセージを送る
		}

		// --✖ボタンで終了メッセージが来たらゲームループを抜ける-- //
		if (win->msg.message == WM_QUIT)
		{
			break;
		}

		////////////////////////////
		// --DirectX毎フレーム処理-- //
		////////////////////////////

#pragma region

		// --入力の更新処理-- //
		input->UpdateInput();

		// --カメラの操作-- //
		{
			eye.x += input->PushKey(DIK_D) - input->PushKey(DIK_A);
			eye.y += input->PushKey(DIK_SPACE);
			eye.z += input->PushKey(DIK_W) - input->PushKey(DIK_S);

			// --マウスの移動量で角度を変更
			angleX -= input->GetMouseVelosity().x * cameraRotaSpeed;
			angleY += input->GetMouseVelosity().y * cameraRotaSpeed;

			// --上下の回転を制限する
			angleY = Util::Clamp(angleY, 180.0f, 0.0f);

			// --注視点を変更
			target.x = eye.x + cosf(Util::Degree2Radian(angleX)) * length;
			target.y = eye.y + cosf(Util::Degree2Radian(angleY)) * length;
			target.z = eye.z + sinf(Util::Degree2Radian(angleX)) * length;

			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		}

		// --床オブジェクト更新処理-- //
		floor->Update(matView, matProjection);

		// --グラフィックスコマンドスタート-- //
		dxMa->GraphicsCommandStart();

#pragma endregion
		/// --END-- ///

		/// --4.描画コマンド-- ///
#pragma region

		/// --ビューポート設定-- ///
#pragma region

		// --ビューポート設定コマンド-- //
		D3D12_VIEWPORT viewport{};
		viewport.Width = win->GetWidth();
		viewport.Height = win->GetHeight();
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		// --ビューポート設定コマンドを、コマンドリストに積む-- //
		dxMa->commandList->RSSetViewports(1, &viewport);

#pragma endregion
		/// --END-- ///

		/// --シザー矩形-- ///
#pragma region

		// --シザー矩形-- //
		D3D12_RECT scissorRect{};
		scissorRect.left = 0; // 切り抜き座標左
		scissorRect.right = scissorRect.left + win->GetWidth(); // 切り抜き座標右
		scissorRect.top = 0; // 切り抜き座標上
		scissorRect.bottom = scissorRect.top + win->GetHeight(); // 切り抜き座標下

		// --シザー矩形設定コマンドを、コマンドリストに積む-- //
		dxMa->commandList->RSSetScissorRects(1, &scissorRect);

#pragma endregion
		/// --END-- ///

		// --パイプラインステートとルートシグネチャの設定コマンド-- //
		dxMa->commandList->SetPipelineState(pipelineState.Get());
		dxMa->commandList->SetGraphicsRootSignature(rootSignature.Get());

		// --プリミティブ形状の設定コマンド-- //
		dxMa->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP); // 線のストリップ

		// --プリミティブ形状の設定コマンド-- //
		dxMa->commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

		//// --頂点バッファビューの設定コマンド-- //
		//commandList->IASetVertexBuffers(0, 1, &vbView);

		// --SRVヒープの設定コマンド-- //
		dxMa->commandList->SetDescriptorHeaps(1, &texture->srvHeap);

		// --床オブジェクト描画-- //
		floor->DrawCube(dxMa->commandList);

#pragma endregion
		/// --END-- ///

		dxMa->GraphicsCommandEnd();

#pragma endregion
		/// --END-- ///

#pragma endregion
	}

	return 0;
}