//--------------------------------------------------------------------------------------
// File: GameTimer.h
//
// �Q�[���̃^�C�}�[�N���X
//
// Date: 2023/11/01
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef GAME_TIMER_DEFINED
#define GAME_TIMER_DEFINED

class GameTimer
{
public:

	// �c�莞�Ԃ̎n�܂�̒l�i�b�j
	static const int TOTAL_SECONDS;
	// �X�v���C�g�̗�
	static const float SPRITE_COLUMN;
	// �X�v���C�g�̍s��
	static const float SPRITE_ROW;

public:

	// �R���X�g���N�^
	GameTimer();

	// �f�X�g���N�^
	~GameTimer();

	// �X�V
	bool Update();
	// �`��
	void Render();

	const int& GetSpendingTime() { return m_spendingTime; }

private:

	// �^�C�}�[��`�悷��
	void RenderTimer(const DirectX::SimpleMath::Vector2& position, const RECT& cutPos);

private:

	// �o�ߎ���(�t���[��)
	int m_spendingTime;

	// �c�莞��
	int m_remainingTime;
	
	// �F
	DirectX::SimpleMath::Color m_color;

	// �傫��
	float m_scale;
};

#endif // GAME_TIMER_DEFINED