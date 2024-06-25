//--------------------------------------------------------------------------------------
// File: ConfettiUtility.cpp
//
// 紙吹雪ユーティリティクラス
// 
//
// Last Update: 2024.01.04
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "ConfettiUtility.h"

#include "StepTimer.h"
#include <vector>

using namespace DirectX;

//-----------------------------------
// コンストラクタ
//-----------------------------------
ConfettiUtility::ConfettiUtility(
	float life,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 velocity,
	DirectX::SimpleMath::Vector3 accele,
	DirectX::SimpleMath::Vector3 rotate,
	DirectX::SimpleMath::Vector3 scale,
	DirectX::SimpleMath::Color color
)
{

	m_startLife = m_life = life;

	m_position = pos;

	m_velocity = velocity;
	m_accele = accele;

	m_rotate = rotate;

	m_scale = scale;


	m_color = color;
}

//-----------------------------------
// デストラクタ
//-----------------------------------
ConfettiUtility::~ConfettiUtility()
{
}

//-----------------------------------
/// 更新
//-----------------------------------
bool ConfettiUtility::Update(float elapsedTime)
{
	// 速度の計算
	m_velocity += m_accele * elapsedTime;
	// 座標の計算
	m_position -= m_velocity * elapsedTime;

	m_life -= elapsedTime;
	// 回転の計算
	m_rotate.x += 0.05f;
	m_rotate.y += 0.05f;
	m_rotate.z += 0.05f;

	// ライフが０で自身を消してもらう
	if (m_life < 0.0f)
	{
		return false;
	}

	return true;
}
