//--------------------------------------------------------------------------------------
// File: Finish.h
//
// カウントダウンクラス
//
// Date: 2023/12/09
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef FINISH_DEFINED
#define FINISH_DEFINED

class Finish
{
public:

	// コンストラクタ
	Finish();

	// デストラクタ
	~Finish();

	// 更新
	bool Update();

	// 描画
	void Render();

private:

	// スケール 
	float m_scale;

	// 透明度
	float m_alpha;

	// アニメーション時間
	float m_timer;
};

#endif // FINISH_DEFINED