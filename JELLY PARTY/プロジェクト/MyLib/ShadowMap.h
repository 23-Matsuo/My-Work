//--------------------------------------------------------------------------------------
// File: ShadowMap.h
//
// �g�����W�V�����N���X
//
// Date: 2023/07/30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef SHADOWMAP_DEFINED
#define SHADOWMAP_DEFINED

#include "RenderTexture.h"
#include "DepthStencil.h"
#include "PostProcess.h"

#include "Scene/GameMainObjects/Objects/CatBall/CatSphere.h"
#include "Scene/GameMainObjects/Managers/StageFloorManager.h"
#include "Scene/GameMainObjects/Managers/StageSoftBodyManager.h"
#include "Scene/GameMainObjects/Light.h"

class ShadowMap
{
public:

	// �R���X�g���N�^
	ShadowMap();
	// �f�X�g���N�^
	~ShadowMap();

	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Render(
		CatSphere* pSphere,
		StageFloorManager* pStageFloorManager,
		StageSoftBodyManager* pSoftBodyManager
	);

private:

	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;

	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

private:

	// �V���h�E�}�b�v�̃T�C�Y
	static const int SHADOWMAP_SIZE = 2048;

	// �V���h�E�}�b�v�p�i�����_�[�e�N�X�`���j
	std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

	// �V���h�E�}�b�v�p�i�f�v�X�X�e���V���j
	std::unique_ptr<DepthStencil> m_shadowMapDS;

	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_Depth;

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Depth;

	// ���C�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_lightPosition;

	// ���C�g�̉�]
	DirectX::SimpleMath::Quaternion m_lightRotate;

private:

	// �萔�o�b�t�@�̍\����
	struct ConstantBuffer
	{
		DirectX::XMMATRIX lightViewProj;	// ���C�g�̓��e��Ԃ֍��W�ϊ�����s��
		DirectX::XMVECTOR lightPosition;	// ���C�g�̈ʒu
		DirectX::XMVECTOR lightAmbient;		// ���C�g�̊���
	};

	// �萔�o�b�t�@�ւ̃|�C���^
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// ���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// �s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;

	// �T���v���[(�V���h�E�}�b�v�Q�Ɨp)
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;

	// ���C�g
	std::unique_ptr<Light> m_light;

};
#endif // SHADOWMAP_DEFINED
