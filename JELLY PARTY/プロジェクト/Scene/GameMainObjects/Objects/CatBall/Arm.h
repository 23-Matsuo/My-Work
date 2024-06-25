//--------------------------------------------------------------------------------------
// File: Arm.h
//
// ボールの腕クラス
//
//
// Last Update: 2023.12.08
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef ARM_DEFINED
#define ARM_DEFINED


class CatSphere;

class Arm
{
public:
	static const DirectX::SimpleMath::Vector3 SCALE;

	enum class ArmState
	{
		IDLE,
		LOOKUP,
		CLAP,
		LOOKCAMERA
	};
public:

	// コンストラクタ
	Arm(CatSphere* parent);

	// デストラクタ
	~Arm();

	// 初期化
	void Initialize(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& rotation);

	// 更新
	void Update(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& rotation);

	// 描画
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection,
		std::function<void __cdecl()> function,
		const DirectX::SimpleMath::Vector3& scale
	);

	// 後処理
	void Finalize();

	// リザルト時の更新
	void ResultUpdate();

public:

	DirectX::SimpleMath::Vector3 GetPosition() { return m_armLPosition; }

	const float& GetArmLAngle() { return m_armLAngle; }
	const float& GetArmRAngle() { return m_armRAngle; }
	// 腕の回転を設定
	void SetArmLAngle(const float& angle) { m_armLAngle = angle; }
	void SetArmRAngle(const float& angle) { m_armRAngle = angle; }

	// 腕の状態を設定
	void SetArmState(ArmState state)	  { m_state = state; }
	
private:

	void Clap();

private:

	// 腕の状態
	ArmState m_state;

	// モデル
	DirectX::Model* m_armModel;

	// 親
	CatSphere* m_parent;

	// 回転
	DirectX::SimpleMath::Quaternion m_rotation;

	// 位置
	DirectX::SimpleMath::Vector3 m_armLPosition;
	DirectX::SimpleMath::Vector3 m_armRPosition;

	// 回転角
	float m_armLAngle;
	float m_armRAngle;
	// 拍手する時のアングル
	float m_armLClapAngle;
	float m_armRClapAngle;
	// 拍手の向き
	char m_clapLDirection;
	char m_clapRDirection;

};
#endif		// ARM_DEFINED