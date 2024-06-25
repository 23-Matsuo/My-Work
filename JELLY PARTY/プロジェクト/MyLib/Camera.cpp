//--------------------------------------------------------------------------------------
// File: Camera.cpp
//
// カメラの基底クラス
//
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Camera.h"
#include "Graphics.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
Camera::Camera()
	:
	m_initFlag(true),
	m_eyePosition{},
	m_targetPosition{}
{
}

//------------------------------------------
// カメラの位置と注視点を設定
//------------------------------------------
void Camera::SetPositionTarget(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target)
{
	// すでに初期化しているか確認
	if (m_initFlag)
	{
		// していたらカメラを初期化する
		m_eyePosition = eye;
		m_targetPosition = target;
		m_initFlag = false;
		return;
	}

	// 位置と注視点を計算
	m_eyePosition += (eye - m_eyePosition);
	m_targetPosition += (target - m_targetPosition);

	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(
		m_eyePosition, m_targetPosition, DirectX::SimpleMath::Vector3::UnitY);

	Graphics::GetInstance()->SetViewMatrix(m_view);
}
