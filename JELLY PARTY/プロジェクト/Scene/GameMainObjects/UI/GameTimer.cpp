//--------------------------------------------------------------------------------------
// File: GameTimer.cpp
//
// �Q�[���̃^�C�}�[�N���X
//
// Date: 2023/11/01
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "GameTimer.h"
#include "MyLib/Graphics.h"
#include "MyLib/Resources.h"


// �c�莞�Ԃ̎n�܂�̒l�i�b�j
const int GameTimer::TOTAL_SECONDS = 90;
// �X�v���C�g�̗�
const float GameTimer::SPRITE_COLUMN = 10;
// �X�v���C�g�̍s��
const float GameTimer::SPRITE_ROW = 2;

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
GameTimer::GameTimer()
	:
	m_spendingTime(0)
	, m_color(DirectX::Colors::White)
	, m_scale(0.8f)
{
	m_remainingTime = TOTAL_SECONDS;
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
GameTimer::~GameTimer()
{

}

//------------------------------------------
// �X�V
//------------------------------------------
bool GameTimer::Update()
{
	// �o�ߎ��Ԃ��v��
	m_spendingTime++;

	// �o�ߎ��Ԃ�b�ɒ���
	int second = m_spendingTime / 60;

	// �c�莞�Ԃ��v�Z
	m_remainingTime = TOTAL_SECONDS - second;

	// �c�莞�Ԃ�0�ɂȂ�����Q�[���I�[�o�[
	if (m_remainingTime <= 0.0f)
	{
		return true;
	}

	if (m_remainingTime <= 3.0f)
	{
		m_color = DirectX::Colors::OrangeRed;
	}
	else if (m_remainingTime <= 4.0f)
	{
		m_color = DirectX::Colors::Tomato;
	}
	else if (m_remainingTime <= 5.0f)
	{
		m_color = DirectX::Colors::Coral;
	}

	return false;
}

//------------------------------------------
// �`��
//------------------------------------------
void GameTimer::Render()
{
	if (m_remainingTime <= 0) return;

	Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"NumberSprite");
	RECT windowSize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	// 1�}�X���̉摜�T�C�Y
	LONG spriteWidth  = static_cast<LONG>(numberTexture.textureWidth / SPRITE_COLUMN);
	LONG spriteHeight = static_cast<LONG>(numberTexture.textureHeight / SPRITE_ROW);


	int time[4];
	// �b
	time[0] = (m_remainingTime % 60) % 10;
	time[1] = (m_remainingTime % 60) / 10;
	// ��
	time[2] = (m_remainingTime / 60) % 10;
	time[3] = (m_remainingTime / 60) / 10;
	// �؂���ʒu
	RECT src0 = { 0, spriteHeight, spriteWidth, spriteHeight * 2 };
	RECT src1 = { spriteWidth * time[0], 0, spriteWidth * (time[0] + 1), spriteHeight };
	RECT src2 = { spriteWidth * time[1], 0, spriteWidth * (time[1] + 1), spriteHeight };
	RECT src3 = { spriteWidth * time[2], 0, spriteWidth * (time[2] + 1), spriteHeight };
	RECT src4 = { spriteWidth * time[3], 0, spriteWidth * (time[3] + 1), spriteHeight };

	// ��ʒu
	DirectX::SimpleMath::Vector2 basePosition = { static_cast<float>(windowSize.right) / 10.0f, 30.0f };

	// ���S�ʒu
	DirectX::SimpleMath::Vector2 pos = basePosition;
	RenderTimer(pos, src0);
	// �b
	pos.x += spriteWidth * m_scale;
	RenderTimer(pos, src2);
	pos.x += spriteWidth * m_scale;
	RenderTimer(pos, src1);
	// ��
	pos = basePosition;
	pos.x -= spriteWidth * m_scale;
	RenderTimer(pos, src3);
	pos.x -= spriteWidth * m_scale;
	RenderTimer(pos, src4);
}

//------------------------------------------
// �^�C�}�[��`��
//------------------------------------------
void GameTimer::RenderTimer(const DirectX::SimpleMath::Vector2& position, const RECT& cutPos)
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"NumberSprite");
	// 1�}�X���̉摜�T�C�Y
	float spriteWidth = static_cast<float>(numberTexture.textureWidth / SPRITE_COLUMN);

	spriteBatch->Begin();

	spriteBatch->Draw(
		numberTexture.texture.Get(),
		position, 
		&cutPos,
		m_color,
		0.0f,
		DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, 0.0f),
		m_scale
	);

	spriteBatch->End();
}
