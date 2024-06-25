//--------------------------------------------------------------------------------------
// File: GameManager.h
//
// シングルトンの共通リソース
//
// Last Update : 2023.10.23
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "GameManager.h"

std::unique_ptr<GameManager> GameManager::m_gameManager = nullptr;

//------------------------------------------
// GameManagerクラスのインスタンスを取得する
//------------------------------------------
GameManager* const GameManager::GetInstance()
{
	if (m_gameManager == nullptr)
	{
		// GameManagerクラスのインスタンスを生成する
		m_gameManager.reset(new GameManager());
	}
	// GameManagerクラスのインスタンスを返す
	return m_gameManager.get();
}

//------------------------------------------
// コンストラクタ
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
// デストラクタ
//------------------------------------------
GameManager::~GameManager()
{
}

//------------------------------------------
// スコア設定
//------------------------------------------
void GameManager::SetNowScore(const int& score)
{
	m_score = score;

	// 最高スコアと比較
	if (m_highScore < m_score)
	{
		// 最高スコアより上だったら設定
		nlohmann::json data;
		data["MAX_SCORE"] = m_score;
		Resources::GetInstance()->WriteJsonFile(data, L"Resources/Datas/ScoreData");
	}
}

