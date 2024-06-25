//--------------------------------------------------------------------------------------
// File: GameTimer.h
//
// ゲームのタイマークラス
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

	// 残り時間の始まりの値（秒）
	static const int TOTAL_SECONDS;
	// スプライトの列数
	static const float SPRITE_COLUMN;
	// スプライトの行数
	static const float SPRITE_ROW;

public:

	// コンストラクタ
	GameTimer();

	// デストラクタ
	~GameTimer();

	// 更新
	bool Update();
	// 描画
	void Render();

	const int& GetSpendingTime() { return m_spendingTime; }

private:

	// タイマーを描画する
	void RenderTimer(const DirectX::SimpleMath::Vector2& position, const RECT& cutPos);

private:

	// 経過時間(フレーム)
	int m_spendingTime;

	// 残り時間
	int m_remainingTime;
	
	// 色
	DirectX::SimpleMath::Color m_color;

	// 大きさ
	float m_scale;
};

#endif // GAME_TIMER_DEFINED