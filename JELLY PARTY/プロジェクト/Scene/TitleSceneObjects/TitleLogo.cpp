//--------------------------------------------------------------------------------------
// File: TitleLogo.cpp
//
// �^�C�g�����S�N���X
//
// Date: 2023.12.02
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "TitleLogo.h"
#include "Scene/GameManager.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
TitleLogo::TitleLogo(const float& width, const float& height)
	:
	m_acceleration(0.0f)
	, m_gravity(100.0f)
	, m_damping{}
	, m_time(0.0f)
	, m_state(IDLE)
{
	m_windowSize = { width, height };
	// �ʒu��������
	m_position = { width / 2.0f, 0.0f };

	if (GameManager::GetInstance()->GetTitleAnimated())
	{
		m_position = { width / 2.0f, height / 2.2f};
	}

	// �A�j���[�V�����p�����[�^��ǂݍ���
	Resources::GetInstance()->ReadJsonFile(&m_popAnimation, L"Resources/Datas/TitleLogoAnimation");

	// �X�P�[�����擾
	DirectX::SimpleMath::Vector2 defaultScale;
	for (const auto &scale : m_popAnimation["DEFAULT_SCALE"])
	{
		defaultScale = { scale[0], scale[1] };
	}

	m_scale = defaultScale;
	m_defaultScale = defaultScale;
}


//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
TitleLogo::~TitleLogo()
{
}

//------------------------------------------
// ������
//------------------------------------------
void TitleLogo::Initialize()
{
	// �ʒu��������
	m_position = { m_windowSize.x / 2.0f, 0.0f };
	m_damping = {};
	m_acceleration = 0.0f;
	m_time = 0.0f;
	m_scale = m_defaultScale;

	GameManager::GetInstance()->SetTitleAnimated(false);
}

//------------------------------------------
// �X�V
//------------------------------------------
bool TitleLogo::Update(const float& elapsedTime)
{
	if (GameManager::GetInstance()->GetTitleAnimated()) return true;

	if (m_position.y >= m_windowSize.y / 2.2f )
	{
		m_time += elapsedTime;
		// �o�l�̓���
		Damping(&m_scale.y, &m_damping.y, m_defaultScale.y);
		
		if (m_time > 1.0f)
		{
			GameManager::GetInstance()->SetTitleAnimated(true);
			m_time = 0;
		}
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
void TitleLogo::Render()
{
	auto* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// �^�C�g�����S���擾
	Resources::TextureTable titleTexture = Resources::GetInstance()->GetTexture(L"Title");

	// �X�v���C�g�o�b�`�`��J�n
	spriteBatch->Begin();

	// �^�C�g���`��
	RECT rect = { 0 ,0 , titleTexture.textureWidth, titleTexture.textureHeight };

	spriteBatch->Draw(
		titleTexture.texture.Get(),
		m_position,
		&rect,
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2(static_cast<float>(rect.right / 2), static_cast<float>(rect.bottom)),
		m_scale);

	spriteBatch->End();
}

//------------------------------------------
// �|�b�v�A�j���[�V����
//------------------------------------------
void TitleLogo::Pop(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	float speed = static_cast<float>(m_popAnimation["ANIMATION_SPEED"]);
	switch (m_state)
	{
	case TitleLogo::IDLE:
		break;
	case TitleLogo::POPOUT:
		m_time += speed;
		PopOut();

		break;
	case TitleLogo::POPIN:
		m_time += speed;
		PopIn();
		break;
	default:
		break;
	}
}

//------------------------------------------
// �|�b�v�A�E�g
//------------------------------------------
void TitleLogo::PopOut()
{
	float scaleChangeAmount = 0;

	if (m_time > m_popAnimation["DURATION"])
	{
		m_time = 0.0f;
		m_scale = {};
		m_state = IDLE;
	}
	else if (m_time > m_popAnimation["POP_OUT"]["KEY2"]["FRAME"])
	{
		scaleChangeAmount = m_popAnimation["POP_OUT"]["KEY2"]["AMOUNT"];

		m_scale -= DirectX::SimpleMath::Vector2(scaleChangeAmount, scaleChangeAmount);
	}
	else if (m_time > m_popAnimation["POP_OUT"]["KEY1"]["FRAME"])
	{
		scaleChangeAmount = m_popAnimation["POP_OUT"]["KEY1"]["AMOUNT"];

		m_scale += DirectX::SimpleMath::Vector2(scaleChangeAmount, scaleChangeAmount);
	}
}

//------------------------------------------
// �|�b�v�C��
//------------------------------------------
void TitleLogo::PopIn()
{
	float scaleChangeAmount = 0;

	if (m_time > m_popAnimation["DURATION"])
	{
		m_time = 0.0f;
		m_scale = m_defaultScale;

		m_state = IDLE;
	}
	else if (m_time > m_popAnimation["POP_IN"]["KEY2"]["FRAME"])
	{
		scaleChangeAmount = m_popAnimation["POP_IN"]["KEY2"]["AMOUNT"];

		m_scale -= DirectX::SimpleMath::Vector2(scaleChangeAmount, scaleChangeAmount);
	}
	else if (m_time > m_popAnimation["POP_IN"]["KEY1"]["FRAME"])
	{
		scaleChangeAmount = m_popAnimation["POP_IN"]["KEY1"]["AMOUNT"];

		m_scale += DirectX::SimpleMath::Vector2(scaleChangeAmount, scaleChangeAmount);
	}

}

//------------------------------------------
// �o�l�̓���
//------------------------------------------
void TitleLogo::Damping(float* scale, float* velocity, float defaultScale)
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
