#include "DrawCommSet.h"

// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --DirectX3D�N���X-- //
#include "DXManager.h"

// --�p�C�v���C���p-- //
#include "Pipeline.h"

// --�e�N�X�`���N���X-- //
#include "Texture.h"

// --2D�I�u�W�F�N�g���ʐݒ�-- //
void DrawCommSet::DrawCommSet2D() {
	// --�p�C�v���C���X�e�[�g�̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->SetPipelineState(Pipeline::GetPipelineSet2D().pipelineState.Get());

	// --���[�g�V�O�l�`���̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->SetGraphicsRootSignature(Pipeline::GetPipelineSet2D().rootSignature.Get());

	// --�v���~�e�B�u�`��̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ���̃X�g���b�v

	// --�f�X�N���v�^�q�[�v�̔z����Z�b�g����R�}���h-- //
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
	DXManager::GetCommandList()->SetDescriptorHeaps(1, ppHeaps);
}

// --3D�I�u�W�F�N�g���ʐݒ�-- //
void DrawCommSet::DrawCommSet3D() {
	// --�p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->SetPipelineState(Pipeline::GetPipelineSet3D().pipelineState.Get());
	DXManager::GetCommandList()->SetGraphicsRootSignature(Pipeline::GetPipelineSet3D().rootSignature.Get());

	// --�v���~�e�B�u�`��̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP); // ���̃X�g���b�v

	// --�v���~�e�B�u�`��̐ݒ�R�}���h-- //
	DXManager::GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	// --SRV�q�[�v�̐ݒ�R�}���h-- //
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::GetSRVHeap() };
	DXManager::GetCommandList()->SetDescriptorHeaps(1, ppHeaps);
}