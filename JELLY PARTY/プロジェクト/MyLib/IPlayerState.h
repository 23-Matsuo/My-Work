//--------------------------------------------------------------------------------------
// File: IPlayerState.h
//
// プレイヤーのステートクラス(GOF Stateパターン)
//
// Last Update: 2023.01.15
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef IPLAYER_STATE_DEFINED
#define IPLAYER_STATE_DEFINED

class IPlayerState
{
public:
	virtual ~IPlayerState() = default;
	// 初期化する
	virtual void Initialize() = 0;
	// 更新する
	virtual void Update(const float& elapsedTime) = 0;
	// 描画する
	virtual void Render() = 0;
};

#endif		// IPLAYER_STATE_DEFINED