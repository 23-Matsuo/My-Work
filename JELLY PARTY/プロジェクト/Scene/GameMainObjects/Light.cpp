//--------------------------------------------------------------------------------------
// File: Light.cpp
//
// �X�e�[�W�̏�
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Light.h"


//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Light::Light()
{
	// ���C�g�̈ʒu
	m_position = DirectX::SimpleMath::Vector3(0.0f, 45.0f, 0.0f);

	// ���C�g�̉�]
	m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(90.0f), 0.0f);

	// ���C�g�̐F
	m_ambient = DirectX::SimpleMath::Vector3(0.6f, 0.6f, 0.6f);

	// ���C�g�̕���
	DirectX::SimpleMath::Vector3 lightDir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f), m_rotation);

	// �r���[�s����쐬
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(
		m_position,
		m_position + lightDir,
		DirectX::SimpleMath::Vector3::UnitY
	);

	// �ˉe�s����쐬
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(90.0f), 1.0f, 0.1f, 50.0f);
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
Light::~Light()
{

}