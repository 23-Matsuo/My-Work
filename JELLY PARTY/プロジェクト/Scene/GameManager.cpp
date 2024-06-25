//--------------------------------------------------------------------------------------
// File: GameManager.h
//
// �V���O���g���̋��ʃ��\�[�X
//
// Last Update : 2023.10.23
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "GameManager.h"

std::unique_ptr<GameManager> GameManager::m_gameManager = nullptr;

//------------------------------------------
// GameManager�N���X�̃C���X�^���X���擾����
//------------------------------------------
GameManager* const GameManager::GetInstance()
{
	if (m_gameManager == nullptr)
	{
		// GameManager�N���X�̃C���X�^���X�𐶐�����
		m_gameManager.reset(new GameManager());
	}
	// GameManager�N���X�̃C���X�^���X��Ԃ�
	return m_gameManager.get();
}

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
GameManager::GameManager()
	:
	m_score(0)
	, m_highScore(0)
	, m_titleAnimated(false)
	, m_reStarted(false)
	, m_lastSceneState(TitleScene::SceneState::Title)
{
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
GameManager::~GameManager()
{
}

//------------------------------------------
// �X�R�A�ݒ�
//------------------------------------------
void GameManager::SetNowScore(const int& score)
{
	m_score = score;

	// �ō��X�R�A�Ɣ�r
	if (m_highScore < m_score)
	{
		// �ō��X�R�A���ゾ������ݒ�
		nlohmann::json data;
		data["MAX_SCORE"] = m_score;
		Resources::GetInstance()->WriteJsonFile(data, L"Resources/Datas/ScoreData");
	}
}

