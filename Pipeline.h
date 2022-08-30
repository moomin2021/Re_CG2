#pragma once
// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --シェーダの読み込みとコンパイル用-- //
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

// --ComPtr用-- //
#include <wrl.h>
using namespace Microsoft::WRL;

// --文字列-- //
#include <string>

// --式が真であることを判定する機能の追加-- //
#include <cassert>

// --パイプラインセット-- //
struct PipelineSet {
	// --パイプラインステートオブジェクト-- //
	ComPtr<ID3D12PipelineState> pipelineState;

	// --ルートシグネチャ-- //
	ComPtr<ID3D12RootSignature> rootSignature;
};

class Pipeline {
/// --メンバ変数-- ///
public:

private:
	// --2D用のパイプライン設定-- //
	static PipelineSet pipelineSet2D;

	// --3D用のパイプライン設定-- //
	static PipelineSet pipelineSet3D;

/// --メンバ変数END-- ///
/// --------------- ///
/// --メンバ関数-- ///
public:
	// --2Dオブジェクト（スプライト）用パイプライン生成-- //
	static void PipelineGenerate2D();

	// --3Dオブジェクト用パイプライン生成-- //
	static void PipelineGenerate3D();

	// --2D用パイプライン設定を参照-- //
	static PipelineSet GetPipelineSet2D();

	// --3D用パイプライン設定を参照-- //
	static PipelineSet GetPipelineSet3D();

private:

/// --メンバ関数END-- ///
};