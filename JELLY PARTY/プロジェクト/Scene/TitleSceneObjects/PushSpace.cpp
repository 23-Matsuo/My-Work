//--------------------------------------------------------------------------------------
// File: PushSpace.cpp
//
// �^�C�g���w�i�N���X
//
// Date: 2023.12.02
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "PushSpace.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
PushSpace::PushSpace(const float& width, const float& height)
	:
	m_time(0.0f)
	, m_state(IDLE)
{
	m_windowSize = { width, height };
	// �ʒu��������
	m_position = { width / 2.0f, height - height / 5.0f};

	// �A�j���[�V�����p�����[�^��ǂݍ���
	Resources::GetInstance()->ReadJsonFile(&m_popAnimation, L"Resources/Datas/PushSpaceAnimation");

	// �X�P�[�����擾
	DirectX::SimpleMath::Vector2 defaultScale;
	for (const auto& scale : m_popAnimation["DEFAULT_SCALE"])
	{
		defaultScale = { scale[0], scale[1] };
	}

	m_scale = {};
	m_defaultScale = defaultScale;

}


//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
PushSpace::~PushSpace()
{
}


//------------------------------------------
// �X�V
//------------------------------------------
void PushSpace::Update(const float& elapsedTime)
{
	float t = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetTotalSeconds());
	m_position.y += sinf(t) / 2.0f;

}

//------------------------------------------
// �`��
//------------------------------------------
void PushSpace::Render()
{
	auto* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// �^�C�g�����S���擾
	Resources::TextureTable pushSpaceTexture = Resources::GetInstance()->GetTexture(L"PressSpace");

	// �X�v���C�g�o�b�`�`��J�n
	spriteBatch->Begin();

	// �^�C�g���`��
	RECT rect = { 0 ,0 , pushSpaceTexture.textureWidth, pushSpaceTexture.textureHeight };

	spriteBatch->Draw(
		pushSpaceTexture.texture.Get(),
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
void PushSpace::Pop(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	float speed = static_cast<float>(m_popAnimation["ANIMATION_SPEED"]);

	switch (m_state)
	{
	case PushSpace::IDLE:
		break;
	case PushSpace::POPOUT:
		m_time += speed;
		PopOut();

		break;
	case PushSpace::POPIN:
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
void PushSpace::PopOut()
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
void PushSpace::PopIn()
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
