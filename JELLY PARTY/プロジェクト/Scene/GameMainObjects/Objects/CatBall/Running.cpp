//--------------------------------------------------------------------------------------
// File: Running.cpp
//
// プレイヤーのラン状態クラス
//
// Last Update: 2023.01.15
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "CatSphere.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
CatSphere::Running::Running(CatSphere* pSphere)
	:
	m_pSphere(pSphere)
{
}

//------------------------------------------
// デストラクタ
//------------------------------------------
CatSphere::Running::~Running()
{
}

//------------------------------------------
// 初期化
//------------------------------------------
void CatSphere::Running::Initialize()
{
}

//------------------------------------------
// エフェクト描画
//------------------------------------------
void CatSphere::Running::Render()
{

}

//------------------------------------------
// 更新
//------------------------------------------
void CatSphere::Running::Update(const float& elapsedTime)
{
	auto keyState = Graphics::GetInstance()->GetKeyboardStateTracker();
	auto key = keyState->GetLastState();

	// 移動速度
	float moveSpeed = m_pSphere->GetMoveSpeed() * elapsedTime;

	// 回転に掛かる時間
	float time = 0.05f;

	DirectX::SimpleMath::Vector3 forward = m_pSphere->GetForward();
	DirectX::SimpleMath::Vector3 velocity = m_pSphere->GetVelocity();

	// 前進
	if (key.W || key.Up)
	{
		velocity.z -= moveSpeed;

		forward = DirectX::SimpleMath::Vector3::Lerp(
			forward, DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f), time
		);
	}
	// 後退
	if (key.S || key.Down)
	{
		velocity.z += moveSpeed;

		forward = DirectX::SimpleMath::Vector3::Lerp(
			forward, DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f), time
		);
	}
	// 左
	if (key.A || key.Left)
	{
		velocity.x -= moveSpeed;

		forward = DirectX::SimpleMath::Vector3::Lerp(
			forward, DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f), time
		);
	}
	// 右
	if (key.D || key.Right)
	{
		velocity.x += moveSpeed;

		forward = DirectX::SimpleMath::Vector3::Lerp(
			forward, DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), time
		);
	}

	m_pSphere->SetVelocity(velocity);
	m_pSphere->SetForward(forward);
}