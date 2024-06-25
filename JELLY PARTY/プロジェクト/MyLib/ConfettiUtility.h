//--------------------------------------------------------------------------------------
// File: ParticleUtil.h
//
// パーティクルユーティリティクラス
//
//-------------------------------------------------------------------------------------

#pragma once

#ifndef CONFETTI_UTILITY_DEFINED
#define CONFETTI_UTILITY_DEFINED

#include "StepTimer.h"
#include <vector>
class ConfettiUtility
{
public:

	//コンストラクタ
	ConfettiUtility(
		float life,									// 生存時間
		DirectX::SimpleMath::Vector3 pos,			// 座標
		DirectX::SimpleMath::Vector3 velocity,		// 速度
		DirectX::SimpleMath::Vector3 accele,		// 加速度
		DirectX::SimpleMath::Vector3 rotate,		// 各軸の回転角
		DirectX::SimpleMath::Vector3 scale,			// スケール
		DirectX::SimpleMath::Color color			// 色
	);


	//デストラクタ
	~ConfettiUtility();

	// 更新
	bool Update(float elapsedTime);

public:	// ゲッター

	const DirectX::SimpleMath::Vector3 GetPosition()	{ return m_position; }
	
	const DirectX::SimpleMath::Vector3 GetVelocity()	{ return m_velocity; }

	const DirectX::SimpleMath::Vector3 GetAccele()		{ return m_accele; }

	const DirectX::SimpleMath::Vector3 GetRotation()	{ return m_rotate; }
	
	const DirectX::SimpleMath::Vector3 GetScale()		{ return m_scale; }

	const float GetLife()								{ return m_life; }
	const float GetStartLife()							{ return m_startLife; }
	const DirectX::SimpleMath::Color GetColor()			{ return m_color; }

private:

	// 座標
	DirectX::SimpleMath::Vector3 m_position;

	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;
	// 加速度
	DirectX::SimpleMath::Vector3 m_accele;

	// 各軸の回転角
	DirectX::SimpleMath::Vector3 m_rotate;

	// スケール
	DirectX::SimpleMath::Vector3 m_scale;

	// 生存時間
	float m_life;
	float m_startLife;

	// カラー
	DirectX::SimpleMath::Color m_color;
};

#endif // CONFETTI_UTILITY_DEFINED