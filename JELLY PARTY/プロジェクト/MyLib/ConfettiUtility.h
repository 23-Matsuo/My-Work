//--------------------------------------------------------------------------------------
// File: ParticleUtil.h
//
// �p�[�e�B�N�����[�e�B���e�B�N���X
//
//-------------------------------------------------------------------------------------

#pragma once

#ifndef CONFETTI_UTILITY_DEFINED
#define CONFETTI_UTILITY_DEFINED

#include "StepTimer.h"
#include <vector>
class ConfettiUtility
{
public:

	//�R���X�g���N�^
	ConfettiUtility(
		float life,									// ��������
		DirectX::SimpleMath::Vector3 pos,			// ���W
		DirectX::SimpleMath::Vector3 velocity,		// ���x
		DirectX::SimpleMath::Vector3 accele,		// �����x
		DirectX::SimpleMath::Vector3 rotate,		// �e���̉�]�p
		DirectX::SimpleMath::Vector3 scale,			// �X�P�[��
		DirectX::SimpleMath::Color color			// �F
	);


	//�f�X�g���N�^
	~ConfettiUtility();

	// �X�V
	bool Update(float elapsedTime);

public:	// �Q�b�^�[

	const DirectX::SimpleMath::Vector3 GetPosition()	{ return m_position; }
	
	const DirectX::SimpleMath::Vector3 GetVelocity()	{ return m_velocity; }

	const DirectX::SimpleMath::Vector3 GetAccele()		{ return m_accele; }

	const DirectX::SimpleMath::Vector3 GetRotation()	{ return m_rotate; }
	
	const DirectX::SimpleMath::Vector3 GetScale()		{ return m_scale; }

	const float GetLife()								{ return m_life; }
	const float GetStartLife()							{ return m_startLife; }
	const DirectX::SimpleMath::Color GetColor()			{ return m_color; }

private:

	// ���W
	DirectX::SimpleMath::Vector3 m_position;

	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;
	// �����x
	DirectX::SimpleMath::Vector3 m_accele;

	// �e���̉�]�p
	DirectX::SimpleMath::Vector3 m_rotate;

	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;

	// ��������
	float m_life;
	float m_startLife;

	// �J���[
	DirectX::SimpleMath::Color m_color;
};

#endif // CONFETTI_UTILITY_DEFINED