//--------------------------------------------------------------------------------------
// File: Result.cpp
//
// �J�E���g�_�E���N���X
//
// Date: 2023/12/09
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "Result.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Result::Result(const float& width, const float& height)
	:
	m_defaultScale(0.4f)
	, m_windowSize(width, height)
	, m_alpha(1.0f)
	, m_timer(0.0f)
	, m_acceleration(0.0f)
	, m_gravity(100.0f)
	, m_damping{}

{
	// �����ʒu
	m_position = { width / 2.0f, 0.0f };

	// �X�P�[��
	m_scale = m_defaultScale;
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
Result::~Result()
{

}

//------------------------------------------
// �X�V
//------------------------------------------
bool Result::Update()
{
	if (m_timer >= 1.5f) return true;

	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	if (m_position.y >= m_windowSize.y / 7.0f)
	{
		m_timer += elapsedTime;
		// �o�l�̓���
		Damping(&m_scale.y, &m_damping.y, m_defaultScale.y);
	}
	else
	{
		m_acceleration += m_gravity;
		m_position.y += m_acceleration * elapsedTime;
		m_scale.y += 0.01f;
	}
	return false;
}

//------------------------------------------
// �`��
//------------------------------------------
void Result::Render()
{

	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();
	Resources::TextureTable resultTexture = Resources::GetInstance()->GetTexture(L"Result");


	// 1�}�X���̉摜�T�C�Y
	LONG spriteWidth = static_cast<LONG>(resultTexture.textureWidth);
	LONG spriteHeight = static_cast<LONG>(resultTexture.textureHeight);

	// �摜�̐؂���ʒu
	RECT src = { 0, 0, spriteWidth, spriteHeight };


	// �`��
	spriteBatch->Begin();

	spriteBatch->Draw(
		resultTexture.texture.Get(),
		m_position,
		&src,
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, m_alpha),
		0.0f,
		DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, spriteHeight),
		m_scale
	);

	spriteBatch->End();
}


//------------------------------------------
// �o�l�̓���
//------------------------------------------
void Result::Damping(float* scale, float* velocity, float defaultScale)
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
