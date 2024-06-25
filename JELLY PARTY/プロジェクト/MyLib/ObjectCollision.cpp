//--------------------------------------------------------------------------------------
// File: ObjectCollision.h
//
// �Փ˔�����s���N���X
//
// Last Update : 2023.11.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "ObjectCollision.h"

std::unique_ptr<ObjectCollision> ObjectCollision::m_objectCollision = nullptr;

//----------------------------------------
// ObjectCollision�N���X�̃C���X�^���X���擾����
//----------------------------------------
ObjectCollision* const ObjectCollision::GetInstance()
{
	if (m_objectCollision == nullptr)
	{
		// ObjectCollision�N���X�̃C���X�^���X�𐶐�����
		m_objectCollision.reset(new ObjectCollision());
	}
	// ObjectCollision�N���X�̃C���X�^���X��Ԃ�
	return m_objectCollision.get();
}


//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
ObjectCollision::ObjectCollision()
	:
	m_pModelCollision(nullptr)
	, m_pCatBody(nullptr)
	, m_pScore(nullptr)
{
}


//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
ObjectCollision::~ObjectCollision()
{
	m_pModelCollision = nullptr;
	m_pCatBody = nullptr;
}


//------------------------------------------
// ���ƃo�[�̏Փ˔���
//------------------------------------------
bool ObjectCollision::CatToBarPoints(
	SoftBodyBox* pBox,
	SoftBodyCylinder* pCenter,
	const float& anglarVelocity,
	DirectX::SimpleMath::Vector3* hitPosition
)
{	
	// �o�[�����������Ȃ�Ȃ����x�̔{��
	const float addForceRatio = 75.0f;

	SoftBodyBox::Points& points = *pBox->GetPoints();

	for (int i = 0; i < points.size(); i++)
	{
		DirectX::BoundingSphere pointCollision = points[i].GetBoundingSphere();

		// �Փ˔���
		if (pointCollision.Intersects(m_pCatBody->GetCollider()))
		{
			// ���S����̕������v�Z
			DirectX::SimpleMath::Vector3 direction = pointCollision.Center - m_pCatBody->GetPosition();
			direction.Normalize();

			// ���S����̋���
			float catDistanceFromCenter = CalculateDistanceFromCenter(pCenter);
			catDistanceFromCenter /= 5.0f;

			// �v�Z���������ɋ��������Ƃɗ͂�������
			DirectX::SimpleMath::Vector3 newForce = -direction * (catDistanceFromCenter * anglarVelocity) * 10;
			
			// y�̗͂͏������Ă���
			newForce.y /= 10;
			// ���̗͂��X�V
			m_pCatBody->UpdateForce(newForce);

			// �o�[�ɉ�����͂��v�Z
			DirectX::SimpleMath::Vector3 addForce =
				(direction * anglarVelocity) * addForceRatio;

			// �o�[�̒��_�ɗ͂�������
			points[i].AddForce(addForce);

			// �q�b�g�ʒu���L�^
			*hitPosition = m_pCatBody->GetPosition() + m_pCatBody->GetTotalForce();

			return true;
		}
	}
	return false;
}

//------------------------------------------
// ���ƃ[���[�L���[�u�̏Փ˔���
//------------------------------------------
bool ObjectCollision::CatToCube(const DirectX::BoundingSphere& cubeCollision)
{
	DirectX::BoundingSphere collision(cubeCollision.Center, cubeCollision.Radius * 1.5f);

	// �Փ˔���
	if (collision.Intersects(m_pCatBody->GetCollider()))
	{
		// �X�R�A���Z
		m_pScore->CountUp();

		return true;
	}
	return false;
}

//------------------------------------------
/// ���Ɖ�]���[���[�̏Փ˔���
//------------------------------------------
bool ObjectCollision::CatToShaft(
	const DirectX::BoundingSphere& shaftCollision, 
	SoftBodyCylinder* pCylinder, 
	DirectX::SimpleMath::Vector3* hitPosition
)
{
	// �Փ˔���
	if (shaftCollision.Contains(m_pCatBody->GetCollider()))
	{
		// �ŋߐړ_�ɗ͂�������
		PhysicalPoint* closestPoint = FindClosestPointFromBall(pCylinder);
		closestPoint->AddForce(m_pCatBody->GetTotalForce() * 5);

		// �������v�Z
		DirectX::SimpleMath::Vector3 direction;
		direction = m_pCatBody->GetPosition() - shaftCollision.Center;
		direction.Normalize();
		// �͂̍��v
		float force = m_pCatBody->GetTotalForce().Length();
		// �v���C���[�𔽔�������
		DirectX::SimpleMath::Vector3 newForce = (direction * force) * 1.5f;
		newForce.y = 0.0f;
		m_pCatBody->UpdateForce(newForce);

		// �q�b�g�ʒu���L�^
		*hitPosition = m_pCatBody->GetPosition() + m_pCatBody->GetTotalForce();

		return true;
	}
	return false;
}

