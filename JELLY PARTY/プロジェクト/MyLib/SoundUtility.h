//--------------------------------------------------------------------------------------
// File: SceneManager.h
//
// �V�[�����Ǘ�����N���X
//
// Date: 2023.10.08
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef SOUND_UTILITY_DEFINED
#define SOUND_UTILITY_DEFINED

#include "Audio.h"

// ���ʉ���炷
inline void PlaySound(DirectX::SoundEffectInstance* snd, bool loop)
{
	snd->Stop();
	snd->Play(loop);
}

class SoundUtility
{
public:
	// �g�����W�V�������
	enum class State
	{
		IDLE,
		FADEIN,
		FADEOUT
	};

public:

	// �R���X�g���N�^
	SoundUtility();
	// �f�X�g���N�^
	~SoundUtility() {};

	// �X�V
	void Update(DirectX::SoundEffectInstance* snd);

	// ���ʂ̃t�F�[�h�C��
	void FadeIn(DirectX::SoundEffectInstance* snd);
	// ���ʂ̃t�F�[�h�A�E�g
	void FadeOut(DirectX::SoundEffectInstance* snd);

	// �t�F�[�h�ʂ�ݒ�
	void SetFadeVolume(float volume)	{ m_fadeVolume = volume; }
	// ��Ԃ�ݒ�
	void SetState(State state)			{ m_state = state; }

private:
	// ���
	State m_state;
	// ����
	float m_volume;
	// �t�F�[�h��
	float m_fadeVolume;
};

#endif // SOUND_UTILITY_DEFINED