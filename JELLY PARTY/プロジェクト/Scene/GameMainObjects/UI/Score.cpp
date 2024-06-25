//--------------------------------------------------------------------------------------
// File: Score.cpp
//
// �Q�[���̃^�C�}�[�N���X
//
// Date: 2023/11/01
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "Score.h"
#include "MyLib/ObjectCollision.h"
#include "Scene/GameManager.h"

inline float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Score::Score(const float& width, const float& height)
	:
	m_windowSize(width, height)
	, m_score(0)
	, m_state(STATE::IDLE)
	, m_sceneState(SceneState::MAIN)
	, m_timer(0.0f)
	, m_color(DirectX::Colors::White)
	, m_resultCountFinished(false)
	, m_isHighScore(false)
	, m_highScoreSoundPlayed(false)
{
	ObjectCollision::GetInstance()->SetScore(this);
	DX::DeviceResources* pDR = Graphics::GetInstance()->GetDeviceResources();

	// �A�j���[�V�����p�����[�^��ǂݍ���
	Resources::GetInstance()->ReadJsonFile(&m_countAnimation, L"Resources/Datas/ScoreCountAnimation");
	m_spriteColumn = m_countAnimation["NUMBER_SPRITE_COLUMN"];
	m_spriteRow = m_countAnimation["NUMBER_SPRITE_ROW"];

	// �X�P�[�����擾
	m_backScale = static_cast<float>(m_countAnimation["BACK_DEFAULT_SCALE"]);
	m_iconScale = static_cast<float>(m_countAnimation["ICON_DEFAULT_SCALE"]);
	m_countScale = static_cast<float>(m_countAnimation["NUMBER_DEFAULT_SCALE"]);

	// �e�N�X�`�������擾
	Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"NumberSprite");
	m_spriteWidth = static_cast<float>(numberTexture.textureWidth / m_spriteColumn);
	m_spriteHeight = static_cast<float>(numberTexture.textureHeight / m_spriteRow);
	// �A�C�R���̈ʒu
	m_iconPosition = DirectX::SimpleMath::Vector2(
		m_windowSize.x / 2.0f - m_spriteWidth * 1.5f, m_spriteHeight);
	// �w�i�̈ʒu
	m_backPosition = DirectX::SimpleMath::Vector2(m_windowSize.x / 2.0f, m_iconPosition.y);
	// �����̈ʒu
	m_countPosition = m_iconPosition;

	// �u�n�C�X�R�A�I�v�I�u�W�F�N�g�̍쐬
	m_highScoreText = std::make_unique<HighScoreText>(m_windowSize.x, m_windowSize.y);

	// �n�C�X�R�A�X�V���̎����቉�o
	m_resultConfetti = std::make_unique<ResultConfetti>();
	m_resultConfetti->Create(pDR);


	// �W�����v��
	m_highScoreSound = Resources::GetInstance()->GetSound(L"HighScore");
	// ���艹
	m_clapSound = Resources::GetInstance()->GetSound(L"Clap");
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
Score::~Score()
{
	m_highScoreSound->Stop();
	m_clapSound->Stop();

	m_highScoreSound = nullptr;
	m_clapSound = nullptr;
}

//------------------------------------------
// ������
//------------------------------------------
void Score::Initialize()
{
	m_score = 0;
	m_color = DirectX::Colors::White;
}

//------------------------------------------
// �X�V
//------------------------------------------
void Score::Update()
{
	// �o�ߎ��Ԏ擾
	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	// �n�C�X�R�A�擾
	int highScore = GameManager::GetInstance()->GetHighScore();

	// �ō��X�R�A���傫���Ȃ������n�C�X�R�A��0����Ȃ��Ƃ�
	if (highScore < m_score && highScore != 0)
	{
		m_color = DirectX::Colors::LightYellow;
	}

	// �V�[���̏��
	if (m_sceneState == RESULT)
	{
		if (highScore < m_score)
		{
			m_color = DirectX::Colors::LightYellow;
			m_isHighScore = true;
			// �X�P�[���ύX
			m_countScale = lerp(m_countScale, static_cast<float>(m_countAnimation["NUMBER_HIGHSCORE_SCALE"]), 0.5f);

			if (m_resultCountFinished)
			{
				// �n�C�X�R�A���̃T�E���h
				if (!m_highScoreSoundPlayed)
				{
					m_highScoreSound->Play();
					m_clapSound->Play();
					m_highScoreSoundPlayed = true;
				}

				// �J�E���g�I��������
				// �u�n�C�X�R�A�I�v�I�u�W�F�N�g�X�V
				m_highScoreText->Update();

				// �����቉�o�쐬
				m_resultConfetti->CreateConfetti(elapsedTime);
				m_resultConfetti->Update(elapsedTime);
			}
		}
		else
		{
			Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"NumberSprite");

			// �ʒu����̕��ɒ���
			m_iconPosition = DirectX::SimpleMath::Vector2(
				m_windowSize.x / 2.0f - m_spriteWidth * 1.5f,
				m_windowSize.y / 2.0f - m_spriteHeight * 3.0f);
			m_backPosition = DirectX::SimpleMath::Vector2(
				m_windowSize.x / 2.0f,
				m_iconPosition.y);

			m_countPosition = m_iconPosition;

			m_backScale = static_cast<float>(m_countAnimation["BACK_RESULT_SCALE"]);
			m_iconScale = static_cast<float>(m_countAnimation["ICON_RESULT_SCALE"]);
			m_countScale = static_cast<float>(m_countAnimation["NUMBER_RESULT_SCALE"]);
		}
	}


	if (m_state == IDLE) return;

	
	m_timer += elapsedTime;
	if (m_state == COUNTUP)
	{
		CountUpAnimation();
	}
}