//------------------------------------------
/// ���ƃ��f���̏Փ˔���
//------------------------------------------
bool ObjectCollision::CatToModel()
{
	// �����L�^����ϐ�
	DirectX::SimpleMath::Vector3 hitPosition;
	DirectX::SimpleMath::Vector3 normal;
	// ���̈ʒu
	DirectX::SimpleMath::Vector3 pos = m_pCatBody->GetPosition();
	// �ʒu���I�t�Z�b�g�����Z
	float positionOffset = 0.15f;
	pos.y -= positionOffset;

	// �������쐬
	DirectX::SimpleMath::Vector3 line = pos;
	const float LINE_LENGTH = 0.5f;
	line.y = pos.y + LINE_LENGTH;

	// �����Ə��̌���������s��
	if (m_pModelCollision->IntersectLineSegment(line, pos, &hitPosition, &normal))
	{
		// �ʒu���I�t�Z�b�g�����Z
		positionOffset = 0.1f;
		hitPosition.y += positionOffset;
		// �ʒu��ݒ�
		m_pCatBody->SetPosition(hitPosition);
		return true;
	}
	else
	{
		return false;
	}
}

//------------------------------------------
// �[���[�ƃ��f���̏Փ˔���
//------------------------------------------
void ObjectCollision::PointsToModel(SoftBodyBox* pBox, const float& friction)
{
	for (auto& point : *pBox->GetPoints())
	{
		DirectX::SimpleMath::Vector3 hitPosition;
		DirectX::SimpleMath::Vector3 normal;

		// �������쐬
		DirectX::SimpleMath::Vector3 line = point.GetPosition();
		line.y -= 0.35f;

		// �����Ə��̌���������s��
		if (m_pModelCollision->IntersectLineSegment(line, point.GetPosition(), &hitPosition, &normal))
		{
			DirectX::SimpleMath::Vector3 force = point.GetVector();
			// ���˃x�N�g�����v�Z
			if (force.y < 0.0f)
			{
				force = -force * friction;
			}
			// �͍X�V
			point.UpdateVector(force);

			hitPosition.y += 0.35f;
			point.SetPosition(hitPosition);
		}
	}
}

//------------------------------------------
// �o�[�ƃ[���[�L���[�u�̏Փ˔���
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
		// ���_�̓����蔻����擾
		DirectX::BoundingSphere pointCollision = points[i].GetBoundingSphere();

		if (cubeCollision.Intersects(pointCollision))
		{
			// ���S����̕������v�Z
			DirectX::SimpleMath::Vector3 barPointCenter = pointCollision.Center;
			DirectX::SimpleMath::Vector3 cubeCenter = cubeCollision.Center;

			DirectX::SimpleMath::Vector3 dir = barPointCenter - cubeCenter;
			dir.Normalize();

			// �������L�^
			*direction = dir;

			return true;
		}
	}
	return false;
}

//------------------------------------------
// ���S�̎��ƃ[���[�L���[�u�̏Փ˔���
//------------------------------------------
void ObjectCollision::ShaftToCube(
	CenterSoftBody* pShaft,
	SoftBodyBox* pCube,
	const DirectX::BoundingSphere& cubeCollision
)
{
	// ���ɐN���������ǂ���
	if (pShaft->GetCollision().Intersects(cubeCollision))
	{
		SoftBodyBox::Points& points = *pCube->GetPoints();
		DirectX::SimpleMath::Vector3 direction;
		DirectX::SimpleMath::Vector3 cubePosition = cubeCollision.Center;
		// ���������v�Z
		direction = cubePosition - pShaft->GetCollision().Center;
		direction.Normalize();

		DirectX::SimpleMath::Vector3 newForce;
		for (auto& point : points)
		{
			// ���ɐN���������_�̗͂��X�V����
			if (pShaft->GetCollision().Intersects(point.GetBoundingSphere()))
			{
				// �͂̌v�Z
				float forceLength = points[0].GetVector().Length();
				newForce = forceLength * direction;

				// �L���[�u�̒��_�̗͍X�V
				point.UpdateVector(newForce / 1.5f);
			}
		}

		// ���S�̉~���ɂ��͂����Z
		PhysicalPoint* closestPoint = FindClosestPointFromSoftBody(pShaft->GetCylinder(), cubeCollision);
		closestPoint->AddForce(-newForce * 2);
	}
}

//------------------------------------------
// �[���[�L���[�u���m�̓����蔻��
//------------------------------------------
void ObjectCollision::CubeToOtherCube(
	SoftBodyBox* pCube, const DirectX::BoundingSphere& pCollision,
	SoftBodyBox* pOtherCube, const DirectX::BoundingSphere& pOtherCollision
)
{
	if (pCollision.Intersects(pOtherCollision))
	{
		// �ŋߓ_��T��
		PhysicalPoint* closestPoint = FindClosestPointFromSoftBody(pCube, pOtherCollision);
		PhysicalPoint* closestOtherPoint = FindClosestPointFromSoftBody(pOtherCube, pCollision);
		// �ŋߓ_�̗͂��擾
		DirectX::SimpleMath::Vector3 force = closestPoint->GetVector();
		DirectX::SimpleMath::Vector3 otherForce = closestOtherPoint->GetVector();
		// ���ꂼ��̗͂����ꂼ��ɐݒ肷��
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
// ���S�̎�����̋������v�Z
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

