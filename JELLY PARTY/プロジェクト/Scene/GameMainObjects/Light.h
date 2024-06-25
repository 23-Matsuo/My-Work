//--------------------------------------------------------------------------------------
// File: Light.h
//
// �X�e�[�W�̏�
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------
#pragma once

#ifndef LIGHT_DEFINED
#define LIGHT_DEFINED

#include "DeviceResources.h"


class Light
{
public:
	
	// �R���X�g���N�^
	Light();

	// �f�X�g���N�^
	~Light();

public:

	// �ʒu���擾
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	// ��]���擾
	DirectX::SimpleMath::Quaternion GetRotation() { return m_rotation; }

	// �F���擾
	DirectX::SimpleMath::Vector3 GetAmbient() { return m_ambient; }

	// �r���[�s����擾
	DirectX::SimpleMath::Matrix GetView() { return m_view; }

	// �ˉe�s����擾
	DirectX::SimpleMath::Matrix GetProjection() { return m_projection; }
	

private:

	// �|�W�V����
	DirectX::SimpleMath::Vector3 m_position;

	// ���C�g�̉�]
	DirectX::SimpleMath::Quaternion m_rotation;

	// ���C�g�̐F
	DirectX::SimpleMath::Vector3 m_ambient;

	// ���C�g�r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	// ���C�g�ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
};

#endif // LIGHT_DEFINED