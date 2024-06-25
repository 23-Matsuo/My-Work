//--------------------------------------------------------------------------------------
// File: Camera.cpp
//
// �J�����̊��N���X
//
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Camera.h"
#include "Graphics.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Camera::Camera()
	:
	m_initFlag(true),
	m_eyePosition{},
	m_targetPosition{}
{
}

//------------------------------------------
// �J�����̈ʒu�ƒ����_��ݒ�
//------------------------------------------
void Camera::SetPositionTarget(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target)
{
	// ���łɏ��������Ă��邩�m�F
	if (m_initFlag)
	{
		// ���Ă�����J����������������
		m_eyePosition = eye;
		m_targetPosition = target;
		m_initFlag = false;
		return;
	}

	// �ʒu�ƒ����_���v�Z
	m_eyePosition += (eye - m_eyePosition);
	m_targetPosition += (target - m_targetPosition);

	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(
		m_eyePosition, m_targetPosition, DirectX::SimpleMath::Vector3::UnitY);

	Graphics::GetInstance()->SetViewMatrix(m_view);
}
