//--------------------------------------------------------------------------------------
// File: ResultConfetti.h
//
// ���U���g�̎����቉�o
//
// 
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef RESULT_CONFETTI_DEFINED
#define RESULT_CONFETTI_DEFINED

#include "StepTimer.h"
#include <vector>
#include <list>

#include "MyLib/ConfettiUtility.h"

class ResultConfetti
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

public:
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// �R���X�g���N�^
	ResultConfetti();

	// �f�X�g���N�^
	~ResultConfetti();

	// ������
	void Create(DX::DeviceResources* pDR);

	// �X�V
	void Update(const float& elapsedTime);

	// �`��
	void Render();

	// �r���{�[�h�쐬
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 up);

	// ������쐬
	void CreateConfetti(const float& elapsedTime);

private:

	// �V�F�[�_�[�쐬
	void CreateShader();

	// �F���擾
	const DirectX::SimpleMath::Color GetColor(int rnd);

private:

	DX::DeviceResources* m_pDR;

	float m_timer;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	m_geometryShader;

	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	DirectX::SimpleMath::Matrix m_billboard;

	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	std::list<ConfettiUtility> m_confettiUtility;

	DirectX::SimpleMath::Vector3 m_cameraPosition;
	DirectX::SimpleMath::Vector3 m_cameraTarget;

	// ��]
	std::vector<DirectX::SimpleMath::Matrix>  m_rotation;

};

#endif // RUN_PARTICLE_DEFINED