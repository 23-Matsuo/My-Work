//--------------------------------------------------------------------------------------
// File: Spring.h
//
// ���_���q����o�l�N���X
//
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef SPRING_DEFINED
#define SPRING_DEFINED

#include "PhysicalPoint.h"

class Spring
{
public:
	
	// �R���X�g���N�^
	Spring(PhysicalPoint *p1, PhysicalPoint *p2);
	// �f�X�g���N�^
	~Spring() {};

	// �͂̌v�Z
	void CalculateForce();

	// �͂�������
	virtual void AddForce();

private:
	//// �͂̌��x
	//const float MAX_FORCE = 100.0f;

	// ���_�P
	PhysicalPoint* m_point1;
	// ���_�Q
	PhysicalPoint* m_point2;

	// �_�Ɠ_�̋���
	float m_distance;
	// �_�Ɠ_�̌��݂̋���
	float m_currentDistance;

	// ���_�P�ɉ��Z�����
	DirectX::SimpleMath::Vector3 m_force1;
	// ���_�Q�ɉ��Z�����
	DirectX::SimpleMath::Vector3 m_force2;
	// �͂̑���
	DirectX::SimpleMath::Vector3 m_fluctuation;

	// ����
	DirectX::SimpleMath::Vector3 m_direction;

	// ��R��
	DirectX::SimpleMath::Vector3 m_resist;

	// ��R��
	float m_resistance;

	// �e����
	float m_elasticity;

	// �p�����[�^
	nlohmann::json m_parameters;
};

#endif // SPRING_DEFINED