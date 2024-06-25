//--------------------------------------------------------------------------------------
// File: HighScoreText.cpp
//
// �u�n�C�X�R�A�I�v�N���X
//
// Date: 2023/01/04
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "HighScoreText.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
HighScoreText::HighScoreText(const float& width, const float& height)
	:
	m_defaultScale(0.8f)
	, m_windowSize(width, height)
	, m_alpha(0.0f)
	, m_timer(0.0f)
{
	// �����ʒu
	m_position = { m_windowSize.x / 1.5f, m_windowSize.y / 3.0f };

	// �X�P�[��
	m_scale = m_defaultScale;
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
HighScoreText::~HighScoreText()
{

}

//------------------------------------------
// �X�V
//------------------------------------------
bool HighScoreText::Update()
{
	if (m_timer >= 1.5f) return true;
	
	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	m_timer += elapsedTime;

	// �X�P�[���ω�
	if (m_scale >= 0.45f)
	{
		m_scale -= 0.05f;
	}
	// �����x�ω�
	if (m_alpha <= 1.0f)
	{
		m_alpha += 0.2f;
	}

	return false;
}

//------------------------------------------
// �`��
//------------------------------------------
void HighScoreText::Render()
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();
	Resources::TextureTable highScoreTexture = Resources::GetInstance()->GetTexture(L"HighScore");


	// 1�}�X���̉摜�T�C�Y
	LONG spriteWidth = static_cast<LONG>(highScoreTexture.textureWidth);
	LONG spriteHeight = static_cast<LONG>(highScoreTexture.textureHeight);

	// �摜�̐؂���ʒu
	RECT src = { 0, 0, spriteWidth, spriteHeight };

	// �`��
	spriteBatch->Begin();

	spriteBatch->Draw(
		highScoreTexture.texture.Get(),
		m_position,
		&src,
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, m_alpha),
		0.2f,
		DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, spriteHeight),
		m_scale
	);

	spriteBatch->End();
}