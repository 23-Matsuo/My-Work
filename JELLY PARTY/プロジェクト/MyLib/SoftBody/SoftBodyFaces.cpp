//--------------------------------------------------------------------------------------
// File: SoftBodyFaces.cpp
//
// �ʂƂ̓����蔻����v�Z����N���X
//
// Date: 2023.8.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "SoftBodyFaces.h"
#include "MyLib/Collision.h"

using namespace DirectX;
//------------------------------------------
// �R���X�g���N�^
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

	// �@���x�N�g�������߂�
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
// �X�V
//------------------------------------------
void SBSquare::Update()
{
	SimpleMath::Vector3 p0, p1, p2, p3;
	GetPointsPosition(&p0, &p1, &p2, &p3);

	// �@���͖��t���[���X�V����
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
// ���̗͂̌v�Z
//------------------------------------------
void SBSquare::CalculateSphereForce(CatSphere* pSphere)
{
	CalculateForce(pSphere, 10.0f);
}

//------------------------------------------
// �͂�������
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
// ��������
//------------------------------------------
bool SBSquare::RayIntersectsFace(
	const DirectX::SimpleMath::Vector3& origin,
	const DirectX::SimpleMath::Vector3& direction,
	float& t
)
{
	float denominator = m_normal.Dot(direction);

	// ���������s�ȏꍇ
	if (fabs(denominator) < FLT_EPSILON)
	{
		return false;
	}

	// �����_�܂ł̃p�����[�^t���v�Z
	t = -(m_normal.Dot(origin)) / denominator;

	// t��0���傫��1�ȉ��Ȃ�����͕��ʂƌ������Ă���
	return (t >= 0.0f && t <= 1.0f);
}

//------------------------------------------
// �͂̌v�Z
//------------------------------------------
void SBSquare::CalculateForce(CatSphere* pSphere, const float forceRatio)
{
	using namespace DirectX;
	// �������
	SimpleMath::Vector3 addForce;
	// ���̈ʒu
	SimpleMath::Vector3 pos = pSphere->GetPosition();
	// ���̗�
	SimpleMath::Vector3 force = pSphere->GetTotalForce();
	// ���˃x�N�g��
	SimpleMath::Vector3 reflection;

	SimpleMath::Vector3 p0, p1, p2, p3;
	GetPointsPosition(&p0, &p1, &p2, &p3);
	// �e���_�Ƌ��̋���
	float d0 = SimpleMath::Vector3::DistanceSquared(p0, pos);
	float d1 = SimpleMath::Vector3::DistanceSquared(p1, pos);
	float d2 = SimpleMath::Vector3::DistanceSquared(p2, pos);
	float d3 = SimpleMath::Vector3::DistanceSquared(p3, pos);
	// �͂̊p�x�v�Z
	float incident = force.Dot(m_normal);
	// ���˂̗͂��v�Z
	reflection = force - (incident * m_normal * 2);
	//SimpleMath::Vector3 result = force - (force * normal) * 2 * normal;
	// �͍X�V
	pSphere->UpdateForce(reflection);
	// �@���x�N�g���̋t�����ɋ��̗͂�������
	addForce = -m_normal * force * forceRatio;
	// �����̔䗦���������̗͂ʂ��v�Z
	SimpleMath::Vector3 f0 = addForce * (1.0f / d0);
	SimpleMath::Vector3 f1 = addForce * (1.0f / d1);
	SimpleMath::Vector3 f2 = addForce * (1.0f / d2);
	SimpleMath::Vector3 f3 = addForce * (1.0f / d3);
	// �͂�������
	AddForce(f0, f1, f2, f3);
}


//------------------------------------------
// ���_�̈ʒu���擾
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
