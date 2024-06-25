//--------------------------------------------------------------------------------------
// File: ObjectCollision.h
//
// �Փ˔�����s���N���X
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

	// Graphics�N���X�̃C���X�^���X���擾����
	static ObjectCollision* const GetInstance();
	// �f�X�g���N�^
	~ObjectCollision();

private:

	// �R���X�g���N�^
	ObjectCollision();
	// ����͋��e���Ȃ�
	void operator=(const ObjectCollision& object) = delete;
	// �R�s�[�R���X�g���N�^�͋��e���Ȃ�
	ObjectCollision(const ObjectCollision& object) = delete;
private:

	// Graphics�N���X�̃C���X�^���X�ւ̃|�C���^
	static std::unique_ptr<ObjectCollision> m_objectCollision;

public:

	void SetModelCollision(Imase::ObjCollision* pCollision) { m_pModelCollision = pCollision; }
	void SetCatBody(CatSphere* pCatBody) { m_pCatBody = pCatBody; }
	void SetScore(Score* pScore) { m_pScore= pScore; }

	Imase::ObjCollision* GetModelCollision() { return m_pModelCollision; }
public:

	// ���ƃo�[�̒��_�̓����蔻��
	bool CatToBarPoints(
		SoftBodyBox* pBox,
		SoftBodyCylinder* pCenter,
		const float& anglarVelocity,				// ��]���x
		DirectX::SimpleMath::Vector3* hitPosition
	);

	// ���ƃ[���[�L���[�u�̓����蔻��
	bool CatToCube(const DirectX::BoundingSphere& cubeCollision);

	// ���ƒ��S�̎��Ƃ̓����蔻��
	bool CatToShaft(
		const DirectX::BoundingSphere& shaftCollision, 
		SoftBodyCylinder* pCylinder,
		DirectX::SimpleMath::Vector3* hitPosition
	);

	// ���ƃ��f���̓����蔻��
	bool CatToModel();

	// �\�t�g�{�f�B�ƃ��f���̓����蔻��
	void PointsToModel(SoftBodyBox* pBox, const float& restitution);

	// ���S�̎��ƃL���[�u�̓����蔻��
	void ShaftToCube(
		CenterSoftBody* pShaft,
		SoftBodyBox* pCube,
		const DirectX::BoundingSphere& cubeCollision
	);

	// �o�[�ƃ[���[�L���[�u�̓����蔻��
	bool BarToCubeBody(
		const DirectX::BoundingSphere& collision, 
		SoftBodyBox* pBarBox, 
		DirectX::SimpleMath::Vector3* direction);

	// �[���[�L���[�u���m�̓����蔻��
	void CubeToOtherCube(
		SoftBodyBox* pThisCube, const DirectX::BoundingSphere& pThisCollision,
		SoftBodyBox* pOtherCube, const DirectX::BoundingSphere& pOtherCollision
	);

	float CalculateDistanceFromCenter(SoftBodyCylinder* pCenter);
	
	//------------------------------------------
	// �ŋߓ_��T��
	//------------------------------------------
	template <typename T>
	PhysicalPoint* FindClosestPointFromBall(T* pSoftBody)
	{
		float closestDistance = 999.0f;
		PhysicalPoint* closestPoint = nullptr;

		for (auto& point : *pSoftBody->GetPoints())
		{
			// ������̋������v�Z
			DirectX::SimpleMath::Vector3 distance = point.GetPosition() - 
				DirectX::SimpleMath::Vector3(m_pCatBody->GetPosition().x ,m_pCatBody->GetPosition().y + m_pCatBody->GetRadius(), m_pCatBody->GetPosition().z);
			float length = distance.Length();
			// ��������ԋ߂�������菬�������ǂ���
			if (length < closestDistance)
			{
				//��������Έ�ԋ߂����_�Ƃ��Đݒ�
				closestDistance = length;
				closestPoint = &point;
			}
		}

		return closestPoint;
	}

	//------------------------------------------
	// �L���[�u����̍ŋߓ_��T��
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
			// �������v�Z
			DirectX::SimpleMath::Vector3 distance = point.GetPosition() - cubeCenter;
			float length = distance.Length();
			// ��������ԋ߂�������菬�������ǂ���
			if (length < closestDistance)
			{
				//��������Έ�ԋ߂����_�Ƃ��Đݒ�
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