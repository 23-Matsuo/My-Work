//--------------------------------------------------------------------------------------
// File: Light.cpp
//
// ステージの床
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Light.h"


//------------------------------------------
// コンストラクタ
//------------------------------------------
Light::Light()
{
	// ライトの位置
	m_position = DirectX::SimpleMath::Vector3(0.0f, 45.0f, 0.0f);

	// ライトの回転
	m_rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(0.0f), DirectX::XMConvertToRadians(90.0f), 0.0f);

	// ライトの色
	m_ambient = DirectX::SimpleMath::Vector3(0.6f, 0.6f, 0.6f);

	// ライトの方向
	DirectX::SimpleMath::Vector3 lightDir = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f), m_rotation);

	// ビュー行列を作成
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(
		m_position,
		m_position + lightDir,
		DirectX::SimpleMath::Vector3::UnitY
	);

	// 射影行列を作成
	m_projection = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		DirectX::XMConvertToRadians(90.0f), 1.0f, 0.1f, 50.0f);
}

//------------------------------------------
// デストラクタ
//------------------------------------------
Light::~Light()
{

}