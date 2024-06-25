//--------------------------------------------------------------------------------------
// File: HowToPlay.cpp
//
// �^�C�g���w�i�N���X
//
// Date: 2023.12.02
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "HowToPlay.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
HowToPlay::HowToPlay(const float& width, const float& height)
	:
	m_scale{}
	, m_time(0.0f)
	, m_state(IDLE)
	, m_popped(false)
{
	m_windowSize = { width, height };
	// �ʒu��������
	m_position = { width / 2.0f + 150.0f, height - 75.0f};

	// �A�j���[�V�����p�����[�^��ǂݍ���
	Resources::GetInstance()->ReadJsonFile(&m_popAnimation, L"Resources/Datas/HowToPlayAnimation");

	// �X�P�[�����擾
	DirectX::SimpleMath::Vector2 defaultScale;
	for (const auto& scale : m_popAnimation["DEFAULT_SCALE"])
	{
		defaultScale = { scale[0], scale[1] };
	}
	m_defaultScale = defaultScale;

}


//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
HowToPlay::~HowToPlay()
{
}


//------------------------------------------
// �X�V
//------------------------------------------
void HowToPlay::Update(const float& elapsedTime)
{
}


//------------------------------------------
// �`��
//------------------------------------------
void HowToPlay::Render()
{
	auto* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// �^�C�g�����S���擾
	Resources::TextureTable howToPlay = Resources::GetInstance()->GetTexture(L"HowToPlay");

	// �X�v���C�g�o�b�`�`��J�n
	spriteBatch->Begin();

	// �^�C�g���`��
	RECT rect = { 0 ,0 , howToPlay.textureWidth, howToPlay.textureHeight };

	spriteBatch->Draw(
		howToPlay.texture.Get(),
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
void HowToPlay::Pop(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	float speed = static_cast<float>(m_popAnimation["ANIMATION_SPEED"]);

	switch (m_state)
	{
	case HowToPlay::IDLE:
		m_time = 0.0f;
		break;
	case HowToPlay::POPOUT:
		m_time += speed;
		PopOut();

		break;
	case HowToPlay::POPIN:
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
void HowToPlay::PopOut()
{
	if (m_scale.x <= 0.0f) return;

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
void HowToPlay::PopIn()
{
	float scaleChangeAmount = 0;

	if (m_time > m_popAnimation["DURATION"])
	{
		m_time = 0.0f;
		m_scale = m_defaultScale;

		m_state = IDLE;
		m_popped = true;
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
