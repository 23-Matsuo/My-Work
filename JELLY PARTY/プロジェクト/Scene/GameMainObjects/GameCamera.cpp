//--------------------------------------------------------------------------------------
// File: GameCamera.cpp
//
// �Q�[���J�����N���X
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------


#include "pch.h"

#include "GameCamera.h"
#include "MyLib/Graphics.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
GameCamera::GameCamera(RECT rect)
	:
	m_type(Type::GAMEMAIN)
	, m_playerPos{}
	, m_vel(0.0f) 
{
	// �ˉe�s��̍쐬
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(45.0f),									// ��p
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),	// ��ʏc����
		1.0f,																// �J���������ԋ߂����e��
		225.0f																// �J���������ԉ������e��
	);
	// ���ʂ̃��\�[�X�Ƃ��Đݒ�
	Graphics::GetInstance()->SetProjectionMatrix(m_projection);

	// �f�o�b�O�J�����̍쐬
	m_debugCamera = std::make_unique<DebugCamera>(rect.right, rect.bottom);
}
//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
GameCamera::~GameCamera()
{
	m_debugCamera.reset();
}
//------------------------------------------
// �X�V
//------------------------------------------
void GameCamera::Update(float elapsedTime)
{
	switch (m_type)
	{
	case Type::GAMEMAIN:		// �v���C���[��ǂ�������J����
	{
		DirectX::SimpleMath::Vector3 targetPos = m_playerPos + DirectX::SimpleMath::Vector3(0.0f, 0.0f, -3.0f) * elapsedTime;
		DirectX::SimpleMath::Vector3 eyePos(0.0f, 15.0f, 15.0f);
		SetPositionTarget(targetPos + eyePos, targetPos);

		Graphics::GetInstance()->SetCameraTarget(targetPos);
		Graphics::GetInstance()->SetCameraEye(eyePos);

		break;
	}
	case Type::RESULT:		// ���U���g�J����
	{
		DirectX::SimpleMath::Vector3 targetPos = m_playerPos + DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
		DirectX::SimpleMath::Vector3 eyePos =
			DirectX::SimpleMath::Vector3::Lerp(GetEyePosition(), targetPos + DirectX::SimpleMath::Vector3(0.0f, 2.5f, 12.0f), 0.05f);
		SetPositionTarget(eyePos, targetPos);
		break;
	}
	case Type::DEAD:		// ���񂾂Ƃ��̃J����
	{
		DirectX::SimpleMath::Vector3 targetPos = m_playerPos;
		SetPositionTarget(GetEyePosition(), targetPos);
		break;
	}
	case Type::DEBUG:		// �f�o�b�O�J����
	{
		DirectX::SimpleMath::Vector3 targetPos = m_playerPos + DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);

		m_debugCamera->SetTargetPosition(targetPos);
		m_debugCamera->SetEyePosition(targetPos);
		m_debugCamera->Update();
		
		SetPositionTarget(m_debugCamera->GetEyePosition(), m_debugCamera->GetTargetPosition());
		break;
	}
	default:
		break;
	}

}
