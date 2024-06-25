//--------------------------------------------------------------------------------------
// File: SoundUtility.cpp
//
// ���̃��[�e�B���e�B�N���X
//
// Date: 2023/07/30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "SoundUtility.h"

//------------------------------
// �R���X�g���N�^
//------------------------------
SoundUtility::SoundUtility()
	:
	m_state(State::FADEIN)
	, m_volume(0.0f)
	, m_fadeVolume(0.01f)
{
	
}

//------------------------------
// �X�V
//------------------------------
void SoundUtility::Update(DirectX::SoundEffectInstance* snd)
{
	// ��Ԃɉ����čX�V
	switch (m_state)
	{
	case State::IDLE:
		m_volume = 1.0f;
		break;
	case State::FADEIN:
		FadeIn(snd);
		break;
	case State::FADEOUT:
		FadeOut(snd);
		break;
	}
}

//------------------------------
// �t�F�[�h�C��
//------------------------------
void SoundUtility::FadeIn(DirectX::SoundEffectInstance* snd)
{
	m_volume += m_fadeVolume;

	if (m_volume >= 1.0f)
	{
		// �X�e�[�g���A�C�h�����
		m_state = State::IDLE;
		// volume���Œ�
		m_volume = 1.0f;
		return;
	}
	snd->SetVolume(m_volume);
}

//------------------------------
// �t�F�[�h�A�E�g
//------------------------------
void SoundUtility::FadeOut(DirectX::SoundEffectInstance* snd)
{
	m_volume -= m_fadeVolume;
	// �o�ߎ��Ԃ��ړ��ɂ����鎞�Ԃ𒴂��Ă��邩
	if (m_volume <= 0.0f)
	{
		// �X�e�[�g���A�C�h�����
		m_state = State::IDLE;
		// volume���Œ�
		m_volume = 0.0f;
		return;
	}
	snd->SetVolume(m_volume);
}

