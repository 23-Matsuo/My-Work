//--------------------------------------------------------------------------------------
// File: HighScoreText.h
//
// �u�n�C�X�R�A�I�v�N���X
//
// Date: 2023/01/04
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef HIGHSCORE_TEXT_DEFINED
#define HIGHSCORE_TEXT_DEFINED

class HighScoreText
{
public:

	// �R���X�g���N�^
	HighScoreText(const float& width, const float& height);

	// �f�X�g���N�^
	~HighScoreText();

	// �X�V
	bool Update();

	// �`��
	void Render();

private:

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;;

	// �X�P�[�� 
	float m_scale;

	// ���̑傫��
	float m_defaultScale;

	// �����x
	float m_alpha;

	// �A�j���[�V��������
	float m_timer;

	// �ʒu
	DirectX::SimpleMath::Vector2 m_position;
};

#endif // HIGHSCORE_TEXT_DEFINED