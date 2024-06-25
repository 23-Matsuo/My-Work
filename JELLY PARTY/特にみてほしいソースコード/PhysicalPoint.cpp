//--------------------------------------------------------------------------------------
// File: PhysicalPoint.h
//
// 頂点に力を加えるクラス
//
// Date: 2023.5.10
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "PhysicalPoint.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
PhysicalPoint::PhysicalPoint(DirectX::SimpleMath::Vector3 pos, const int& ID)
	:
	m_id(ID)
	, m_totalForce{}
	, m_addForce{}
	, m_vector{}
	, m_state(STATE::UNFIXED)
	, m_uv{}
	, m_boundingSphere(pos, 0.1f)
	, m_resetPosition{}
{
	// パラメータ読み込み
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/SoftBodyParameter");
	m_mass = static_cast<float>(m_parameters["SoftBodyParameter"]["PointParameter"]["MASS"]);
	m_gravity = static_cast<float>(m_parameters["SoftBodyParameter"]["PointParameter"]["GRAVITY"]);

	// 重力加速度を設定
	m_gravityAcceleration = m_gravity * m_mass;

	//位置を設定
	SetPosition(pos);
	m_initPosition = pos;
}

//------------------------------------------
/// 更新
//------------------------------------------
void PhysicalPoint::Update(const DX::StepTimer& timer)
{
	m_boundingSphere.Center = GetPosition();

	// もし固定状態なら処理しない
	if (m_state == STATE::FIXED) return;

	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	// 加えられる力の合計を計算
	m_totalForce = m_addForce;
	// 壁とバーは重力を計算しない
	if (m_state == STATE::WALL|| m_state == STATE::BAR)
	{
		m_gravityAcceleration = 0.0f;
	}
	// 重力を計算
	m_totalForce.y -= m_gravityAcceleration;

	// 力を移動量に加算
	m_vector += m_totalForce / m_mass * elapsedTime;

	// 力の上限
	m_vector.Clamp(
		-DirectX::SimpleMath::Vector3::One / 1.75f, 
		 DirectX::SimpleMath::Vector3::One / 1.75f
	);

	// バーならy座標は動かさない
	if (m_state == STATE::BAR)
	{
		m_vector.y = 0.0f;
	}

	// ポジションに加算
	SetPosition(GetPosition() + m_vector);

	// 境界球を頂点の位置に設定
	m_boundingSphere.Center = GetPosition();
	
	// 加えた力を初期化
	m_addForce = DirectX::SimpleMath::Vector3::Zero;
}

//------------------------------------------
// 力を加える
//------------------------------------------
void PhysicalPoint::AddForce(DirectX::SimpleMath::Vector3 force)
{
	m_addForce += force;
}

//------------------------------------------
// ベクトル更新
//------------------------------------------
void PhysicalPoint::UpdateVector(DirectX::SimpleMath::Vector3 force)
{
	m_vector = force;
	m_totalForce.y = 0;
}

//------------------------------------------
// 初期化
//------------------------------------------
void PhysicalPoint::ParameterReset(const DirectX::SimpleMath::Vector3& position)
{
	SetPosition(m_initPosition + position);
	SetResetPosition(GetPosition());

	m_addForce = {};
	m_totalForce = {};
	m_vector = {};
}

//------------------------------------------
// リザルトの初期化
//------------------------------------------
void PhysicalPoint::ResultReset(const DirectX::SimpleMath::Vector3& position)
{
	SetPosition(m_resetPosition + position);

	m_addForce = {};
	m_totalForce = {};
	m_vector = {};
}