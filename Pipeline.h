#pragma once
// --Direct3D 12 �p-- //
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

// --�V�F�[�_�̓ǂݍ��݂ƃR���p�C���p-- //
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

// --ComPtr�p-- //
#include <wrl.h>
using namespace Microsoft::WRL;

// --������-- //
#include <string>

// --�����^�ł��邱�Ƃ𔻒肷��@�\�̒ǉ�-- //
#include <cassert>

// --�p�C�v���C���Z�b�g-- //
struct PipelineSet {
	// --�p�C�v���C���X�e�[�g�I�u�W�F�N�g-- //
	ComPtr<ID3D12PipelineState> pipelineState;

	// --���[�g�V�O�l�`��-- //
	ComPtr<ID3D12RootSignature> rootSignature;
};

class Pipeline {
/// --�����o�ϐ�-- ///
public:

private:
	// --2D�p�̃p�C�v���C���ݒ�-- //
	static PipelineSet pipelineSet2D;

	// --3D�p�̃p�C�v���C���ݒ�-- //
	static PipelineSet pipelineSet3D;

/// --�����o�ϐ�END-- ///
/// --------------- ///
/// --�����o�֐�-- ///
public:
	// --2D�I�u�W�F�N�g�i�X�v���C�g�j�p�p�C�v���C������-- //
	static void PipelineGenerate2D();

	// --3D�I�u�W�F�N�g�p�p�C�v���C������-- //
	static void PipelineGenerate3D();

	// --2D�p�p�C�v���C���ݒ���Q��-- //
	static PipelineSet GetPipelineSet2D();

	// --3D�p�p�C�v���C���ݒ���Q��-- //
	static PipelineSet GetPipelineSet3D();

private:

/// --�����o�֐�END-- ///
};