//------------------------------------------
// �`��
//------------------------------------------
void Score::Render()
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// �摜�擾
	Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"NumberSprite");
	Resources::TextureTable icon = Resources::GetInstance()->GetTexture(L"Bait");
	Resources::TextureTable back = Resources::GetInstance()->GetTexture(L"BaitNumBack");
	Resources::TextureTable highScore = Resources::GetInstance()->GetTexture(L"HighScore");

	// ������3���ɕ���
	int number[3] = {};
	number[0] = m_score % 10;
	number[1] = m_score / 10 % 10;
	number[2] = m_score / 100 % 10;

	LONG spWidth =  static_cast<LONG>(m_spriteWidth);
	LONG spHeight = static_cast<LONG>(m_spriteHeight);
	// 1����
	RECT src1 = { spWidth * number[0], 0, spWidth * (number[0] + 1), spHeight };
	// 2����	
	RECT src2 = { spWidth * number[1], 0, spWidth * (number[1] + 1), spHeight };
	// 3����
	RECT src3 = { spWidth * number[2], 0, spWidth * (number[2] + 1), spHeight };

	// �����̈ʒu��icon�̈ʒu�𒆐S�ɍl����
	m_countPosition.x = m_iconPosition.x;

	spriteBatch->Begin();

	// �w�i�̕`��
	spriteBatch->Draw(
		back.texture.Get(),
		m_backPosition,
		{},
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2(back.textureWidth / 2.0f, back.textureHeight / 2.0f),
		m_backScale
	);

	// �A�C�R���̕`��
	spriteBatch->Draw(
		icon.texture.Get(),
		m_iconPosition,
		{},
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2(icon.textureWidth / 2.0f, icon.textureHeight / 2.0f),
		m_iconScale
		);


	// 3����
	m_countPosition.x += m_spriteWidth;
	spriteBatch->Draw(
		numberTexture.texture.Get(),
		m_countPosition,
		&src3,
		m_color,
		0.0f,
		DirectX::SimpleMath::Vector2(m_spriteWidth / 2.0f, m_spriteHeight / 2.0f),
		m_countScale
		);

	// 2����
	m_countPosition.x += m_spriteWidth * m_countScale;
	spriteBatch->Draw(
		numberTexture.texture.Get(),
		m_countPosition,
		&src2,
		m_color,
		0.0f,
		DirectX::SimpleMath::Vector2(m_spriteWidth / 2.0f, m_spriteHeight / 2.0f),
		m_countScale
	);
	// 1����
	m_countPosition.x += m_spriteWidth * m_countScale;
	spriteBatch->Draw(
		numberTexture.texture.Get(),
		m_countPosition,
		&src1,
		m_color,
		0.0f,
		DirectX::SimpleMath::Vector2(m_spriteWidth / 2.0f, m_spriteHeight / 2.0f),
		m_countScale
	);

	spriteBatch->End();


	if (m_isHighScore && m_resultCountFinished)
	{
		m_highScoreText->Render();
		m_resultConfetti->Render();
	}
}

//------------------------------------------
// �J�E���g�A�b�v
//------------------------------------------
void Score::CountUp()
{
	m_score ++; 
	m_state = COUNTUP;
}

//------------------------------------------
// �J�E���g�A�b�v�̃A�j���[�V����
//------------------------------------------
void Score::CountUpAnimation()
{
	if (m_timer > m_countAnimation["DURATION"])
	{
		m_state = IDLE;
		m_timer = 0.0f;
		m_countPosition.y = m_iconPosition.y;
	}
	else if (m_timer > m_countAnimation["COUNT_UP"]["KEY2"]["FRAME"])
	{
		m_countPosition.y += static_cast<float>(m_countAnimation["COUNT_UP"]["KEY2"]["AMOUNT"]);
	}
	else if (m_timer > m_countAnimation["COUNT_UP"]["KEY1"]["FRAME"])
	{
		m_countPosition.y -= static_cast<float>(m_countAnimation["COUNT_UP"]["KEY1"]["AMOUNT"]);
	}
}
