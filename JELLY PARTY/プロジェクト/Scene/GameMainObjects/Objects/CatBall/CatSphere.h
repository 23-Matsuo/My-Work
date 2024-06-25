//--------------------------------------------------------------------------------------
// File: Sphere.h
//
// プレイヤークラス
//
//
// Last Update: 2023.10.31
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef SPHERE_DEFINED
#define SPHERE_DEFINED

#include "MyLib/PhysicalObject.h"
#include "Arm.h"

#include "ImaseLib/ObjCollision.h"
#include "Scene/Effects/LandParticle.h"
#include "Scene/Effects/JumpParticle.h"
#include "Scene/Effects/RunParticle.h"

#include "MyLib/IPlayerState.h"

class PhysicalObject;

class CatSphere : public PhysicalObject
{
public:

	class Running;
	class Jumping;

	Running* GetRunning() { return &m_running; }
	Jumping* GetJumping() { return &m_jumping; }

public:

	// ランニングステートクラス
	class Running : public IPlayerState
	{
	public:
		// コンストラクタ
		Running(CatSphere* pSphere);

		// デストラクタ
		~Running();

		// 初期化する
		inline void Initialize() override;

		// 描画
		inline void Render() override;

		// キー入力
		inline void Update(const float& elapsedTime) override;

	private:

		CatSphere* m_pSphere;
	};

public:

	// ジャンプステートクラス
	class Jumping : public IPlayerState
	{
	public:
		// コンストラクタ
		Jumping(CatSphere* pSphere);

		// デストラクタ
		~Jumping();

		// 初期化する
		inline void Initialize() override;

		// 描画
		inline void Render() override;

		// キー入力
		inline void Update(const float& elapsedTime) override;

	private:

		CatSphere* m_pSphere;

		// ジャンプ音
		DirectX::SoundEffectInstance* m_jumpSound;
	};

private:
	
	// ランニング状態
	Running m_running;
	// ジャンプ状態
	Jumping m_jumping;

public:

	enum class ResultState
	{
		IDLE,
		LOOKUP,
		CLAP,
		RESULT,
	};

public:
	// コンストラクタ
	CatSphere();

	// デストラクタ
	~CatSphere();

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
	void SetGravityAc(const float& gravityAc)									{ m_gravityAcceleration = gravityAc; }
	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity)				{ m_velocity = velocity; }
	void SetTotalForce(const DirectX::SimpleMath::Vector3& force)				{ m_totalForce = force; }
	void SetForward(const DirectX::SimpleMath::Vector3& forward)				{ m_forward = forward; }
	void SetCurrentState(IPlayerState* currentState)							{ m_currentState = currentState; }

public: // ゲッター
	const DirectX::SimpleMath::Vector3& GetForward() 			{ return m_forward; }
	const float& GetGravityAc()									{ return m_gravityAcceleration; }
	const float& GetMoveSpeed()									{ return m_moveSpeed; }
	const DirectX::SimpleMath::Vector3& GetVelocity()			{ return m_velocity; }
	const DirectX::SimpleMath::Vector3& GetTotalForce()			{ return m_totalForce; }
	const DirectX::SimpleMath::Quaternion& GetRotation() 		{ return m_rotation; }
	const float& GetJumpForce()									{ return m_jumpForce; }
	const bool& GetIsDead()										{ return m_isDead; }
	const DirectX::BoundingSphere& GetCollider() 				{ return m_collider; }
	const float& GetRadius()									{ return m_radius; }
	DirectX::Model* GetModel()									{ return m_model; }
	const IPlayerState* GetCurrentState()						{ return m_currentState; }

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
	// 初期スケール
	DirectX::SimpleMath::Vector3 m_defaultScale;

	// ボールの移動方向
	DirectX::SimpleMath::Vector3 m_forward;
	// ボールの速さ
	DirectX::SimpleMath::Vector3 m_velocity;

	// 動く速さ
	float m_moveSpeed;
	// ジャンプ力
	float m_jumpForce;
	// 摩擦
	float m_fricition;

private:

	// 現在の状態
	IPlayerState* m_currentState;

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

private:

	// 半径
	float m_radius;

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

#endif // SPHERE_DEFINED