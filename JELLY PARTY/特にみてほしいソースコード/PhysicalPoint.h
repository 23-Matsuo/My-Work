//--------------------------------------------------------------------------------------
// File: PhysicalPoint.h
//
// 頂点に力を加えるクラス
// 
// Date: 2023.5.10
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef PHYSICAL_POINT_DEFINED
#define PHYSICAL_POINT_DEFINED

#include "MyLib/PhysicalObject.h"

#include "ImaseLib/ObjCollision.h"
class PhysicalPoint : public PhysicalObject
{
public:
	enum class STATE
	{
		FIXED,		// 固定
		UNFIXED,	// 固定しない
		WALL,		// 壁
		BAR,		// バー
	};
public:
	// コンストラクタ
	PhysicalPoint(DirectX::SimpleMath::Vector3 pos, const int& ID);

	// デストラクタ
	~PhysicalPoint() {};

	// 更新
	void Update(const DX::StepTimer& timer) override;

	// 力を加算する関数
	void AddForce(DirectX::SimpleMath::Vector3 force);

	// 反射
	void UpdateVector(DirectX::SimpleMath::Vector3 force);


	// パラメータ初期化
	void ParameterReset(const DirectX::SimpleMath::Vector3& position);

	// リセットを設定
	void ResultReset(const DirectX::SimpleMath::Vector3& position);

public:
	// UV座標を取得
	const DirectX::SimpleMath::Vector2 GetUV() 				{ return m_uv; }
	// 状態を取得
	const STATE GetState() 									{ return m_state; }
	// 移動量を取得
	const DirectX::SimpleMath::Vector3 GetVector() 			{ return m_vector; }
	// 初期位置を取得
	const DirectX::SimpleMath::Vector3 GetInitPosition()	{ return m_initPosition; }
	// バウンディングスフィア取得
	const DirectX::BoundingSphere GetBoundingSphere()		{ return m_boundingSphere; }
	// 半径取得
	const float GetRadius() 								{ return static_cast<float>(m_parameters["SoftBodyParameter"]["PointParameter"]["RADIUS"]); }
public:

	// 状態を設定
	void SetState(const STATE& state) { m_state = state; }
	// UV座標を設定
	void SetUV(const DirectX::SimpleMath::Vector2& uv) { m_uv = uv; }
	// リセット位置を設定
	void SetResetPosition(const DirectX::SimpleMath::Vector3& position) { m_resetPosition = position; }
	// 衝突判定オブジェクトのサイズを設定
	void SetColliderSize(const float& radius) { m_boundingSphere.Radius = radius; }

private:

	// 頂点の状態
	STATE m_state;
	
	// ベクトル
	DirectX::SimpleMath::Vector3 m_vector;

	// 重力加速度
	float m_gravityAcceleration;

	// 質量
	float m_mass;
	// 重力
	float m_gravity;

	// 直接的に加えられた力
	DirectX::SimpleMath::Vector3 m_addForce;

	// 加えられた力の合計
	DirectX::SimpleMath::Vector3 m_totalForce;

	// 初期位置
	DirectX::SimpleMath::Vector3 m_initPosition;

	// バウンディングスフィアへのポインタ
	DirectX::BoundingSphere m_boundingSphere;

	// ID番号
	int m_id;

	// UV座標
	DirectX::SimpleMath::Vector2 m_uv;

	// パラメータ
	nlohmann::json m_parameters;

	DirectX::SimpleMath::Vector3 m_resetPosition;
};

#endif // PHYSICAL_POINT_DEFINED