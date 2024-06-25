//--------------------------------------------------------------------------------------
// File: Running.cpp
//
// �v���C���[�̃�����ԃN���X
//
// Last Update: 2023.01.15
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "CatSphere.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
CatSphere::Running::Running(CatSphere* pSphere)
	:
	m_pSphere(pSphere)
{
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
CatSphere::Running::~Running()
{
}

//------------------------------------------
// ������
//------------------------------------------
void CatSphere::Running::Initialize()
{
}

//------------------------------------------
// �G�t�F�N�g�`��
//------------------------------------------
void CatSphere::Running::Render()
{

}

//------------------------------------------
// �X�V
//------------------------------------------
void CatSphere::Running::Update(const float& elapsedTime)
{
	auto keyState = Graphics::GetInstance()->GetKeyboardStateTracker();
	auto key = keyState->GetLastState();

	// �ړ����x
	float moveSpeed = m_pSphere->GetMoveSpeed() * elapsedTime;

	// ��]�Ɋ|���鎞��
	float time = 0.05f;

	DirectX::SimpleMath::Vector3 forward = m_pSphere->GetForward();
	DirectX::SimpleMath::Vector3 velocity = m_pSphere->GetVelocity();

	// �O�i
	if (key.W || key.Up)
	{
		velocity.z -= moveSpeed;

		forward = DirectX::SimpleMath::Vector3::Lerp(
			forward, DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f), time
		);
	}
	// ���
	if (key.S || key.Down)
	{
		velocity.z += moveSpeed;

		forward = DirectX::SimpleMath::Vector3::Lerp(
			forward, DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f), time
		);
	}
	// ��
	if (key.A || key.Left)
	{
		velocity.x -= moveSpeed;

		forward = DirectX::SimpleMath::Vector3::Lerp(
			forward, DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f), time
		);
	}
	// �E
	if (key.D || key.Right)
	{
		velocity.x += moveSpeed;

		forward = DirectX::SimpleMath::Vector3::Lerp(
			forward, DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), time
		);
	}

	m_pSphere->SetVelocity(velocity);
	m_pSphere->SetForward(forward);
}