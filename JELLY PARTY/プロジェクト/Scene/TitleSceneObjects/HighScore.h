//--------------------------------------------------------------------------------------
// File: HighScore.h
//
// �ō��X�R�A
//
// Date: 2023/12/30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef HIGH_SCORE_DEFINED
#define HIGH_SCORE_DEFINED

class HighScore
{
public:

	// �R���X�g���N�^
	HighScore(const float& width, const float& height);

	// �f�X�g���N�^
	~HighScore();

	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Render();

	// �����擾
	const int GetHighScore() { return m_score; }

	// ����ݒ�
	void SetHighScore(const int& score) { m_score = score; }

private:

	// �擾��
	int m_score;

	// �����̈ʒu
	DirectX::SimpleMath::Vector2 m_position;

	// �A�j���[�V�����p�����[�^
	nlohmann::json m_data;

	// �X�v���C�g�̗�
	int m_spriteColumn;
	// �X�v���C�g�̍s��
	int m_spriteRow;

	// �X�P�[��
	float m_scale;
};

#endif // HIGH_SCORE_DEFINED