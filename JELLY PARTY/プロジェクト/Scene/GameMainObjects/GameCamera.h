//--------------------------------------------------------------------------------------
// File: GameCamera.h
//
// ゲームカメラクラス
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef GAME_CAMERA_DEFINED
#define GAME_CAMERA_DEFINED

#include "MyLib/Camera.h"
#include "MyLib/DebugCamera.h"

class GameCamera : public Camera
{
public:
	// カメラのタイプ
	enum class Type
	{
		GAMEMAIN,
		DEAD,
		RESULT,

		DEBUG,

	};
public:
	// コンストラクタ
	GameCamera(RECT rect);
	// デストラクタ
	~GameCamera();

	// 更新
	void Update(float elapsedTime);

public:
	// プレイヤーのポジション設定
	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& pos)	{ m_playerPos = pos; }
	
	// カメラのタイプを設定する
	void SetType(Type type) { m_type = type; }
public:

	// 射影行列を取得
	DirectX::SimpleMath::Matrix GetProjection() const { return m_projection; }
private:

	// 現在のカメラタイプ
	Type m_type;

	// デバッグカメラへのポインタ
	std::unique_ptr<DebugCamera> m_debugCamera;

	// プレイヤーの位置
	DirectX::SimpleMath::Vector3 m_playerPos;

	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;

	float m_vel;

};

#endif // GAME_CAMERA_DEFINED