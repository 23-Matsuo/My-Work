//--------------------------------------------------------------------------------------
// File: GameCamera.cpp
//
// ゲームカメラクラス
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------


#include "pch.h"

#include "GameCamera.h"
#include "MyLib/Graphics.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
GameCamera::GameCamera(RECT rect)
	:
	m_type(Type::GAMEMAIN)
	, m_playerPos{}
	, m_vel(0.0f) 
{
	// 射影行列の作成
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(45.0f),									// 画角
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),	// 画面縦横比
		1.0f,																// カメラから一番近い投影面
		225.0f																// カメラから一番遠い投影面
	);
	// 共通のリソースとして設定
	Graphics::GetInstance()->SetProjectionMatrix(m_projection);

	// デバッグカメラの作成
	m_debugCamera = std::make_unique<DebugCamera>(rect.right, rect.bottom);
}
//------------------------------------------
// デストラクタ
//------------------------------------------
GameCamera::~GameCamera()
{
	m_debugCamera.reset();
}
//------------------------------------------
// 更新
//------------------------------------------
void GameCamera::Update(float elapsedTime)
{
	switch (m_type)
	{
	case Type::GAMEMAIN:		// プレイヤーを追いかけるカメラ
	{
		DirectX::SimpleMath::Vector3 targetPos = m_playerPos + DirectX::SimpleMath::Vector3(0.0f, 0.0f, -3.0f) * elapsedTime;
		DirectX::SimpleMath::Vector3 eyePos(0.0f, 15.0f, 15.0f);
		SetPositionTarget(targetPos + eyePos, targetPos);

		Graphics::GetInstance()->SetCameraTarget(targetPos);
		Graphics::GetInstance()->SetCameraEye(eyePos);

		break;
	}
	case Type::RESULT:		// リザルトカメラ
	{
		DirectX::SimpleMath::Vector3 targetPos = m_playerPos + DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
		DirectX::SimpleMath::Vector3 eyePos =
			DirectX::SimpleMath::Vector3::Lerp(GetEyePosition(), targetPos + DirectX::SimpleMath::Vector3(0.0f, 2.5f, 12.0f), 0.05f);
		SetPositionTarget(eyePos, targetPos);
		break;
	}
	case Type::DEAD:		// 死んだときのカメラ
	{
		DirectX::SimpleMath::Vector3 targetPos = m_playerPos;
		SetPositionTarget(GetEyePosition(), targetPos);
		break;
	}
	case Type::DEBUG:		// デバッグカメラ
	{
		DirectX::SimpleMath::Vector3 targetPos = m_playerPos + DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);

		m_debugCamera->SetTargetPosition(targetPos);
		m_debugCamera->SetEyePosition(targetPos);
		m_debugCamera->Update();
		
		SetPositionTarget(m_debugCamera->GetEyePosition(), m_debugCamera->GetTargetPosition());
		break;
	}
	default:
		break;
	}

}
