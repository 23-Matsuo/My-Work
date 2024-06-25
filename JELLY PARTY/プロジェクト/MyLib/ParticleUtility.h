//--------------------------------------------------------------------------------------
// File: ParticleUtil.h
//
// パーティクルユーティリティクラス
// positionや速度などパーティクル１つに必要な要素を格納
//
// Last Update: 2023.10.30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef PARTICLE_UTILITY_DEFINED
#define PARTICLE_UTILITY_DEFINED

#include "StepTimer.h"
#include <vector>
class ParticleUtility
{
public:

	//コンストラクタ
	ParticleUtility(
		float life,									// 生存時間
		DirectX::SimpleMath::Vector3 pos,			// 座標
		DirectX::SimpleMath::Vector3 velocity,		// 速度
		DirectX::SimpleMath::Vector3 accele,		// 加速度
		DirectX::SimpleMath::Vector3 startScale,	// 初期サイズ
		DirectX::SimpleMath::Vector3 endScale,		// 最終サイズ
		DirectX::SimpleMath::Color startColor,		// 初期色
		DirectX::SimpleMath::Color endColor			// 最終色
	);

	//デストラクタ
	~ParticleUtility();

	// 更新
	bool Update(float elapsedTime);

public:	// ゲッター

	const DirectX::SimpleMath::Vector3 GetPosition()	{ return m_position; }
	
	const DirectX::SimpleMath::Vector3 GetVelocity()	{ return m_velocity; }

	const DirectX::SimpleMath::Vector3 GetAccele()		{ return m_accele; }
	
	const DirectX::SimpleMath::Vector3 GetNowScale()	{ return m_nowScale; }
	const DirectX::SimpleMath::Vector3 GetStartScale()	{ return m_startScale; }
	const DirectX::SimpleMath::Vector3 GetEndScale()	{ return m_endScale; }

	const float GetLife()								{ return m_life; }
	const float GetStartLife()							{ return m_startLife; }
	const DirectX::SimpleMath::Color GetNowColor()		{ return m_nowColor; }
	const DirectX::SimpleMath::Color GetStartColor()	{ return m_startColor; }
	const DirectX::SimpleMath::Color GetEndColor()		{ return m_endColor; }

private:

	//座標
	DirectX::SimpleMath::Vector3 m_position;

	//速度
	DirectX::SimpleMath::Vector3 m_velocity;
	//加速度
	DirectX::SimpleMath::Vector3 m_accele;

	//スケール
	DirectX::SimpleMath::Vector3 m_nowScale;
	DirectX::SimpleMath::Vector3 m_startScale;
	DirectX::SimpleMath::Vector3 m_endScale;

	//生存時間
	float m_life;
	float m_startLife;

	//カラー
	DirectX::SimpleMath::Color m_nowColor;
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_endColor;
};

#endif // PARTICLE_UTILITY_DEFINED