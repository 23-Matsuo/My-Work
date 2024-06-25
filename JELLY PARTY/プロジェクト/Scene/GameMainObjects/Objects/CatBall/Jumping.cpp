//--------------------------------------------------------------------------------------
// File: Jumping.cpp
//
// プレイヤーのジャンプ状態クラス
//
// Last Update: 2023.01.15
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "CatSphere.h"

#include "MyLib/SoundUtility.h"
//------------------------------------------
// コンストラクタ
//------------------------------------------
CatSphere::Jumping::Jumping(CatSphere* pSphere)
	:
	m_pSphere(pSphere)
{
	// ジャンプ音
	m_jumpSound = Resources::GetInstance()->GetSound(L"Jump");
}

//------------------------------------------
// デストラクタ
//------------------------------------------
CatSphere::Jumping::~Jumping()
{
}

//------------------------------------------
// 初期化
//------------------------------------------
void CatSphere::Jumping::Initialize()
{
}

//------------------------------------------
// エフェクト描画
//------------------------------------------
void CatSphere::Jumping::Render()
{

}

//------------------------------------------
// 更新
//------------------------------------------
void CatSphere::Jumping::Update(const float& elapsedTime)
{
	// ジャンプ力を計算
	DirectX::SimpleMath::Vector3 force = m_pSphere->GetTotalForce();
	force.y += m_pSphere->GetJumpForce();
 	m_pSphere->SetTotalForce(force);

	// 音を鳴らす
	PlaySound(m_jumpSound, false);

	// ランニング状態に設定
	m_pSphere->SetCurrentState(m_pSphere->GetRunning());
}