//--------------------------------------------------------------------------------------
// File: Camera.h
//
// カメラの基底クラス
//
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef CAMERA_DEFINED
#define CAMERA_DEFINED

class Camera
{
public:

	// コンストラクタ
	Camera();

	// デストラクタ
	virtual ~Camera() {}

	// カメラの位置とターゲットを指定する関数
	void SetPositionTarget(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target);

	// 視点の位置を取得する関数
	const DirectX::SimpleMath::Vector3& GetEyePosition() { return m_eyePosition; }
	// 注目点の位置を取得する関数
	const DirectX::SimpleMath::Vector3& GetTargetPosition() { return m_targetPosition; }
	// ビュー行列を取得
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }


private:

	// 視点の位置
	DirectX::SimpleMath::Vector3 m_eyePosition;
	// 注目点の位置
	DirectX::SimpleMath::Vector3 m_targetPosition;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// カメラの初期位置フラグ
	bool m_initFlag;
};

#endif //CAMERA_DEFINED