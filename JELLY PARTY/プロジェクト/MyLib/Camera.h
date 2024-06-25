//--------------------------------------------------------------------------------------
// File: Camera.h
//
// �J�����̊��N���X
//
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef CAMERA_DEFINED
#define CAMERA_DEFINED

class Camera
{
public:

	// �R���X�g���N�^
	Camera();

	// �f�X�g���N�^
	virtual ~Camera() {}

	// �J�����̈ʒu�ƃ^�[�Q�b�g���w�肷��֐�
	void SetPositionTarget(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target);

	// ���_�̈ʒu���擾����֐�
	const DirectX::SimpleMath::Vector3& GetEyePosition() { return m_eyePosition; }
	// ���ړ_�̈ʒu���擾����֐�
	const DirectX::SimpleMath::Vector3& GetTargetPosition() { return m_targetPosition; }
	// �r���[�s����擾
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }


private:

	// ���_�̈ʒu
	DirectX::SimpleMath::Vector3 m_eyePosition;
	// ���ړ_�̈ʒu
	DirectX::SimpleMath::Vector3 m_targetPosition;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	// �J�����̏����ʒu�t���O
	bool m_initFlag;
};

#endif //CAMERA_DEFINED