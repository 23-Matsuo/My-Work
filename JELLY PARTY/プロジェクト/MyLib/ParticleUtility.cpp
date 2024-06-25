//--------------------------------------------------------------------------------------
// File: ParticleUtil.cpp
//
// パーティクルユーティリティクラス
// positionや速度などパーティクル１つに必要な要素を格納
//
// Last Update: 2023.10.30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Particleutility.h"

#include "StepTimer.h"
#include <vector>

using namespace DirectX;

//-----------------------------------
// コンストラクタ
//-----------------------------------
ParticleUtility::ParticleUtility(
	float life,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 velocity,
	DirectX::SimpleMath::Vector3 accele,
	DirectX::SimpleMath::Vector3 startScale, DirectX::SimpleMath::Vector3 endScale,
	DirectX::SimpleMath::Color startColor, DirectX::SimpleMath::Color endColor)
{

	m_startLife = m_life = life;

	m_position = pos;
	m_velocity = velocity;
	m_accele = accele;

	m_startScale = m_nowScale = startScale;
	m_endScale = endScale;


	m_startColor = m_nowColor = startColor;
	m_endColor = endColor;
}

//-----------------------------------
// デストラクタ
//-----------------------------------
ParticleUtility::~ParticleUtility()
{
}

//-----------------------------------
/// 更新
//-----------------------------------
bool ParticleUtility::Update(float elapsedTime)
{
	// スケール
	m_nowScale = SimpleMath::Vector3::Lerp(m_startScale, m_endScale, 1.0f - m_life / m_startLife);
	// 色
	m_nowColor = SimpleMath::Color::Lerp(m_startColor, m_endColor, 1.0f - m_life / m_startLife);

	// 速度の計算
	m_velocity += m_accele * elapsedTime;
	// 座標の計算
	m_position -= m_velocity * elapsedTime;

	m_life -= elapsedTime;
	// ライフが０で自身を消してもらう
	if (m_life < 0.0f)
	{
		return false;
	}

	return true;
}
