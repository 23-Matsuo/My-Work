//--------------------------------------------------------------------------------------
// File: ConfettiUtility.cpp
//
// �����჆�[�e�B���e�B�N���X
// 
//
// Last Update: 2024.01.04
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "ConfettiUtility.h"

#include "StepTimer.h"
#include <vector>

using namespace DirectX;

//-----------------------------------
// �R���X�g���N�^
//-----------------------------------
ConfettiUtility::ConfettiUtility(
	float life,
	DirectX::SimpleMath::Vector3 pos,
	DirectX::SimpleMath::Vector3 velocity,
	DirectX::SimpleMath::Vector3 accele,
	DirectX::SimpleMath::Vector3 rotate,
	DirectX::SimpleMath::Vector3 scale,
	DirectX::SimpleMath::Color color
)
{

	m_startLife = m_life = life;

	m_position = pos;

	m_velocity = velocity;
	m_accele = accele;

	m_rotate = rotate;

	m_scale = scale;


	m_color = color;
}

//-----------------------------------
// �f�X�g���N�^
//-----------------------------------
ConfettiUtility::~ConfettiUtility()
{
}

//-----------------------------------
/// �X�V
//-----------------------------------
bool ConfettiUtility::Update(float elapsedTime)
{
	// ���x�̌v�Z
	m_velocity += m_accele * elapsedTime;
	// ���W�̌v�Z
	m_position -= m_velocity * elapsedTime;

	m_life -= elapsedTime;
	// ��]�̌v�Z
	m_rotate.x += 0.05f;
	m_rotate.y += 0.05f;
	m_rotate.z += 0.05f;

	// ���C�t���O�Ŏ��g�������Ă��炤
	if (m_life < 0.0f)
	{
		return false;
	}

	return true;
}
