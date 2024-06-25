//--------------------------------------------------------------------------------------
// File: ObjectCollision.h
//
// 衝突判定を行うクラス
//
// Last Update : 2023.11.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef OBJECT_COLLISION_DEFINED
#define OBJECT_COLLISION_DEFINED

#include "Scene/GameMainObjects/Objects/CenterSoftBody.h"
#include "Scene/GameMainObjects/Objects/SoftBodyBar.h"
#include "Scene/GameMainObjects/Objects/SoftBodyCube.h"
#include "Scene/GameMainObjects/Objects/CatBall/CatSphere.h"
#include "Scene/GameMainObjects/UI/Score.h"

class ObjectCollision
{
public:

	// Graphicsクラスのインスタンスを取得する
	static ObjectCollision* const GetInstance();
	// デストラクタ
	~ObjectCollision();

private:

	// コンストラクタ
	ObjectCollision();
	// 代入は許容しない
	void operator=(const ObjectCollision& object) = delete;
	// コピーコンストラクタは許容しない
	ObjectCollision(const ObjectCollision& object) = delete;
private:

	// Graphicsクラスのインスタンスへのポインタ
	static std::unique_ptr<ObjectCollision> m_objectCollision;

public:

	void SetModelCollision(Imase::ObjCollision* pCollision) { m_pModelCollision = pCollision; }
	void SetCatBody(CatSphere* pCatBody) { m_pCatBody = pCatBody; }
	void SetScore(Score* pScore) { m_pScore= pScore; }

	Imase::ObjCollision* GetModelCollision() { return m_pModelCollision; }
public:

	// 球とバーの頂点の当たり判定
	bool CatToBarPoints(
		SoftBodyBox* pBox,
		SoftBodyCylinder* pCenter,
		const float& anglarVelocity,				// 回転速度
		DirectX::SimpleMath::Vector3* hitPosition
	);

	// 球とゼリーキューブの当たり判定
	bool CatToCube(const DirectX::BoundingSphere& cubeCollision);

	// 球と中心の軸との当たり判定
	bool CatToShaft(
		const DirectX::BoundingSphere& shaftCollision, 
		SoftBodyCylinder* pCylinder,
		DirectX::SimpleMath::Vector3* hitPosition
	);

	// 球とモデルの当たり判定
	bool CatToModel();

	// ソフトボディとモデルの当たり判定
	void PointsToModel(SoftBodyBox* pBox, const float& restitution);

	// 中心の軸とキューブの当たり判定
	void ShaftToCube(
		CenterSoftBody* pShaft,
		SoftBodyBox* pCube,
		const DirectX::BoundingSphere& cubeCollision
	);

	// バーとゼリーキューブの当たり判定
	bool BarToCubeBody(
		const DirectX::BoundingSphere& collision, 
		SoftBodyBox* pBarBox, 
		DirectX::SimpleMath::Vector3* direction);

	// ゼリーキューブ同士の当たり判定
	void CubeToOtherCube(
		SoftBodyBox* pThisCube, const DirectX::BoundingSphere& pThisCollision,
		SoftBodyBox* pOtherCube, const DirectX::BoundingSphere& pOtherCollision
	);

	float CalculateDistanceFromCenter(SoftBodyCylinder* pCenter);
	
	//------------------------------------------
	// 最近点を探す
	//------------------------------------------
	template <typename T>
	PhysicalPoint* FindClosestPointFromBall(T* pSoftBody)
	{
		float closestDistance = 999.0f;
		PhysicalPoint* closestPoint = nullptr;

		for (auto& point : *pSoftBody->GetPoints())
		{
			// 球からの距離を計算
			DirectX::SimpleMath::Vector3 distance = point.GetPosition() - 
				DirectX::SimpleMath::Vector3(m_pCatBody->GetPosition().x ,m_pCatBody->GetPosition().y + m_pCatBody->GetRadius(), m_pCatBody->GetPosition().z);
			float length = distance.Length();
			// 距離が一番近い距離より小さいかどうか
			if (length < closestDistance)
			{
				//小さければ一番近い頂点として設定
				closestDistance = length;
				closestPoint = &point;
			}
		}

		return closestPoint;
	}

	//------------------------------------------
	// キューブからの最近点を探す
	//------------------------------------------
	template <typename T>
	PhysicalPoint* FindClosestPointFromSoftBody(
		T* pBody,
		const DirectX::BoundingSphere& collision
	)
	{
		float closestDistance = 999.0f;
		PhysicalPoint* closestPoint = nullptr;

		DirectX::SimpleMath::Vector3 cubeCenter = collision.Center;

		for (auto& point : *pBody->GetPoints())
		{
			// 距離を計算
			DirectX::SimpleMath::Vector3 distance = point.GetPosition() - cubeCenter;
			float length = distance.Length();
			// 距離が一番近い距離より小さいかどうか
			if (length < closestDistance)
			{
				//小さければ一番近い頂点として設定
				closestDistance = length;
				closestPoint = &point;
			}
		}

		return closestPoint;
	}

private:

	Imase::ObjCollision* m_pModelCollision;
	
	CatSphere* m_pCatBody;

	Score* m_pScore;
};

#endif // OBJECT_COLLISION_DEFINED