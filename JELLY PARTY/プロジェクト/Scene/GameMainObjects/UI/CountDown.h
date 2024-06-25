//--------------------------------------------------------------------------------------
// File: CountDown.h
//
// �J�E���g�_�E���N���X
//
// Date: 2023/11/01
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef COUNTDOWN_DEFINED
#define COUNTDOWN_DEFINED

class CountDown
{
public:

	// �R���X�g���N�^
	CountDown();

	// �f�X�g���N�^
	~CountDown();

	// �X�V
	bool Update();

	// �`��
	void Render();

private:

	// �ŏ��̃J�E���g�_�E��
	int m_countDown;

	// �X�P�[�� 
	float m_scale;

	// �����x
	float m_alpha;

	// 
	bool m_animated;
};

#endif // COUNTDOWN_DEFINED