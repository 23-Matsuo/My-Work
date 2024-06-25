//--------------------------------------------------------------------------------------
// File: LandParticle.h
//
// ���n�p�[�e�B�N���N���X
//
// 
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef LAND_PARTICLE_DEFINED
#define LAND_PARTICLE_DEFINED

#include "StepTimer.h"
#include <vector>
#include <list>

#include "MyLib/ParticleUtility.h"

class LandParticle
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

	enum class State
	{
		IDLE,
		PLAY
	};

public:
	// �C���v�b�g���C�A�E�g
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// �R���X�g���N�^
	LandParticle();

	// �f�X�g���N�^
	~LandParticle();

	// ������
	void Create(DX::DeviceResources* pDR);

	// �X�V
	void Update(const DX::StepTimer& timer);

	// �`��
	void Render();

	// �r���{�[�h�쐬
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 up);

	void SetState(State state) { m_state = state; }
	State GetState() { return m_state; }

	// �p�[�e�B�N���쐬
	void CreateLandParticle(DirectX::SimpleMath::Vector3 position);

private:

	void CreateShader();

private:

	DX::DeviceResources* m_pDR;

	State m_state;

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
	std::list<ParticleUtility> m_particleUtility;

	DirectX::SimpleMath::Vector3 m_cameraPosition;
	DirectX::SimpleMath::Vector3 m_cameraTarget;

};

#endif // LAND_PARTICLE_DEFINED