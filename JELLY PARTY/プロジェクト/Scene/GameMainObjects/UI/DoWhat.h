//--------------------------------------------------------------------------------------
// File: DoWhat.h
//
// �J�E���g�_�E���N���X
//
// Date: 2023/12/07
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef DO_WHAT_DEFINED
#define DO_WHAT_DEFINED

class DoWhat
{
public:

	// �R���X�g���N�^
	DoWhat(const float& width, const float& height);

	// �f�X�g���N�^
	~DoWhat();

	// �X�V
	void Update();

	// �`��
	void Render();

private:

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;;

	// �X�P�[�� 
	float m_scale;

	// �����x
	float m_alpha;

	// �A�j���[�V��������
	float m_timer;
};

#endif // DO_WHAT_DEFINED