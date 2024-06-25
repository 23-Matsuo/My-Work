//--------------------------------------------------------------------------------------
// File: Finish.cpp
//
// �J�E���g�_�E���N���X
//
// Date: 2023/12/09
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "Finish.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Finish::Finish()
	:
	m_scale(1.0f)
	, m_alpha(0.0f)
	, m_timer(0.0f)
{

}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
Finish::~Finish()
{

}

//------------------------------------------
// �X�V
//------------------------------------------
bool Finish::Update()
{
	if (m_timer >= 1.5f) return true;

	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	m_timer += elapsedTime;

	if (m_scale >= 0.5f)
	{
		m_scale -= 0.02f;
	}
	if (m_alpha <= 1.0f)
	{
		m_alpha += 0.1f;
	}

	return false;
}

//------------------------------------------
// �`��
//------------------------------------------
void Finish::Render()
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();
	Resources::TextureTable doWhatTexture = Resources::GetInstance()->GetTexture(L"Finish");

	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	// 1�}�X���̉摜�T�C�Y
	LONG spriteWidth = static_cast<LONG>(doWhatTexture.textureWidth);
	LONG spriteHeight = static_cast<LONG>(doWhatTexture.textureHeight);

	// �摜�̐؂���ʒu
	RECT src = { 0, 0, spriteWidth, spriteHeight };

	// ���S�ʒu
	DirectX::SimpleMath::Vector2 pos = DirectX::SimpleMath::Vector2(static_cast<float>(rect.right) / 2.0f, static_cast<float>(rect.bottom) / 2.0f);

	// �`��
	spriteBatch->Begin();

	spriteBatch->Draw(
		doWhatTexture.texture.Get(),
		pos,
		&src,
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, m_alpha),
		0.0f,
		DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, spriteHeight / 2.0f),
		m_scale
	);

	spriteBatch->End();
}