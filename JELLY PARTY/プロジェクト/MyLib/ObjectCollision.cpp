//--------------------------------------------------------------------------------------
// File: ObjectCollision.h
//
// 衝突判定を行うクラス
//
// Last Update : 2023.11.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "ObjectCollision.h"

std::unique_ptr<ObjectCollision> ObjectCollision::m_objectCollision = nullptr;

//----------------------------------------
// ObjectCollisionクラスのインスタンスを取得する
//----------------------------------------
ObjectCollision* const ObjectCollision::GetInstance()
{
	if (m_objectCollision == nullptr)
	{
		// ObjectCollisionクラスのインスタンスを生成する
		m_objectCollision.reset(new ObjectCollision());
	}
	// ObjectCollisionクラスのインスタンスを返す
	return m_objectCollision.get();
}


//------------------------------------------
// コンストラクタ
//------------------------------------------
ObjectCollision::ObjectCollision()
	:
	m_pModelCollision(nullptr)
	, m_pCatBody(nullptr)
	, m_pScore(nullptr)
{
}


//------------------------------------------
// デストラクタ
//------------------------------------------
ObjectCollision::~ObjectCollision()
{
	m_pModelCollision = nullptr;
	m_pCatBody = nullptr;
}


//------------------------------------------
// 球とバーの衝突判定
//------------------------------------------
bool ObjectCollision::CatToBarPoints(
	SoftBodyBox* pBox,
	SoftBodyCylinder* pCenter,
	const float& anglarVelocity,
	DirectX::SimpleMath::Vector3* hitPosition
)
{	
	// バーがおかしくならない程度の倍率
	const float addForceRatio = 75.0f;

	SoftBodyBox::Points& points = *pBox->GetPoints();

	for (int i = 0; i < points.size(); i++)
	{
		DirectX::BoundingSphere pointCollision = points[i].GetBoundingSphere();

		// 衝突判定
		if (pointCollision.Intersects(m_pCatBody->GetCollider()))
		{
			// 中心からの方向を計算
			DirectX::SimpleMath::Vector3 direction = pointCollision.Center - m_pCatBody->GetPosition();
			direction.Normalize();

			// 中心からの距離
			float catDistanceFromCenter = CalculateDistanceFromCenter(pCenter);
			catDistanceFromCenter /= 5.0f;

			// 計算した方向に距離をもとに力を加える
			DirectX::SimpleMath::Vector3 newForce = -direction * (catDistanceFromCenter * anglarVelocity) * 10;
			
			// yの力は小さくていい
			newForce.y /= 10;
			// 球の力を更新
			m_pCatBody->UpdateForce(newForce);

			// バーに加える力を計算
			DirectX::SimpleMath::Vector3 addForce =
				(direction * anglarVelocity) * addForceRatio;

			// バーの頂点に力を加える
			points[i].AddForce(addForce);

			// ヒット位置を記録
			*hitPosition = m_pCatBody->GetPosition() + m_pCatBody->GetTotalForce();

			return true;
		}
	}
	return false;
}

//------------------------------------------
// 球とゼリーキューブの衝突判定
//------------------------------------------
bool ObjectCollision::CatToCube(const DirectX::BoundingSphere& cubeCollision)
{
	DirectX::BoundingSphere collision(cubeCollision.Center, cubeCollision.Radius * 1.5f);

	// 衝突判定
	if (collision.Intersects(m_pCatBody->GetCollider()))
	{
		// スコア加算
		m_pScore->CountUp();

		return true;
	}
	return false;
}

//------------------------------------------
/// 球と回転軸ゼリーの衝突判定
//------------------------------------------
bool ObjectCollision::CatToShaft(
	const DirectX::BoundingSphere& shaftCollision, 
	SoftBodyCylinder* pCylinder, 
	DirectX::SimpleMath::Vector3* hitPosition
)
{
	// 衝突判定
	if (shaftCollision.Contains(m_pCatBody->GetCollider()))
	{
		// 最近接点に力を加える
		PhysicalPoint* closestPoint = FindClosestPointFromBall(pCylinder);
		closestPoint->AddForce(m_pCatBody->GetTotalForce() * 5);

		// 方向を計算
		DirectX::SimpleMath::Vector3 direction;
		direction = m_pCatBody->GetPosition() - shaftCollision.Center;
		direction.Normalize();
		// 力の合計
		float force = m_pCatBody->GetTotalForce().Length();
		// プレイヤーを反発させる
		DirectX::SimpleMath::Vector3 newForce = (direction * force) * 1.5f;
		newForce.y = 0.0f;
		m_pCatBody->UpdateForce(newForce);

		// ヒット位置を記録
		*hitPosition = m_pCatBody->GetPosition() + m_pCatBody->GetTotalForce();

		return true;
	}
	return false;
}

//------------------------------------------
/// 球とモデルの衝突判定
//------------------------------------------
bool ObjectCollision::CatToModel()
{
	// 情報を記録する変数
	DirectX::SimpleMath::Vector3 hitPosition;
	DirectX::SimpleMath::Vector3 normal;
	// 球の位置
	DirectX::SimpleMath::Vector3 pos = m_pCatBody->GetPosition();
	// 位置をオフセット分加算
	float positionOffset = 0.15f;
	pos.y -= positionOffset;

	// 線分を作成
	DirectX::SimpleMath::Vector3 line = pos;
	const float LINE_LENGTH = 0.5f;
	line.y = pos.y + LINE_LENGTH;

	// 線分と床の交差判定を行う
	if (m_pModelCollision->IntersectLineSegment(line, pos, &hitPosition, &normal))
	{
		// 位置をオフセット分加算
		positionOffset = 0.1f;
		hitPosition.y += positionOffset;
		// 位置を設定
		m_pCatBody->SetPosition(hitPosition);
		return true;
	}
	else
	{
		return false;
	}
}

