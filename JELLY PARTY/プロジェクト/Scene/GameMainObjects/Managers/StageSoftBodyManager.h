//--------------------------------------------------------------------------------------
// File: StageSoftBodyManager.h
//
// ステージ上のソフトボディ管理クラス
//
// Date: 2023.11.26
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef STAGE_SOFTBODY_MANAGER_DEFINED
#define STAGE_SOFTBODY_MANAGER_DEFINED

#include "MyLib/SoftBody/SoftBodyFactory.h"
#include "Scene/GameMainObjects/Objects/SoftBodyBar.h"
#include "Scene/GameMainObjects/Objects/CenterSoftBody.h"
#include "Scene/GameMainObjects/Objects/SoftBodyCube.h"

class StageSoftBodyManager
{
public:

	enum SceneState
	{
		COUNTDOWN,
		MAIN,
		RESULT,
	};

	// バーの回転速度
	static const float ROTATE_SPEED;

	// 回転の最高速度
	static const float MAX_SPEED;

public:

	// コンストラクタ
	StageSoftBodyManager();

	// デストラクタ
	~StageSoftBodyManager();

	// 更新
	void Update(const int& spendTime);

	// 描画
	void Render();

	// 影描画
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);


public:

	// ゼリーキューブのリザルト時の初期化
	void CubeResultInitialize(Score* pScore);

	// シーンの状態を設定
	void SetSceneState(SceneState state) { m_sceneState = state; }
	
	// カウント終了したかどうか
	const bool& GetCountFinished() { return m_cube->GetCountFinished(); }

private:

	// 中央のやつ
	std::unique_ptr<CenterSoftBody> m_center;

	// バーのやつ
	std::unique_ptr<SoftBodyBar> m_bar;

	// ゼリーキューブのソフトボディ
	std::unique_ptr<SoftBodyCube> m_cube;

private:

	// 回転行列
	DirectX::SimpleMath::Matrix m_rotation;
	// バーの回転速度
	float m_anglarVelocity;

	// シーンの状態
	SceneState m_sceneState;
};

#endif // SOFTBODY_FLOOR_MANAGER_DEFINED