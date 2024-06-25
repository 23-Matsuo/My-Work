//--------------------------------------------------------------------------------------
// File: CountDown.cpp
//
// �J�E���g�_�E���N���X
//
// Date: 2023/11/01
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "CountDown.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
CountDown::CountDown()
	:
	m_countDown(180)
	, m_scale(1.0f)
	, m_alpha(0.0f)
	, m_animated(false)
{

}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
CountDown::~CountDown()
{

}

//------------------------------------------
// �X�V
//------------------------------------------
bool CountDown::Update()
{
	if (m_countDown % 60 == 0)
	{
		m_scale = 1.0f;
		m_alpha = 0.0f;
		m_animated = true;
	}

	if (m_animated)
	{
		m_scale -= 0.02f;
		m_alpha += 0.1f;

		if (m_scale <= 0.5f)
		{
			m_scale = 0.5f;
		}

		if (m_alpha >= 1)
		{
			m_animated = false;
		}
	}

	// �J�E���g�_�E��
	m_countDown--;
	// �J�E���g�_�E����0�ɂȂ�����X�^�[�g
	if (m_countDown <= -1)
	{
		return true;
	}

	return false;
}

//------------------------------------------
// �`��
//------------------------------------------
void CountDown::Render()
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();
	Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"CountDownSprite");
	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	// 1�}�X���̉摜�T�C�Y
	LONG spriteWidth = static_cast<LONG>(numberTexture.textureWidth / 3);
	LONG spriteHeight = static_cast<LONG>(numberTexture.textureHeight);

	// �t���[������b�ɕϊ�
	int count = m_countDown / 60;


	// �摜�̐؂���ʒu
	RECT src = { spriteWidth * count, 0, spriteWidth * (count + 1), spriteHeight };

	// ���S�ʒu
	DirectX::SimpleMath::Vector2 pos = DirectX::SimpleMath::Vector2(static_cast<float>(rect.right) / 2.0f, static_cast<float>(rect.bottom) / 2.0f);

	// �`��
	spriteBatch->Begin();

	spriteBatch->Draw(
		numberTexture.texture.Get(),
		pos,
		&src,
		DirectX::SimpleMath::Color(1.0f,1.0f,1.0f,m_alpha),
		0.0f,
		DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, spriteHeight / 2.0f),
		m_scale
		);

	spriteBatch->End();
}

