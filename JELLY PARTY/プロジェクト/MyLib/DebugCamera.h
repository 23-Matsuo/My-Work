//--------------------------------------------------------------------------------------
// File: DebugCamera.h
//
// デバッグ用カメラクラス
// 
//--------------------------------------------------------------------------------------
#pragma once
#ifndef DEBUG_CAMERA_DEFINED
#define DEBUG_CAMERA_DEFINED

#include "SimpleMath.h"

// デバッグ用カメラクラス
class DebugCamera
{
	// カメラの距離
	static const float DEFAULT_CAMERA_DISTANCE;

	// 横回転
	float m_yAngle, m_yTmp;

	// 縦回転
	float m_xAngle, m_xTmp;

	// ドラッグされた座標
	int m_x, m_y;

	float m_sx, m_sy;

	// 生成されたビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// スクロールフォイール値
	int m_scrollWheelValue;

	// 視点
	DirectX::SimpleMath::Vector3 m_eye;
	DirectX::SimpleMath::Vector3 m_eyePosition;

	// 注視点
	DirectX::SimpleMath::Vector3 m_target;

	// マウストラッカー
	DirectX::Mouse::ButtonStateTracker m_tracker;

	// スクリーンサイズ
	int m_screenW, m_screenH;

private:

	void Motion(int x, int y);

public:
	// コンストラクタ
	DebugCamera(int windowWidth, int windowHeight);

	// デバッグカメラの更新
	void Update();

public:
	// デバッグカメラのビュー行列の取得関数
	DirectX::SimpleMath::Matrix GetViewMatrix() const { return m_view;	}
	// デバッグカメラの位置の取得関数
	DirectX::SimpleMath::Vector3 GetEyePosition() const { return m_eye; }
	// デバッグカメラの注視点の取得関数
	DirectX::SimpleMath::Vector3 GetTargetPosition() const { return m_target; }
	// デバッグカメラの注視点の設定
	void SetTargetPosition(DirectX::SimpleMath::Vector3 target){ m_target = target;	}
	// デバッグカメラの注視点の設定
	void SetEyePosition(DirectX::SimpleMath::Vector3 eye){ m_eyePosition = eye;	}
	

	// ウィンドウサイズを設定
	void SetWindowSize(int windowWidth, int windowHeight);

	// ウィンドウサイズを取得
	void GetWindowSize(int& windowWidth, int& windowHeight);
};

#endif // DEBUG_CAMERA_DEFINED