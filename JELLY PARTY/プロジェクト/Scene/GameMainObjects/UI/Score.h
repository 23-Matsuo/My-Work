//--------------------------------------------------------------------------------------
// File: Score.h
//
// スコアクラス
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

	// アニメーションの状態
	enum STATE
	{
		IDLE,
		COUNTUP
	};

	// シーンの状態
	enum SceneState
	{
		MAIN,
		RESULT,
	};

public:

	// コンストラクタ
	Score(const float& width, const float& height);

	// デストラクタ
	~Score();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Render();

public:

	// カウントする
	void CountUp();

public:

	// 数を取得
	const int GetScore() { return m_score; }

	// リザルトのカウント終了フラグを取得
	const bool GetResultCountFinish() { return m_resultCountFinished; }

	// 数を設定
	void SetScore(const int& score) { m_score = score; }

	// シーンの状態を設定
	void SetSceneState(const SceneState& state) { m_sceneState = state; }

	// リザルトのカウント終了フラグ
	void SetResultCountFinish(const bool& flag)	{ m_resultCountFinished = flag; }

private:

	// カウントアップのアニメーション
	void CountUpAnimation();

private:

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;;

	// 取得数
	int m_score;

	// アニメーションの状態
	STATE m_state;

	// シーンの状態
	SceneState m_sceneState;

private:

	// スケール
	float m_countScale;
	float m_iconScale;
	float m_backScale;

	// 数字の位置
	DirectX::SimpleMath::Vector2 m_countPosition;
	// アイコンの位置
	DirectX::SimpleMath::Vector2 m_iconPosition;
	// 背景の位置
	DirectX::SimpleMath::Vector2 m_backPosition;
	// 背景の位置
	DirectX::SimpleMath::Vector2 m_highScorePosition;

	// 色
	DirectX::SimpleMath::Color m_color;

private:

	// アニメーション時間
	float m_timer;

	// アニメーションパラメータ
	nlohmann::json m_countAnimation;

	// スプライトの列数
	int m_spriteColumn;
	// スプライトの行数
	int m_spriteRow;

	// スプライトの横幅
	float m_spriteWidth;
	// スプライトの縦幅
	float m_spriteHeight;

private:

	// リザルトのスコアカウント終了フラグ
	bool m_resultCountFinished;
	// ハイスコアになった
	bool m_isHighScore;
	// ハイスコアになった時の音フラグ
	bool m_highScoreSoundPlayed;

private:

	// 「ハイスコア！」オブジェクト
	std::unique_ptr<HighScoreText> m_highScoreText;

	// ハイスコア更新時の紙吹雪演出
	std::unique_ptr<ResultConfetti> m_resultConfetti;

	// ハイスコア時の音
	DirectX::SoundEffectInstance* m_highScoreSound;

	// 拍手の音
	DirectX::SoundEffectInstance* m_clapSound;
};

#endif // SCORE_DEFINED