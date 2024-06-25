//--------------------------------------------------------------------------------------
// File: SoftBodyCorn.h
//
// ���ʃI�u�W�F�N�g�p���p�N���X
//
// Usage: SoftBodyCorn�Ɍp������
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#include "MyLib/PhysicalObject.h"
#include "SoftBodyFaces.h"
#include "Spring.h"
#include "ImaseLib/ObjCollision.h"

class SoftBodyCorn : public PhysicalObject
{
public:
	enum class STATE
	{
		FIXED,
		UNFIXED
	};

public:

	// �R���X�g���N�^
	SoftBodyCorn();
	// �f�X�g���N�^
	~SoftBodyCorn() {};
	// ������
	void Intialize(DirectX::SimpleMath::Vector3 startPos);
	// �X�V
	void Update(const DX::StepTimer& timer) override;
	// �`��
	void Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* batch);
	// �`�����
	void CreateFigure();
	// �_�Ɠ_�̊Ԃ̂΂˂����
	void CreateArms();
	// �_�Ɠ_�̊Ԃ̖ʂ����
	void CreateFaces();
	// �_�Ƃ̏Փ˔�������
	void CollisionDitection(PhysicalPoint& point);
	// �_�ƃ\�t�g�{�f�B�̓��O����
	bool IsPointInsideBox();
	// �ŋߖʂ̈ʒu������
	void ClosestFace();
public:
	// �_��X�������̐�
	int POINT_X_NUM = 2;
	// �_��Z�������̐�
	int POINT_Z_NUM = 2;

public:
	std::vector<PhysicalPoint> GetPoints() { return m_points; }
	std::vector<Spring> GetSprings() { return m_springs; }
	std::vector<SBSquare> GetFaces() { return m_faces; }

public:
	void SetObjCollision(Imase::ObjCollision* pObj) { m_pObjCollision = pObj; }
	// ����ݒ�
	void SetSphere(CatSphere*sphere) { m_pSphere = sphere; }
	// ��]�p��ݒ�
	void SetRotation(DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }
	// ���_�̐���ݒ�
	void SetPointNumber(const int x, const int z) 
	{
		POINT_X_NUM = x; POINT_Z_NUM = z;
	}
	// ���_�̈ʒu��ݒ�
	void SetPointsPosition(DirectX::SimpleMath::Vector3 position) 
	{
		for (int i = 0; i < m_points.size(); i++)
		{
			m_points[i].SetPosition(m_points[i].GetPosition() + position);
		}
	}
private:
	// ���_�̃R���e�i
	std::vector<PhysicalPoint> m_points;
	// �΂˂̃R���e�i
	std::vector<Spring> m_springs;
	// �ʂ̃R���e�i
	std::vector<SBSquare> m_faces;

	// �|�W�V����
	DirectX::SimpleMath::Vector3 m_position;
	// ��]�p
	DirectX::SimpleMath::Quaternion m_rotation;

	Imase::ObjCollision* m_pObjCollision;
	CatSphere* m_pSphere;
	
	std::unique_ptr<DirectX::BoundingBox> m_sbBox;

	std::vector<DirectX::SimpleMath::Vector3> m_closestPoints;
};