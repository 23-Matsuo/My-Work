//--------------------------------------------------------------------------------------
// File: HighScore.h
//
// 最高スコア
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

	// コンストラクタ
	HighScore(const float& width, const float& height);

	// デストラクタ
	~HighScore();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Render();

	// 数を取得
	const int GetHighScore() { return m_score; }

	// 数を設定
	void SetHighScore(const int& score) { m_score = score; }

private:

	// 取得数
	int m_score;

	// 数字の位置
	DirectX::SimpleMath::Vector2 m_position;

	// アニメーションパラメータ
	nlohmann::json m_data;

	// スプライトの列数
	int m_spriteColumn;
	// スプライトの行数
	int m_spriteRow;

	// スケール
	float m_scale;
};

#endif // HIGH_SCORE_DEFINED