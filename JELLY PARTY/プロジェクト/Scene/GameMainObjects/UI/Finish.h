//--------------------------------------------------------------------------------------
// File: Finish.h
//
// �J�E���g�_�E���N���X
//
// Date: 2023/12/09
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef FINISH_DEFINED
#define FINISH_DEFINED

class Finish
{
public:

	// �R���X�g���N�^
	Finish();

	// �f�X�g���N�^
	~Finish();

	// �X�V
	bool Update();

	// �`��
	void Render();

private:

	// �X�P�[�� 
	float m_scale;

	// �����x
	float m_alpha;

	// �A�j���[�V��������
	float m_timer;
};

#endif // FINISH_DEFINED