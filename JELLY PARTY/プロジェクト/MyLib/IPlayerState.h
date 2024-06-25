//--------------------------------------------------------------------------------------
// File: IPlayerState.h
//
// �v���C���[�̃X�e�[�g�N���X(GOF State�p�^�[��)
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
	// ����������
	virtual void Initialize() = 0;
	// �X�V����
	virtual void Update(const float& elapsedTime) = 0;
	// �`�悷��
	virtual void Render() = 0;
};

#endif		// IPLAYER_STATE_DEFINED