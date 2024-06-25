//--------------------------------------------------------------------------------------
// File: Result.h
//
// カウントダウンクラス
//
// Date: 2023/12/09
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef RESULT_DEFINED
#define RESULT_DEFINED

class Result
{
public:

	// コンストラクタ
	Result(const float& width, const float& height);

	// デストラクタ
	~Result();

	// 更新
	bool Update();

	// 描画
	void Render();

private:

	// バネの動き
	void Damping(float* scale, float* velocity, float defaultScale);

private:

	// スケール 
	DirectX::SimpleMath::Vector2 m_windowSize;;

	// スケール 
	DirectX::SimpleMath::Vector2 m_scale;

	// 元の大きさ
	DirectX::SimpleMath::Vector2 m_defaultScale;

	// 透明度
	float m_alpha;

	// アニメーション時間
	float m_timer;

	// 位置
	DirectX::SimpleMath::Vector2 m_position;

	// 加速度
	float m_acceleration;

	// 重力
	float m_gravity;

	// バネの力
	DirectX::SimpleMath::Vector2 m_damping;

};

#endif // RESULT_DEFINED