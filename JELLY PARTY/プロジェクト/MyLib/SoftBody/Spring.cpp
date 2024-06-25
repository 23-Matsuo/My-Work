//--------------------------------------------------------------------------------------
// File: Spring.cpp
//
// ���_���q����o�l�N���X
//
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Spring.h"

using namespace DirectX;

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Spring::Spring(PhysicalPoint* p1, PhysicalPoint* p2)
	:
	m_distance(0)
	, m_currentDistance(0)
	, m_direction{}
	, m_resist{}
	, m_point1(p1)
	, m_point2(p2)
	, m_fluctuation{}
{
	// �������v�Z
	m_distance = SimpleMath::Vector3::Distance(p1->GetPosition(),p2->GetPosition());

	//�p�����[�^�擾
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/SoftBodyParameter");
	// �e����
	m_elasticity = static_cast<float>(m_parameters["SoftBodyParameter"]["SpringParameter"]["ELASTICITY"]);
	// ��R��
	m_resistance = static_cast<float>(m_parameters["SoftBodyParameter"]["SpringParameter"]["RESISTANCE"]);

}

//------------------------------------------
// �΂˂̂�����v�Z
//----------------------------- -------------
void Spring::CalculateForce()
{
	// ���_�̃|�W�V�������擾
	const SimpleMath::Vector3 pos1 = m_point1->GetPosition();
	const SimpleMath::Vector3 pos2 = m_point2->GetPosition();

	// ���݂̋������v�Z
	m_currentDistance = SimpleMath::Vector3::Distance(pos1, pos2);
	
	// �x�N�g���̌����v�Z
	m_direction = pos2 - pos1;
	m_direction.Normalize();

	// �΂˂̗͌v�Z 
	// ------------------------------------------------------
	// �΂˂̕�����( F = -kx )
	// �΂˂̎��R������̐L�сi�ψʁj�����߂�
	//x > 0 �̂Ƃ��AF �͕��̌����Ax < 0 �̂Ƃ��AF �͐��̌���
	// ------------------------------------------------------
	
	// �΂˂̗͂����߂�
	SimpleMath::Vector3 force = -m_direction *
		(m_distance - m_currentDistance) * m_elasticity;
	// ��R�̗͂ƌ��� = �΂˂̃x�N�g���̋t�~��R��
	m_resist = -m_fluctuation * m_resistance;

	// ��
	m_force1 = force + m_resist;
	// ���������
	m_force2 = -force - m_resist;
	// �L��
	m_fluctuation = force;

	// �͂̑傫���𐧌�
	SimpleMath::Vector3 min = -SimpleMath::Vector3::One / 5;
	SimpleMath::Vector3 max = SimpleMath::Vector3::One / 5;

	m_force1.Clamp(min, max);
	m_force2.Clamp(min, max);
	// �͂�������
	AddForce();
}

//------------------------------------------
// ���ꂼ��̓_�ɗ͂�������
//------------------------------------------
void Spring::AddForce()
{
	m_point1->AddForce(m_force1);
	m_point2->AddForce(m_force2);
}