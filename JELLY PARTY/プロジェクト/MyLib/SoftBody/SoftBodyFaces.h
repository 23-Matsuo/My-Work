//--------------------------------------------------------------------------------------
// File: SoftBodyFaces.h
//
// 面との当たり判定を計算するクラス
//
// Date: 2023.8.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef SOFTBODY_FACE_DEFINED
#define SOFTBODY_FACE_DEFINED


#include "PhysicalPoint.h"
#include "Scene/GameMainObjects/Objects/CatBall/CatSphere.h"

// 三角平面
class SBSquare
{
public:
	// コンストラクタ
	SBSquare(PhysicalPoint* a, PhysicalPoint* b, PhysicalPoint* c, PhysicalPoint* d);
	// デストラクタ
	~SBSquare() {};

	PhysicalPoint* GetPointA() { return m_pA; }
	PhysicalPoint* GetPointB() { return m_pB; }
	PhysicalPoint* GetPointC() { return m_pC; }
	PhysicalPoint* GetPointD() { return m_pD; }
	DirectX::SimpleMath::Vector3 GetNormal() { return m_normal; }

private:
	// 法線ベクトル
	DirectX::SimpleMath::Vector3 m_normal;
	// 頂点
	PhysicalPoint* m_pA;
	PhysicalPoint* m_pB;
	PhysicalPoint* m_pC;
	PhysicalPoint* m_pD;
public:
	// 更新
	void Update();
		
	// 面の頂点に力を加える
	void CalculateSphereForce(CatSphere* pSphere);
	
	// 力の加算
	void AddForce(
		DirectX::SimpleMath::Vector3 f0, DirectX::SimpleMath::Vector3 f1,
		DirectX::SimpleMath::Vector3 f2, DirectX::SimpleMath::Vector3 f3
	);

	void CalculateForce(CatSphere* pSphere, const float forceRatio);

	bool RayIntersectsFace(
		const DirectX::SimpleMath::Vector3& origin,
		const DirectX::SimpleMath::Vector3& direction,
		float& t
	);
public:

	void GetPointsPosition(
		DirectX::SimpleMath::Vector3* p1, DirectX::SimpleMath::Vector3* p2,
		DirectX::SimpleMath::Vector3* p3, DirectX::SimpleMath::Vector3* p4);


private:
	// 平面の頂点を格納するコンテナ
	std::vector<PhysicalPoint*> m_points;
};

#endif //SOFTBODY_FACE_DEFINED