//------------------------------------------
// ゼリーとモデルの衝突判定
//------------------------------------------
void ObjectCollision::PointsToModel(SoftBodyBox* pBox, const float& friction)
{
	for (auto& point : *pBox->GetPoints())
	{
		DirectX::SimpleMath::Vector3 hitPosition;
		DirectX::SimpleMath::Vector3 normal;

		// 線分を作成
		DirectX::SimpleMath::Vector3 line = point.GetPosition();
		line.y -= 0.35f;

		// 線分と床の交差判定を行う
		if (m_pModelCollision->IntersectLineSegment(line, point.GetPosition(), &hitPosition, &normal))
		{
			DirectX::SimpleMath::Vector3 force = point.GetVector();
			// 反射ベクトルを計算
			if (force.y < 0.0f)
			{
				force = -force * friction;
			}
			// 力更新
			point.UpdateVector(force);

			hitPosition.y += 0.35f;
			point.SetPosition(hitPosition);
		}
	}
}

//------------------------------------------
// バーとゼリーキューブの衝突判定
//------------------------------------------
bool ObjectCollision::BarToCubeBody(
	const DirectX::BoundingSphere& cubeCollision,
	SoftBodyBox* pBarBox, 
	DirectX::SimpleMath::Vector3* direction
)
{
	SoftBodyBox::Points& points = *pBarBox->GetPoints();

	for (int i = 0; i < points.size(); i++)
	{
		// 頂点の当たり判定を取得
		DirectX::BoundingSphere pointCollision = points[i].GetBoundingSphere();

		if (cubeCollision.Intersects(pointCollision))
		{
			// 中心からの方向を計算
			DirectX::SimpleMath::Vector3 barPointCenter = pointCollision.Center;
			DirectX::SimpleMath::Vector3 cubeCenter = cubeCollision.Center;

			DirectX::SimpleMath::Vector3 dir = barPointCenter - cubeCenter;
			dir.Normalize();

			// 向きを記録
			*direction = dir;

			return true;
		}
	}
	return false;
}

//------------------------------------------
// 中心の軸とゼリーキューブの衝突判定
//------------------------------------------
void ObjectCollision::ShaftToCube(
	CenterSoftBody* pShaft,
	SoftBodyBox* pCube,
	const DirectX::BoundingSphere& cubeCollision
)
{
	// 軸に侵入したかどうか
	if (pShaft->GetCollision().Intersects(cubeCollision))
	{
		SoftBodyBox::Points& points = *pCube->GetPoints();
		DirectX::SimpleMath::Vector3 direction;
		DirectX::SimpleMath::Vector3 cubePosition = cubeCollision.Center;
		// 反発方向計算
		direction = cubePosition - pShaft->GetCollision().Center;
		direction.Normalize();

		DirectX::SimpleMath::Vector3 newForce;
		for (auto& point : points)
		{
			// 軸に侵入した頂点の力を更新する
			if (pShaft->GetCollision().Intersects(point.GetBoundingSphere()))
			{
				// 力の計算
				float forceLength = points[0].GetVector().Length();
				newForce = forceLength * direction;

				// キューブの頂点の力更新
				point.UpdateVector(newForce / 1.5f);
			}
		}

		// 中心の円柱にも力を加算
		PhysicalPoint* closestPoint = FindClosestPointFromSoftBody(pShaft->GetCylinder(), cubeCollision);
		closestPoint->AddForce(-newForce * 2);
	}
}

//------------------------------------------
// ゼリーキューブ同士の当たり判定
//------------------------------------------
void ObjectCollision::CubeToOtherCube(
	SoftBodyBox* pCube, const DirectX::BoundingSphere& pCollision,
	SoftBodyBox* pOtherCube, const DirectX::BoundingSphere& pOtherCollision
)
{
	if (pCollision.Intersects(pOtherCollision))
	{
		// 最近点を探す
		PhysicalPoint* closestPoint = FindClosestPointFromSoftBody(pCube, pOtherCollision);
		PhysicalPoint* closestOtherPoint = FindClosestPointFromSoftBody(pOtherCube, pCollision);
		// 最近点の力を取得
		DirectX::SimpleMath::Vector3 force = closestPoint->GetVector();
		DirectX::SimpleMath::Vector3 otherForce = closestOtherPoint->GetVector();
		// それぞれの力をそれぞれに設定する
		for (auto& point : *pCube->GetPoints())
		{
			point.UpdateVector(otherForce);
		}

		for (auto& point : *pOtherCube->GetPoints())
		{
			point.UpdateVector(force);
		}
	}
}

//------------------------------------------
// 中心の軸からの距離を計算
//------------------------------------------
float ObjectCollision::CalculateDistanceFromCenter(SoftBodyCylinder* pCenter)
{
	DirectX::SimpleMath::Vector3 center = pCenter->GetPosition();
	DirectX::SimpleMath::Vector3 catPosition = m_pCatBody->GetPosition();

	center.y = 0.0f;
	catPosition.y = 0.0f;

	float distance = DirectX::SimpleMath::Vector3::Distance(center, catPosition);
	
	return distance;
}

