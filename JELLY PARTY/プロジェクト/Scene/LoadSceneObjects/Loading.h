//--------------------------------------------------------------------------------------
// File: Loading.h
//
// ���[�f�B���O���̃X�v���C�g
//
// Date: 2023.11.03
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef LOADING_DEFINED
#define LOADING_DEFINED

#include "MyLib/Resources.h"

class Loading
{
public:

	// 1�̉摜�̑傫��
	int SPRITE_WIDTH;
	int SPRITE_HEIGHT;
	// �X�v���C�g�V�[�g�̗�
	const int SPRITE_COLUMN = 10;
	// �X�v���C�g�V�[�g�̍s��
	const int SPRITE_ROW = 3;

	const float SCALE = 1.5f;

public:
	// �R���e�L�X�g
	Loading();
	// �f�X�g���N�^
	~Loading();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	// �t���[�����J�E���g
	DirectX::SimpleMath::Vector2 m_frameCount;

	// �X�v���C�g�f�[�^
	Resources::TextureTable m_loadingSprite;

	// �ʒu
	DirectX::SimpleMath::Vector2 m_position;
};

#endif // LOADING_DEFINED