//--------------------------------------------------------------------------------------
// File: GameManager.h
//
// シングルトンの共通リソース
//
// Last Update: 2023.12.09
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#pragma once

#ifndef GAME_MANAGER_DEFINED
#define GAME_MANAGER_DEFINED

#include "TitleScene.h"
#include "PlayScene.h"

// GameManagerクラスを定義する
class GameManager
{

public:
	// Resoucesクラスのインスタンスを取得する
	static GameManager* const GetInstance();

	// デストラクタ
	~GameManager();

private:

	// コンストラクタ
	GameManager();
	// 代入は許容しない
	void operator=(const GameManager& object) = delete;
	// コピーコンストラクタは許容しない
	GameManager(const GameManager& object) = delete;

public: // セッター

	void SetHighScore(const int& score)							{ m_highScore = score; }
	void SetNowScore(const int& score);
	void SetTitleAnimated(const bool& flag)						{ m_titleAnimated = flag; }
	void SetReStarted(const bool& flag)							{ m_reStarted = flag; }
	void SetTitleLastState(const TitleScene::SceneState& state) { m_lastSceneState = state; }

public: // ゲッター

	const int  GetHighScore()					 { return m_highScore; }
	const int  GetNowScore()					 { return m_score; }
	const bool GetTitleAnimated()				 { return m_titleAnimated; }
	const bool GetReStarted()					 { return m_reStarted; }
	const TitleScene::SceneState& GetLastState() { return m_lastSceneState; }
	
private:

	// GameManagerクラスのインスタンスへのポインタ
	static std::unique_ptr<GameManager> m_gameManager;

private:

	// 最高スコア
	int m_highScore;

	// スコア
	int m_score;

	// タイトルアニメーションしたか
	bool m_titleAnimated;

	// リスタートされたか
	bool m_reStarted;

	// タイトル最終ステート
	TitleScene::SceneState m_lastSceneState;
};

#endif // GAME_MANAGER_DEFINED