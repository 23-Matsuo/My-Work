//--------------------------------------------------------------------------------------
// File: SceneManager.h
//
// シーンを管理するクラス
//
// Date: 2023.10.08
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef SOUND_UTILITY_DEFINED
#define SOUND_UTILITY_DEFINED

#include "Audio.h"

// 効果音を鳴らす
inline void PlaySound(DirectX::SoundEffectInstance* snd, bool loop)
{
	snd->Stop();
	snd->Play(loop);
}

class SoundUtility
{
public:
	// トランジション状態
	enum class State
	{
		IDLE,
		FADEIN,
		FADEOUT
	};

public:

	// コンストラクタ
	SoundUtility();
	// デストラクタ
	~SoundUtility() {};

	// 更新
	void Update(DirectX::SoundEffectInstance* snd);

	// 音量のフェードイン
	void FadeIn(DirectX::SoundEffectInstance* snd);
	// 音量のフェードアウト
	void FadeOut(DirectX::SoundEffectInstance* snd);

	// フェード量を設定
	void SetFadeVolume(float volume)	{ m_fadeVolume = volume; }
	// 状態を設定
	void SetState(State state)			{ m_state = state; }

private:
	// 状態
	State m_state;
	// 音量
	float m_volume;
	// フェード量
	float m_fadeVolume;
};

#endif // SOUND_UTILITY_DEFINED