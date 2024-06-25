//--------------------------------------------------------------------------------------
// File: DebugCamera.cpp
//
// デバッグ用カメラクラス
//
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "DebugCamera.h"
#include "Mouse.h"

using namespace DirectX;

const float DebugCamera::DEFAULT_CAMERA_DISTANCE = 5.0f;

//--------------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------------
DebugCamera::DebugCamera(int windowWidth, int windowHeight)
	: m_eyePosition(0.0f)
	, m_yAngle(0.0f)
	, m_yTmp(0.0f)
	, m_xAngle(0.0f)
	, m_xTmp(0.0f)
	, m_x(0)
	, m_y(0)
	, m_scrollWheelValue(0)
	, m_screenW(windowWidth)
	, m_screenH(windowHeight)
{
	SetWindowSize(windowWidth, windowHeight);
}

//--------------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------------
void DebugCamera::Update()
{
	auto state = Mouse::Get().GetState();

	// 相対モードなら何もしない
	if (state.positionMode == Mouse::MODE_RELATIVE) return;

	m_tracker.Update(state);

	// マウスの左ボタンが押された
	if (m_tracker.leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	{
		// マウスの座標を取得
		m_x = state.x;
		m_y = state.y;
	}
	else if (m_tracker.leftButton == Mouse::ButtonStateTracker::ButtonState::RELEASED)
	{
		// 現在の回転を保存
		m_xAngle = m_xTmp;
		m_yAngle = m_yTmp;
	}
	// マウスのボタンが押されていたらカメラを移動させる
	if (state.leftButton)
	{
		Motion(state.x, state.y);
	}

	// マウスのフォイール値を取得
	m_scrollWheelValue = state.scrollWheelValue;
	if (m_scrollWheelValue > 0)
	{
		m_scrollWheelValue = 0;
		Mouse::Get().ResetScrollWheelValue();
	}

	// ビュー行列を算出する
	SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_yTmp);
	SimpleMath::Matrix rotX = SimpleMath::Matrix::CreateRotationX(m_xTmp);

	SimpleMath::Matrix rt = rotY * rotX;

	SimpleMath::Vector3 eye(0.0f, 1.0f, 1.0f);
	SimpleMath::Vector3 target(m_target);
	SimpleMath::Vector3 up(0.0f, 1.0f, 0.0f);

	eye = SimpleMath::Vector3::Transform(eye, rt.Invert());
	eye *= (DEFAULT_CAMERA_DISTANCE - m_scrollWheelValue / 100);
	up = SimpleMath::Vector3::Transform(up, rt.Invert());

	auto keystate = Keyboard::Get().GetState();

	//カメラの方向ベクトルから正面方向ベクトルを取得する（Yは不要なので0）
	SimpleMath::Vector3 front = SimpleMath::Vector3(-eye.x, 0.0f, -eye.z);
	//正面方向ベクトルを正規化
	front.Normalize();
	//正規化ベクトルを小さくして移動ベクトルに変換（倍率はデバッグ用なので適当）
	SimpleMath::Vector3 move = front * 0.02f;

	m_eye = eye + m_eyePosition;

	m_view = SimpleMath::Matrix::CreateLookAt(m_eye, m_target, up);
}

//--------------------------------------------------------------------------------------
// 行列の生成
//--------------------------------------------------------------------------------------
void DebugCamera::Motion(int x, int y)
{
	// マウスポインタの位置のドラッグ開始位置からの変位 (相対値)
	float dx = (x - m_x) * m_sx;
	float dy = (y - m_y) * m_sy;

	if (dx != 0.0f || dy != 0.0f)
	{
		// Ｙ軸の回転
		float yAngle = dx * XM_PI;
		// Ｘ軸の回転
		float xAngle = dy * XM_PI;

		m_xTmp = m_xAngle + xAngle;
		m_yTmp = m_yAngle + yAngle;
	}
}

//--------------------------------------------------------------------------------------
// ウィンドウサイズを設定
//--------------------------------------------------------------------------------------
void DebugCamera::SetWindowSize(int windowWidth, int windowHeight)
{
	// 画面サイズに対する相対的なスケールに調整
	m_sx = 1.0f / float(windowWidth);
	m_sy = 1.0f / float(windowHeight);
}


//--------------------------------------------------------------------------------------
// ウィンドウサイズを取得
//--------------------------------------------------------------------------------------
void DebugCamera::GetWindowSize(int & windowWidth, int & windowHeight)
{
	windowWidth = m_screenW;
	windowHeight = m_screenH;
}
