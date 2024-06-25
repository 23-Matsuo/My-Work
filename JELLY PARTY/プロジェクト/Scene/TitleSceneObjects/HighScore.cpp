//--------------------------------------------------------------------------------------
// File: HighScore.cpp
//
// �Q�[���̃^�C�}�[�N���X
//
// Date: 2023/11/01
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "HighScore.h"
#include "MyLib/ObjectCollision.h"
#include "Scene/GameManager.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
HighScore::HighScore(const float& width, const float& height)
	:
	m_scale(0.6f)
{
	Resources::GetInstance()->ReadJsonFile(&m_data, L"Resources/Datas/ScoreCountAnimation");
	m_spriteColumn = m_data["NUMBER_SPRITE_COLUMN"];
	m_spriteRow = m_data["NUMBER_SPRITE_ROW"];

	// �����̈ʒu
	m_position = DirectX::SimpleMath::Vector2(width - width / 8.25f, height / 2.8f);

	// �ō��X�R�A���擾
	m_score = GameManager::GetInstance()->GetHighScore();

	// �N�����v
	if (m_score >= 1000)
	{
		m_score = 999;
		GameManager::GetInstance()->SetHighScore(m_score);
	}
	if (m_score < 0)
	{
		m_score = 0;
		GameManager::GetInstance()->SetHighScore(m_score);
	}
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
HighScore::~HighScore()
{

}

//------------------------------------------
// ������
//------------------------------------------
void HighScore::Initialize()
{
	m_score = 0;
}

//------------------------------------------
// �X�V
//------------------------------------------
void HighScore::Update()
{
}

//------------------------------------------
// �`��
//------------------------------------------
void HighScore::Render()
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// �摜�擾
	Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"NumberSprite");

	// 1�}�X���̉摜�T�C�Y
	LONG spriteWidth  = static_cast<LONG>(numberTexture.textureWidth / m_spriteColumn);
	LONG spriteHeight = static_cast<LONG>(numberTexture.textureHeight / m_spriteRow);

	// ������3���ɕ���
	int number[3] = {};
	number[0] = m_score % 10;
	number[1] = m_score / 10 % 10;
	number[2] = m_score / 100 % 10;

	// �؂���ʒu
	// 1����
	RECT src1 = { spriteWidth * number[0], 0, spriteWidth * (number[0] + 1), spriteHeight };
	// 2����
	RECT src2 = { spriteWidth * number[1], 0, spriteWidth * (number[1] + 1), spriteHeight };
	// 3����
	RECT src3 = { spriteWidth * number[2], 0, spriteWidth * (number[2] + 1), spriteHeight };

	// �n�C�t��
	RECT srcNothing = { spriteWidth * 2, spriteHeight, spriteWidth * 3, spriteHeight * 2};

	spriteBatch->Begin();

	DirectX::SimpleMath::Vector2 texturePos = m_position;

	if (m_score == 0)
	{
		texturePos.x += spriteWidth * m_scale;
		spriteBatch->Draw(
			numberTexture.texture.Get(),
			texturePos,
			&srcNothing,
			DirectX::Colors::White,
			0.0f,
			DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, spriteHeight / 2.0f),
			m_scale
		);
	}
	else
	{
		// 3����
		spriteBatch->Draw(
			numberTexture.texture.Get(),
			texturePos,
			&src3,
			DirectX::Colors::White,
			0.0f,
			DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, spriteHeight / 2.0f),
			m_scale
		);
		
		// 2����
		texturePos.x += spriteWidth * m_scale;
		spriteBatch->Draw(
			numberTexture.texture.Get(),
			texturePos,
			&src2,
			DirectX::Colors::White,
			0.0f,
			DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, spriteHeight / 2.0f),
			m_scale
		);
		
		// 1����
		texturePos.x += spriteWidth * m_scale;
		spriteBatch->Draw(
			numberTexture.texture.Get(),
			texturePos,
			&src1,
			DirectX::Colors::White,
			0.0f,
			DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, spriteHeight / 2.0f),
			m_scale
		);
	}

	spriteBatch->End();
}