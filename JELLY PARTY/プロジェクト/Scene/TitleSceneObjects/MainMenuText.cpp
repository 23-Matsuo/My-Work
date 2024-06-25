//--------------------------------------------------------------------------------------
// File: MainMenuText.cpp
//
// �^�C�g�����S�N���X
//
// Date: 2023.12.02
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "MainMenuText.h"

const DirectX::SimpleMath::Vector2 MainMenuText::SCALE = { 0.5f,0.5f };

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
MainMenuText::MainMenuText(const float& width, const float& height)
	:
	m_acceleration(0.0f)
	, m_gravity(100.0f)
	, m_scale(SCALE)
	, m_damping{}
	, m_time(0.0f)
	, m_animated(false)
	, m_state(IDLE)
	, m_alpha(0.0f)
{
	m_windowSize = { width, height };
	// �ʒu��������
	m_position = { width / 4.0f, 0.0f};
}


//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
MainMenuText::~MainMenuText()
{
}


//------------------------------------------
// �X�V
//------------------------------------------
bool MainMenuText::Update(const float& elapsedTime)
{
	if (m_animated) return true;

	if (m_position.y >= m_windowSize.y / 7.0f )
	{
		m_time += elapsedTime;
		// �o�l�̓���
		Damping(&m_scale.y, &m_damping.y, SCALE.y);
		
		if (m_time > 1.0f)
		{
			m_animated = true;
			m_time = 0;
		}
	}
	else
	{
		m_acceleration += m_gravity * elapsedTime;
		m_position.y += m_acceleration ;
		m_scale.y += 0.005f;
		m_alpha += 0.01f;
	}

	return false;
}


//------------------------------------------
// �`��
//------------------------------------------
void MainMenuText::Render()
{
	auto* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// �^�C�g�����S���擾
	Resources::TextureTable menuTextTexture = Resources::GetInstance()->GetTexture(L"MainMenu");

	// �X�v���C�g�o�b�`�`��J�n
	spriteBatch->Begin();

	// �^�C�g���`��
	RECT rect = { 0 ,0 , menuTextTexture.textureWidth, menuTextTexture.textureHeight };

	spriteBatch->Draw(
		menuTextTexture.texture.Get(),
		m_position,
		&rect,
		DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha),
		0.0f,
		DirectX::SimpleMath::Vector2(static_cast<float>(rect.right) / 2.0f, static_cast<float>(rect.bottom)),
		m_scale);

	spriteBatch->End();

}

//------------------------------------------
// �o�l�̓���
//------------------------------------------
void MainMenuText::Damping(float* scale, float* velocity, float defaultScale)
{
	// �o�l�萔
	const float K = 0.08f;
	// ������
	const float FRICTION = 0.9f;

	float a = -K * (*scale - defaultScale);
	*velocity += a;
	*velocity *= FRICTION;
	*scale += *velocity;
}
