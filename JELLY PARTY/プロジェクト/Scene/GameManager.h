//--------------------------------------------------------------------------------------
// File: GameManager.h
//
// �V���O���g���̋��ʃ��\�[�X
//
// Last Update: 2023.12.09
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#pragma once

#ifndef GAME_MANAGER_DEFINED
#define GAME_MANAGER_DEFINED

#include "TitleScene.h"
#include "PlayScene.h"

// GameManager�N���X���`����
class GameManager
{

public:
	// Resouces�N���X�̃C���X�^���X���擾����
	static GameManager* const GetInstance();

	// �f�X�g���N�^
	~GameManager();

private:

	// �R���X�g���N�^
	GameManager();
	// ����͋��e���Ȃ�
	void operator=(const GameManager& object) = delete;
	// �R�s�[�R���X�g���N�^�͋��e���Ȃ�
	GameManager(const GameManager& object) = delete;

public: // �Z�b�^�[

	void SetHighScore(const int& score)							{ m_highScore = score; }
	void SetNowScore(const int& score);
	void SetTitleAnimated(const bool& flag)						{ m_titleAnimated = flag; }
	void SetReStarted(const bool& flag)							{ m_reStarted = flag; }
	void SetTitleLastState(const TitleScene::SceneState& state) { m_lastSceneState = state; }

public: // �Q�b�^�[

	const int  GetHighScore()					 { return m_highScore; }
	const int  GetNowScore()					 { return m_score; }
	const bool GetTitleAnimated()				 { return m_titleAnimated; }
	const bool GetReStarted()					 { return m_reStarted; }
	const TitleScene::SceneState& GetLastState() { return m_lastSceneState; }
	
private:

	// GameManager�N���X�̃C���X�^���X�ւ̃|�C���^
	static std::unique_ptr<GameManager> m_gameManager;

private:

	// �ō��X�R�A
	int m_highScore;

	// �X�R�A
	int m_score;

	// �^�C�g���A�j���[�V����������
	bool m_titleAnimated;

	// ���X�^�[�g���ꂽ��
	bool m_reStarted;

	// �^�C�g���ŏI�X�e�[�g
	TitleScene::SceneState m_lastSceneState;
};

#endif // GAME_MANAGER_DEFINED