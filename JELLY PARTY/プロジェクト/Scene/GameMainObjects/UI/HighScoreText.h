//--------------------------------------------------------------------------------------
// File: HighScoreText.h
//
// 「ハイスコア！」クラス
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

	// コンストラクタ
	HighScoreText(const float& width, const float& height);

	// デストラクタ
	~HighScoreText();

	// 更新
	bool Update();

	// 描画
	void Render();

private:

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;;

	// スケール 
	float m_scale;

	// 元の大きさ
	float m_defaultScale;

	// 透明度
	float m_alpha;

	// アニメーション時間
	float m_timer;

	// 位置
	DirectX::SimpleMath::Vector2 m_position;
};

#endif // HIGHSCORE_TEXT_DEFINED