//--------------------------------------------------------------------------------------
// File: Enemy.h
//
// プレイヤークラス
//
//
// Last Update: 2023.10.31
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef ENEMY_DEFINED
#define ENEMY_DEFINED

#include "MyLib/PhysicalObject.h"
#include "Arm.h"

#include "ImaseLib/ObjCollision.h"
#include "Scene/Effects/LandParticle.h"
#include "Scene/Effects/JumpParticle.h"
#include "Scene/Effects/RunParticle.h"

class Enemy : public PhysicalObject
{
public:

	enum class ResultState
	{
		IDLE,
		LOOKUP,
		CLAP,
		RESULT,
	};

public:

	// デフォルトの大きさ
	const DirectX::SimpleMath::Vector3 DEFAULT_SCALE = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

public:
	// コンストラクタ
	Enemy();

	// デストラクタ
	~Enemy();

	// 初期化
	void Intialize();

	// 更新
	void Update(const DX::StepTimer& timer) override;

	// 描画
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection,
		std::function<void __cdecl()> function
	);

	void RenderEffect();

	// 終了処理
	void Finalize();


public:

	// 反射
	void Reflect(DirectX::SimpleMath::Quaternion pointRotation);

	// 反射
	void UpdateForce(DirectX::SimpleMath::Vector3 force);

	// リザルト時の更新
	void ResultUpdate();


private:

	// 移動
	void Movement(float elapsedTime);

	// キー入力
	void KeyInput(float elapsedTime);

	// 着地アニメーション
	void LandingAnimation();

public:	// セッター
	void SetResultState(const ResultState& state)								{ m_resultState = state; }
	void SetHitFlag(const bool& flag)		 									{ m_isHit = flag; }	
	void SetJumpFlag(const bool& flag)											{ m_isJumped = flag; }
	void SetGravityAc(const float& gravityAc)									{ m_gravityAcceleration = gravityAc; }
	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity)				{ m_velocity = velocity; }

public: // ゲッター
	const DirectX::SimpleMath::Vector3& GetForward() 			{ return m_forward; }
	const float& GetGravityAc()									{ return m_gravityAcceleration; }
	const DirectX::SimpleMath::Vector3& GetVelocity()			{ return m_velocity; }
	const DirectX::SimpleMath::Vector3& GetTotalForce()			{ return m_totalForce; }
	const DirectX::SimpleMath::Quaternion& GetRotation() 		{ return m_rotation; }
	const float& GetJumpForce()									{ return m_jumpForce; }
	const bool& GetIsDead()										{ return m_isDead; }
	const DirectX::BoundingSphere& GetCollider() 				{ return m_collider; }
	const float& GetRadius()									{ return m_radius; }
	DirectX::Model* GetModel()									{ return m_model; }
private:

	// 死亡する位置
	const float DEADLINE = -20.0f;

	// 当たり判定
	DirectX::BoundingSphere m_collider;

private:

	// 加えられた力の合計
	DirectX::SimpleMath::Vector3 m_totalForce;
	// 重力加速度
	float m_gravityAcceleration;
	// ボールの回転
	DirectX::SimpleMath::Quaternion m_rotation;
	// スケール
	DirectX::SimpleMath::Vector3 m_scale;

	// ボールの移動方向
	DirectX::SimpleMath::Vector3 m_forward;
	// ボールの速さ
	DirectX::SimpleMath::Vector3 m_velocity;

	// 影のヒット位置
	DirectX::SimpleMath::Vector3 m_shadowHitPosition;

private:

	// モデルへのポインタ
	DirectX::Model* m_model;

	// アームへのポインタ
	std::unique_ptr<Arm> m_arm;

	// 衝突判定の表示オブジェクトへのポインタ
	std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// パーティクルへのポインタ
	std::unique_ptr<LandParticle> m_landParticle;
	std::unique_ptr<JumpParticle> m_jumpParticle;
	std::unique_ptr<RunParticle> m_runParticle;

	// ジャンプ音
	DirectX::SoundEffectInstance* m_jumpSound;

private:
	// 半径
	float m_radius;

	// ジャンプ力
	float m_jumpForce;
	// ジャンプフラグ
	bool m_isJumped;
	// 衝突フラグ
	bool m_isHit;
	// 死亡フラグ
	bool m_isDead;
	// 着地エフェクト出したか
	bool m_landParticlePlayed;
	// アニメーションしたか
	bool m_animated;

	// アニメーションの時間
	float m_timer;

	// パラメータ
	nlohmann::json m_parameters;

	// リザルト時の状態
	ResultState m_resultState;
};

#endif // ENEMY_DEFINED