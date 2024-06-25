//--------------------------------------------------------------------------------------
// File: Light.h
//
// ステージの床
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------
#pragma once

#ifndef LIGHT_DEFINED
#define LIGHT_DEFINED

#include "DeviceResources.h"


class Light
{
public:
	
	// コンストラクタ
	Light();

	// デストラクタ
	~Light();

public:

	// 位置を取得
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	// 回転を取得
	DirectX::SimpleMath::Quaternion GetRotation() { return m_rotation; }

	// 色を取得
	DirectX::SimpleMath::Vector3 GetAmbient() { return m_ambient; }

	// ビュー行列を取得
	DirectX::SimpleMath::Matrix GetView() { return m_view; }

	// 射影行列を取得
	DirectX::SimpleMath::Matrix GetProjection() { return m_projection; }
	

private:

	// ポジション
	DirectX::SimpleMath::Vector3 m_position;

	// ライトの回転
	DirectX::SimpleMath::Quaternion m_rotation;

	// ライトの色
	DirectX::SimpleMath::Vector3 m_ambient;

	// ライトビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// ライト射影行列
	DirectX::SimpleMath::Matrix m_projection;
};

#endif // LIGHT_DEFINED