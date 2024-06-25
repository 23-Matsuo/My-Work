//--------------------------------------------------------------------------------------
// File: PhysicalPoint.h
//
// ���_�ɗ͂�������N���X
//
// Date: 2023.5.10
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "PhysicalPoint.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
PhysicalPoint::PhysicalPoint(DirectX::SimpleMath::Vector3 pos, const int& ID)
	:
	m_id(ID)
	, m_totalForce{}
	, m_addForce{}
	, m_vector{}
	, m_state(STATE::UNFIXED)
	, m_uv{}
	, m_boundingSphere(pos, 0.1f)
	, m_resetPosition{}
{
	// �p�����[�^�ǂݍ���
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/SoftBodyParameter");
	m_mass = static_cast<float>(m_parameters["SoftBodyParameter"]["PointParameter"]["MASS"]);
	m_gravity = static_cast<float>(m_parameters["SoftBodyParameter"]["PointParameter"]["GRAVITY"]);

	// �d�͉����x��ݒ�
	m_gravityAcceleration = m_gravity * m_mass;

	//�ʒu��ݒ�
	SetPosition(pos);
	m_initPosition = pos;
}

//------------------------------------------
/// �X�V
//------------------------------------------
void PhysicalPoint::Update(const DX::StepTimer& timer)
{
	m_boundingSphere.Center = GetPosition();

	// �����Œ��ԂȂ珈�����Ȃ�
	if (m_state == STATE::FIXED) return;

	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	// ��������͂̍��v���v�Z
	m_totalForce = m_addForce;
	// �ǂƃo�[�͏d�͂��v�Z���Ȃ�
	if (m_state == STATE::WALL|| m_state == STATE::BAR)
	{
		m_gravityAcceleration = 0.0f;
	}
	// �d�͂��v�Z
	m_totalForce.y -= m_gravityAcceleration;

	// �͂��ړ��ʂɉ��Z
	m_vector += m_totalForce / m_mass * elapsedTime;

	// �͂̏��
	m_vector.Clamp(
		-DirectX::SimpleMath::Vector3::One / 1.75f, 
		 DirectX::SimpleMath::Vector3::One / 1.75f
	);

	// �o�[�Ȃ�y���W�͓������Ȃ�
	if (m_state == STATE::BAR)
	{
		m_vector.y = 0.0f;
	}

	// �|�W�V�����ɉ��Z
	SetPosition(GetPosition() + m_vector);

	// ���E���𒸓_�̈ʒu�ɐݒ�
	m_boundingSphere.Center = GetPosition();
	
	// �������͂�������
	m_addForce = DirectX::SimpleMath::Vector3::Zero;
}

//------------------------------------------
// �͂�������
//------------------------------------------
void PhysicalPoint::AddForce(DirectX::SimpleMath::Vector3 force)
{
	m_addForce += force;
}

//------------------------------------------
// �x�N�g���X�V
//------------------------------------------
void PhysicalPoint::UpdateVector(DirectX::SimpleMath::Vector3 force)
{
	m_vector = force;
	m_totalForce.y = 0;
}

//------------------------------------------
// ������
//------------------------------------------
void PhysicalPoint::ParameterReset(const DirectX::SimpleMath::Vector3& position)
{
	SetPosition(m_initPosition + position);
	SetResetPosition(GetPosition());

	m_addForce = {};
	m_totalForce = {};
	m_vector = {};
}

//------------------------------------------
// ���U���g�̏�����
//------------------------------------------
void PhysicalPoint::ResultReset(const DirectX::SimpleMath::Vector3& position)
{
	SetPosition(m_resetPosition + position);

	m_addForce = {};
	m_totalForce = {};
	m_vector = {};
}