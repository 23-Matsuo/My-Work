//--------------------------------------------------------------------------------------
// File: DoWhat.h
//
// カウントダウンクラス
//
// Date: 2023/12/07
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef DO_WHAT_DEFINED
#define DO_WHAT_DEFINED

class DoWhat
{
public:

	// コンストラクタ
	DoWhat(const float& width, const float& height);

	// デストラクタ
	~DoWhat();

	// 更新
	void Update();

	// 描画
	void Render();

private:

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;;

	// スケール 
	float m_scale;

	// 透明度
	float m_alpha;

	// アニメーション時間
	float m_timer;
};

#endif // DO_WHAT_DEFINED