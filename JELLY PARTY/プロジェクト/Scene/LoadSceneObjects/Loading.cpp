//--------------------------------------------------------------------------------------
// File: Loading.cpp
//
// ���[�f�B���O���̃X�v���C�g
//
// Date: 2023.11.03
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Loading.h"
#include "MyLib/Graphics.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Loading::Loading()
	:
	m_position(920.0f , 500.0f)
{
	m_loadingSprite = Resources::GetInstance()->GetTexture(L"LoadingSprite");
	SPRITE_WIDTH = m_loadingSprite.textureWidth / SPRITE_COLUMN;
	SPRITE_HEIGHT = m_loadingSprite.textureHeight / SPRITE_ROW;

}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
Loading::~Loading()
{
}

//------------------------------------------
// �X�V
//------------------------------------------
void Loading::Update()
{
	// �`�摬�x���J�E���g
	m_frameCount.x += 1.0f / 3.0f;
	// Sprite�V�[�g�̉E�[�ɍs������
	if (m_frameCount.x >= SPRITE_COLUMN)
	{
		// x��������
		m_frameCount.x = 1.0f;
		// y���J�E���g
		m_frameCount.y += 1.0f;
	}

	// SpriteSheet�̈�ԉ��ɍs������
	if (m_frameCount.y >= SPRITE_ROW)
	{
		// ������
		m_frameCount.x = 1.0f;
		m_frameCount.y = 0.0f;
	}

}

//------------------------------------------
// �`��
//------------------------------------------
void Loading::Render()
{
	auto state = Graphics::GetInstance()->GetCommonStates();
	auto spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// �؂���J�n�ʒu
	int startCut = static_cast<int>(m_frameCount.x - 1.0f);
	// �؂���I���ʒu
	int endCut = static_cast<int>(m_frameCount.x);
	// �؂���ʒu�v�Z
	RECT src = {
		SPRITE_WIDTH * startCut,
		SPRITE_HEIGHT * static_cast<int>(m_frameCount.y),
		SPRITE_WIDTH * endCut,
		SPRITE_HEIGHT * static_cast<int>((m_frameCount.y + 1.0f))
	};
	// �X�v���C�g�o�b�`�`��J�n
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, state->NonPremultiplied());

	// ���[�h�摜�`��
	spriteBatch->Draw(
		m_loadingSprite.texture.Get(),				// �e�N�X�`��
		m_position,									// �`����W
		&src,										// �؂�����W
		DirectX::Colors::Yellow,					// �F
		0.0f,										// ��]
		DirectX::SimpleMath::Vector2::Zero,			// ���_
		SCALE										// �X�P�[��
	);

	spriteBatch->End();

}
