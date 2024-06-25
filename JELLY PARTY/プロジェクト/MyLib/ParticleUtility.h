//--------------------------------------------------------------------------------------
// File: ParticleUtil.h
//
// �p�[�e�B�N�����[�e�B���e�B�N���X
// position�⑬�x�Ȃǃp�[�e�B�N���P�ɕK�v�ȗv�f���i�[
//
// Last Update: 2023.10.30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef PARTICLE_UTILITY_DEFINED
#define PARTICLE_UTILITY_DEFINED

#include "StepTimer.h"
#include <vector>
class ParticleUtility
{
public:

	//�R���X�g���N�^
	ParticleUtility(
		float life,									// ��������
		DirectX::SimpleMath::Vector3 pos,			// ���W
		DirectX::SimpleMath::Vector3 velocity,		// ���x
		DirectX::SimpleMath::Vector3 accele,		// �����x
		DirectX::SimpleMath::Vector3 startScale,	// �����T�C�Y
		DirectX::SimpleMath::Vector3 endScale,		// �ŏI�T�C�Y
		DirectX::SimpleMath::Color startColor,		// �����F
		DirectX::SimpleMath::Color endColor			// �ŏI�F
	);

	//�f�X�g���N�^
	~ParticleUtility();

	// �X�V
	bool Update(float elapsedTime);

public:	// �Q�b�^�[

	const DirectX::SimpleMath::Vector3 GetPosition()	{ return m_position; }
	
	const DirectX::SimpleMath::Vector3 GetVelocity()	{ return m_velocity; }

	const DirectX::SimpleMath::Vector3 GetAccele()		{ return m_accele; }
	
	const DirectX::SimpleMath::Vector3 GetNowScale()	{ return m_nowScale; }
	const DirectX::SimpleMath::Vector3 GetStartScale()	{ return m_startScale; }
	const DirectX::SimpleMath::Vector3 GetEndScale()	{ return m_endScale; }

	const float GetLife()								{ return m_life; }
	const float GetStartLife()							{ return m_startLife; }
	const DirectX::SimpleMath::Color GetNowColor()		{ return m_nowColor; }
	const DirectX::SimpleMath::Color GetStartColor()	{ return m_startColor; }
	const DirectX::SimpleMath::Color GetEndColor()		{ return m_endColor; }

private:

	//���W
	DirectX::SimpleMath::Vector3 m_position;

	//���x
	DirectX::SimpleMath::Vector3 m_velocity;
	//�����x
	DirectX::SimpleMath::Vector3 m_accele;

	//�X�P�[��
	DirectX::SimpleMath::Vector3 m_nowScale;
	DirectX::SimpleMath::Vector3 m_startScale;
	DirectX::SimpleMath::Vector3 m_endScale;

	//��������
	float m_life;
	float m_startLife;

	//�J���[
	DirectX::SimpleMath::Color m_nowColor;
	DirectX::SimpleMath::Color m_startColor;
	DirectX::SimpleMath::Color m_endColor;
};

#endif // PARTICLE_UTILITY_DEFINED