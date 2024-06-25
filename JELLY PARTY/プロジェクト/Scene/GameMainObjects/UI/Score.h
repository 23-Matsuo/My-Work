//--------------------------------------------------------------------------------------
// File: Score.h
//
// �X�R�A�N���X
//
// Date: 2023/12/02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------
#pragma once

#include "HighScoreText.h"
#include "Scene/Effects/ResultConfetti.h"

#ifndef SCORE_DEFINED
#define SCORE_DEFINED

class Score
{
public:

	// �A�j���[�V�����̏��
	enum STATE
	{
		IDLE,
		COUNTUP
	};

	// �V�[���̏��
	enum SceneState
	{
		MAIN,
		RESULT,
	};

public:

	// �R���X�g���N�^
	Score(const float& width, const float& height);

	// �f�X�g���N�^
	~Score();

	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Render();

public:

	// �J�E���g����
	void CountUp();

public:

	// �����擾
	const int GetScore() { return m_score; }

	// ���U���g�̃J�E���g�I���t���O���擾
	const bool GetResultCountFinish() { return m_resultCountFinished; }

	// ����ݒ�
	void SetScore(const int& score) { m_score = score; }

	// �V�[���̏�Ԃ�ݒ�
	void SetSceneState(const SceneState& state) { m_sceneState = state; }

	// ���U���g�̃J�E���g�I���t���O
	void SetResultCountFinish(const bool& flag)	{ m_resultCountFinished = flag; }

private:

	// �J�E���g�A�b�v�̃A�j���[�V����
	void CountUpAnimation();

private:

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;;

	// �擾��
	int m_score;

	// �A�j���[�V�����̏��
	STATE m_state;

	// �V�[���̏��
	SceneState m_sceneState;

private:

	// �X�P�[��
	float m_countScale;
	float m_iconScale;
	float m_backScale;

	// �����̈ʒu
	DirectX::SimpleMath::Vector2 m_countPosition;
	// �A�C�R���̈ʒu
	DirectX::SimpleMath::Vector2 m_iconPosition;
	// �w�i�̈ʒu
	DirectX::SimpleMath::Vector2 m_backPosition;
	// �w�i�̈ʒu
	DirectX::SimpleMath::Vector2 m_highScorePosition;

	// �F
	DirectX::SimpleMath::Color m_color;

private:

	// �A�j���[�V��������
	float m_timer;

	// �A�j���[�V�����p�����[�^
	nlohmann::json m_countAnimation;

	// �X�v���C�g�̗�
	int m_spriteColumn;
	// �X�v���C�g�̍s��
	int m_spriteRow;

	// �X�v���C�g�̉���
	float m_spriteWidth;
	// �X�v���C�g�̏c��
	float m_spriteHeight;

private:

	// ���U���g�̃X�R�A�J�E���g�I���t���O
	bool m_resultCountFinished;
	// �n�C�X�R�A�ɂȂ���
	bool m_isHighScore;
	// �n�C�X�R�A�ɂȂ������̉��t���O
	bool m_highScoreSoundPlayed;

private:

	// �u�n�C�X�R�A�I�v�I�u�W�F�N�g
	std::unique_ptr<HighScoreText> m_highScoreText;

	// �n�C�X�R�A�X�V���̎����቉�o
	std::unique_ptr<ResultConfetti> m_resultConfetti;

	// �n�C�X�R�A���̉�
	DirectX::SoundEffectInstance* m_highScoreSound;

	// ����̉�
	DirectX::SoundEffectInstance* m_clapSound;
};

#endif // SCORE_DEFINED