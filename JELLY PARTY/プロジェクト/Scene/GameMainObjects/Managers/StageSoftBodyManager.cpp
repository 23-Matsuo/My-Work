//--------------------------------------------------------------------------------------
// File: StageSoftBodyManager.cpp
//
// ステージ上のソフトボディ管理クラス
//
// Date: 2023.10.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------


#include "pch.h"
#include "StageSoftBodyManager.h"
#include "MyLib/ObjectCollision.h"

// 範囲制限関数
template<class T> T Clamp(T value, T min, T max)
{
	if (value <= min)
	{
		return min;
	}
	else if (value >= max)
	{
		return max;
	}
	return value;
}

// バーの回転速度
const float StageSoftBodyManager::ROTATE_SPEED = 0.0008f;

// 回転の最高速度
const float StageSoftBodyManager::MAX_SPEED = 0.0425f;

//------------------------------------------
// コンストラクタ
//------------------------------------------
StageSoftBodyManager::StageSoftBodyManager()
	:
	m_anglarVelocity(0.0f)
	, m_sceneState(SceneState::COUNTDOWN)
{
	// 中央の軸作成
	m_center = std::make_unique<CenterSoftBody>();
	// バー作成
	m_bar = std::make_unique<SoftBodyBar>(m_center->GetJointPoints());
	// ゼリーキューブ作成
	m_cube = std::make_unique<SoftBodyCube>();
}

//------------------------------------------
// デストラクタ
//------------------------------------------
StageSoftBodyManager::~StageSoftBodyManager()
{
}

//------------------------------------------
// 更新
//------------------------------------------
void StageSoftBodyManager::Update(const int& spendTime)
{
	// リザルトならゼリーキューブ以外処理しない
	if (m_sceneState == SceneState::RESULT) 
	{
		m_cube->ResultUpdate();
		return;
	} 

	// 回転の速さを計算
	m_anglarVelocity += DirectX::XMConvertToRadians(ROTATE_SPEED);
	// 回転の速さを制限
	m_anglarVelocity = Clamp(m_anglarVelocity, 0.0f, MAX_SPEED);
	// 回転行列を作成
	m_rotation = DirectX::SimpleMath::Matrix::CreateRotationY(m_anglarVelocity);

	// ゼリーキューブの更新
	m_cube->Update(spendTime, m_center.get());

	// 中央の軸更新
	m_center->Update(m_rotation);

	// バーの更新
	m_bar->Update(m_rotation, m_center->GetCylinder(), m_anglarVelocity);
}

//------------------------------------------
// 描画
//------------------------------------------
void StageSoftBodyManager::Render()
{

	// リザルトならゼリーキューブ以外描画しない
	if (m_sceneState == SceneState::RESULT)
	{
		m_cube->ResultRender();
		return;
	}

	if (m_sceneState != COUNTDOWN)
	{
		// ゼリーキューブの描画
		m_cube->Render();
	}

	// バーの描画
	m_bar->Render();

	// 中央の軸描画
	m_center->Render();
}


//------------------------------------------
// 描画
//------------------------------------------
void StageSoftBodyManager::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	if (m_sceneState == SceneState::RESULT && m_cube->GetCountFinished())
	{
		// リザルトかつスコアカウントが終わっていたら
		// 影の描画
		m_cube->ResultShadowRender(lightView, lightProjection);
		return;
	}
	else if (m_sceneState == SceneState::RESULT && !m_cube->GetCountFinished())
	{
		// リザルトかつスコアカウントが終わっていない
		return;
	}

	if (m_sceneState != COUNTDOWN)
	{
		// ゼリーキューブの描画
		m_cube->ShadowRender(lightView, lightProjection);
	}

	// バーの描画
	m_bar->ShadowRender(lightView, lightProjection);

	// 中央の軸描画
	m_center->ShadowRender(lightView, lightProjection);
}


//------------------------------------------
// リザルト時の初期化
//------------------------------------------
void StageSoftBodyManager::CubeResultInitialize(Score* pScore)
{
	m_cube->ResultInitialize(pScore);
}