#include "DrawCommSet.h"

// --Direct3D 12 用-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --DirectX3Dクラス-- //
#include "DXManager.h"

// --パイプライン用-- //
#include "Pipeline.h"

// --テクスチャクラス-- //
#include "Texture.h"

// --2Dオブジェクト共通設定-- //
void DrawCommSet::DrawCommSet2D() {
	// --パイプラインステートの設定コマンド-- //
	DXManager::GetCommandList()->SetPipelineState(Pipeline::GetPipelineSet2D().pipelineState.Get());

	// --ルートシグネチャの設定コマンド-- //
	DXManager::GetCommandList()->SetGraphicsRootSignature(Pipeline::GetPipelineSet2D().rootSignature.Get());

	// --プリミティブ形状の設定コマンド-- //
	DXManager::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 線のストリップ

	// --デスクリプタヒープの配列をセットするコマンド-- //
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
	DXManager::GetCommandList()->SetDescriptorHeaps(1, ppHeaps);
}

// --3Dオブジェクト共通設定-- //
void DrawCommSet::DrawCommSet3D() {
	// --パイプラインステートとルートシグネチャの設定コマンド-- //
	DXManager::GetCommandList()->SetPipelineState(Pipeline::GetPipelineSet3D().pipelineState.Get());
	DXManager::GetCommandList()->SetGraphicsRootSignature(Pipeline::GetPipelineSet3D().rootSignature.Get());

	// --プリミティブ形状の設定コマンド-- //
	DXManager::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP); // 線のストリップ

	// --プリミティブ形状の設定コマンド-- //
	DXManager::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	// --SRVヒープの設定コマンド-- //
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
	DXManager::GetCommandList()->SetDescriptorHeaps(1, ppHeaps);
}