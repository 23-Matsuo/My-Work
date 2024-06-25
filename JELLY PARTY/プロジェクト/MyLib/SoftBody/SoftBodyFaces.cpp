//--------------------------------------------------------------------------------------
// File: SoftBodyFaces.cpp
//
// 面との当たり判定を計算するクラス
//
// Date: 2023.8.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "SoftBodyFaces.h"
#include "MyLib/Collision.h"

using namespace DirectX;
//------------------------------------------
// コンストラクタ
//------------------------------------------
SBSquare::SBSquare(PhysicalPoint* a, PhysicalPoint* b, PhysicalPoint* c, PhysicalPoint* d)
	:
	m_pA(a)
	, m_pB(b)
	, m_pC(c)
	, m_pD(d)
{
	// -----------------------------
	//			p3		 p2
	//			|~~~~~~~~|
	//			| 		 |
	//			|________|
	//			p1		 p0
	//------------------------------

	// 法線ベクトルを求める
	DirectX::SimpleMath::Vector3 ba = b->GetPosition() - a->GetPosition();
	DirectX::SimpleMath::Vector3 ca = c->GetPosition() - a->GetPosition();
	DirectX::SimpleMath::Vector3 cb = c->GetPosition() - b->GetPosition();
	DirectX::SimpleMath::Vector3 db = d->GetPosition() - b->GetPosition();
	DirectX::SimpleMath::Vector3 abc = ba.Cross(ca);
	DirectX::SimpleMath::Vector3 bcd = cb.Cross(db);

	m_normal = (abc + bcd) / 2;

	m_normal.Normalize();

	m_points.push_back(a);
	m_points.push_back(b);
	m_points.push_back(c);
	m_points.push_back(d);
}

//------------------------------------------
// 更新
//------------------------------------------
void SBSquare::Update()
{
	SimpleMath::Vector3 p0, p1, p2, p3;
	GetPointsPosition(&p0, &p1, &p2, &p3);

	// 法線は毎フレーム更新する
	DirectX::SimpleMath::Vector3 ba = p1 - p0;
	DirectX::SimpleMath::Vector3 ca = p2 - p0;
	DirectX::SimpleMath::Vector3 cb = p2 - p1;
	DirectX::SimpleMath::Vector3 db = p3 - p1;
	DirectX::SimpleMath::Vector3 abc = ba.Cross(ca);
	DirectX::SimpleMath::Vector3 bcd = cb.Cross(db);

	m_normal = (abc + bcd) / 2;
	m_normal.Normalize();
}

//------------------------------------------
// 球の力の計算
//------------------------------------------
void SBSquare::CalculateSphereForce(CatSphere* pSphere)
{
	CalculateForce(pSphere, 10.0f);
}

//------------------------------------------
// 力を加える
//------------------------------------------
void SBSquare::AddForce(
	DirectX::SimpleMath::Vector3 f0,
	DirectX::SimpleMath::Vector3 f1,
	DirectX::SimpleMath::Vector3 f2,
	DirectX::SimpleMath::Vector3 f3
)
{
	m_pA->AddForce(f0);
	m_pB->AddForce(f1);
	m_pC->AddForce(f2);
	m_pD->AddForce(f3);
}

//------------------------------------------
// 交差判定
//------------------------------------------
bool SBSquare::RayIntersectsFace(
	const DirectX::SimpleMath::Vector3& origin,
	const DirectX::SimpleMath::Vector3& direction,
	float& t
)
{
	float denominator = m_normal.Dot(direction);

	// 線分が平行な場合
	if (fabs(denominator) < FLT_EPSILON)
	{
		return false;
	}

	// 交差点までのパラメータtを計算
	t = -(m_normal.Dot(origin)) / denominator;

	// tが0より大きく1以下なら線分は平面と交差している
	return (t >= 0.0f && t <= 1.0f);
}

//------------------------------------------
// 力の計算
//------------------------------------------
void SBSquare::CalculateForce(CatSphere* pSphere, const float forceRatio)
{
	using namespace DirectX;
	// 加える力
	SimpleMath::Vector3 addForce;
	// 球の位置
	SimpleMath::Vector3 pos = pSphere->GetPosition();
	// 球の力
	SimpleMath::Vector3 force = pSphere->GetTotalForce();
	// 反射ベクトル
	SimpleMath::Vector3 reflection;

	SimpleMath::Vector3 p0, p1, p2, p3;
	GetPointsPosition(&p0, &p1, &p2, &p3);
	// 各頂点と球の距離
	float d0 = SimpleMath::Vector3::DistanceSquared(p0, pos);
	float d1 = SimpleMath::Vector3::DistanceSquared(p1, pos);
	float d2 = SimpleMath::Vector3::DistanceSquared(p2, pos);
	float d3 = SimpleMath::Vector3::DistanceSquared(p3, pos);
	// 力の角度計算
	float incident = force.Dot(m_normal);
	// 反射の力を計算
	reflection = force - (incident * m_normal * 2);
	//SimpleMath::Vector3 result = force - (force * normal) * 2 * normal;
	// 力更新
	pSphere->UpdateForce(reflection);
	// 法線ベクトルの逆方向に球の力を加える
	addForce = -m_normal * force * forceRatio;
	// 距離の比率から加える力の量を計算
	SimpleMath::Vector3 f0 = addForce * (1.0f / d0);
	SimpleMath::Vector3 f1 = addForce * (1.0f / d1);
	SimpleMath::Vector3 f2 = addForce * (1.0f / d2);
	SimpleMath::Vector3 f3 = addForce * (1.0f / d3);
	// 力を加える
	AddForce(f0, f1, f2, f3);
}


//------------------------------------------
// 頂点の位置を取得
//------------------------------------------
void SBSquare::GetPointsPosition(
	DirectX::SimpleMath::Vector3* p1,
	DirectX::SimpleMath::Vector3* p2,
	DirectX::SimpleMath::Vector3* p3,
	DirectX::SimpleMath::Vector3* p4
)
{
	*p1 = m_pA->GetPosition();
	*p2 = m_pB->GetPosition();
	*p3 = m_pC->GetPosition();
	*p4 = m_pC->GetPosition();
}
