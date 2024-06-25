//--------------------------------------------------------------------------------------
// File: SoundUtility.cpp
//
// 音のユーティリティクラス
//
// Date: 2023/07/30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "SoundUtility.h"

//------------------------------
// コンストラクタ
//------------------------------
SoundUtility::SoundUtility()
	:
	m_state(State::FADEIN)
	, m_volume(0.0f)
	, m_fadeVolume(0.01f)
{
	
}

//------------------------------
// 更新
//------------------------------
void SoundUtility::Update(DirectX::SoundEffectInstance* snd)
{
	// 状態に応じて更新
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
// フェードイン
//------------------------------
void SoundUtility::FadeIn(DirectX::SoundEffectInstance* snd)
{
	m_volume += m_fadeVolume;

	if (m_volume >= 1.0f)
	{
		// ステートをアイドル状態
		m_state = State::IDLE;
		// volumeを固定
		m_volume = 1.0f;
		return;
	}
	snd->SetVolume(m_volume);
}

//------------------------------
// フェードアウト
//------------------------------
void SoundUtility::FadeOut(DirectX::SoundEffectInstance* snd)
{
	m_volume -= m_fadeVolume;
	// 経過時間が移動にかかる時間を超えているか
	if (m_volume <= 0.0f)
	{
		// ステートをアイドル状態
		m_state = State::IDLE;
		// volumeを固定
		m_volume = 0.0f;
		return;
	}
	snd->SetVolume(m_volume);
}

