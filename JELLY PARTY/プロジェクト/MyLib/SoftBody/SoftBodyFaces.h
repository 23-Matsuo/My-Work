//--------------------------------------------------------------------------------------
// File: SoftBodyFaces.h
//
// �ʂƂ̓����蔻����v�Z����N���X
//
// Date: 2023.8.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef SOFTBODY_FACE_DEFINED
#define SOFTBODY_FACE_DEFINED


#include "PhysicalPoint.h"
#include "Scene/GameMainObjects/Objects/CatBall/CatSphere.h"

// �O�p����
class SBSquare
{
public:
	// �R���X�g���N�^
	SBSquare(PhysicalPoint* a, PhysicalPoint* b, PhysicalPoint* c, PhysicalPoint* d);
	// �f�X�g���N�^
	~SBSquare() {};

	PhysicalPoint* GetPointA() { return m_pA; }
	PhysicalPoint* GetPointB() { return m_pB; }
	PhysicalPoint* GetPointC() { return m_pC; }
	PhysicalPoint* GetPointD() { return m_pD; }
	DirectX::SimpleMath::Vector3 GetNormal() { return m_normal; }

private:
	// �@���x�N�g��
	DirectX::SimpleMath::Vector3 m_normal;
	// ���_
	PhysicalPoint* m_pA;
	PhysicalPoint* m_pB;
	PhysicalPoint* m_pC;
	PhysicalPoint* m_pD;
public:
	// �X�V
	void Update();
		
	// �ʂ̒��_�ɗ͂�������
	void CalculateSphereForce(CatSphere* pSphere);
	
	// �͂̉��Z
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
	// ���ʂ̒��_���i�[����R���e�i
	std::vector<PhysicalPoint*> m_points;
};

#endif //SOFTBODY_FACE_DEFINED