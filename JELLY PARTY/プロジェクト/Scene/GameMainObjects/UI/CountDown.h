//--------------------------------------------------------------------------------------
// File: CountDown.h
//
// カウントダウンクラス
//
// Date: 2023/11/01
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef COUNTDOWN_DEFINED
#define COUNTDOWN_DEFINED

class CountDown
{
public:

	// コンストラクタ
	CountDown();

	// デストラクタ
	~CountDown();

	// 更新
	bool Update();

	// 描画
	void Render();

private:

	// 最初のカウントダウン
	int m_countDown;

	// スケール 
	float m_scale;

	// 透明度
	float m_alpha;

	// 
	bool m_animated;
};

#endif // COUNTDOWN_DEFINED