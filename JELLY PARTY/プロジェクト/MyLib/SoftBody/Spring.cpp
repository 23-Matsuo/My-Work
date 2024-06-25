//--------------------------------------------------------------------------------------
// File: Spring.cpp
//
// 頂点を繋げるバネクラス
//
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Spring.h"

using namespace DirectX;

//------------------------------------------
// コンストラクタ
//------------------------------------------
Spring::Spring(PhysicalPoint* p1, PhysicalPoint* p2)
	:
	m_distance(0)
	, m_currentDistance(0)
	, m_direction{}
	, m_resist{}
	, m_point1(p1)
	, m_point2(p2)
	, m_fluctuation{}
{
	// 距離を計算
	m_distance = SimpleMath::Vector3::Distance(p1->GetPosition(),p2->GetPosition());

	//パラメータ取得
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/SoftBodyParameter");
	// 弾性率
	m_elasticity = static_cast<float>(m_parameters["SoftBodyParameter"]["SpringParameter"]["ELASTICITY"]);
	// 抵抗率
	m_resistance = static_cast<float>(m_parameters["SoftBodyParameter"]["SpringParameter"]["RESISTANCE"]);

}

//------------------------------------------
// ばねのちから計算
//----------------------------- -------------
void Spring::CalculateForce()
{
	// 頂点のポジションを取得
	const SimpleMath::Vector3 pos1 = m_point1->GetPosition();
	const SimpleMath::Vector3 pos2 = m_point2->GetPosition();

	// 現在の距離を計算
	m_currentDistance = SimpleMath::Vector3::Distance(pos1, pos2);
	
	// ベクトルの向き計算
	m_direction = pos2 - pos1;
	m_direction.Normalize();

	// ばねの力計算 
	// ------------------------------------------------------
	// ばねの方程式( F = -kx )
	// ばねの自然長からの伸び（変位）を求める
	//x > 0 のとき、F は負の向き、x < 0 のとき、F は正の向き
	// ------------------------------------------------------
	
	// ばねの力を求める
	SimpleMath::Vector3 force = -m_direction *
		(m_distance - m_currentDistance) * m_elasticity;
	// 抵抗の力と向き = ばねのベクトルの逆×抵抗率
	m_resist = -m_fluctuation * m_resistance;

	// 力
	m_force1 = force + m_resist;
	// 反発する力
	m_force2 = -force - m_resist;
	// 記憶
	m_fluctuation = force;

	// 力の大きさを制限
	SimpleMath::Vector3 min = -SimpleMath::Vector3::One / 5;
	SimpleMath::Vector3 max = SimpleMath::Vector3::One / 5;

	m_force1.Clamp(min, max);
	m_force2.Clamp(min, max);
	// 力を加える
	AddForce();
}

//------------------------------------------
// それぞれの点に力を加える
//------------------------------------------
void Spring::AddForce()
{
	m_point1->AddForce(m_force1);
	m_point2->AddForce(m_force2);
}