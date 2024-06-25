//--------------------------------------------------------------------------------------
// File: Result.h
//
// �J�E���g�_�E���N���X
//
// Date: 2023/12/09
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef RESULT_DEFINED
#define RESULT_DEFINED

class Result
{
public:

	// �R���X�g���N�^
	Result(const float& width, const float& height);

	// �f�X�g���N�^
	~Result();

	// �X�V
	bool Update();

	// �`��
	void Render();

private:

	// �o�l�̓���
	void Damping(float* scale, float* velocity, float defaultScale);

private:

	// �X�P�[�� 
	DirectX::SimpleMath::Vector2 m_windowSize;;

	// �X�P�[�� 
	DirectX::SimpleMath::Vector2 m_scale;

	// ���̑傫��
	DirectX::SimpleMath::Vector2 m_defaultScale;

	// �����x
	float m_alpha;

	// �A�j���[�V��������
	float m_timer;

	// �ʒu
	DirectX::SimpleMath::Vector2 m_position;

	// �����x
	float m_acceleration;

	// �d��
	float m_gravity;

	// �o�l�̗�
	DirectX::SimpleMath::Vector2 m_damping;

};

#endif // RESULT_DEFINED