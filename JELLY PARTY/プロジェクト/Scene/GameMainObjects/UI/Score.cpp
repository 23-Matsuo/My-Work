//--------------------------------------------------------------------------------------
// File: Score.cpp
//
// ゲームのタイマークラス
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
// コンストラクタ
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

	// アニメーションパラメータを読み込む
	Resources::GetInstance()->ReadJsonFile(&m_countAnimation, L"Resources/Datas/ScoreCountAnimation");
	m_spriteColumn = m_countAnimation["NUMBER_SPRITE_COLUMN"];
	m_spriteRow = m_countAnimation["NUMBER_SPRITE_ROW"];

	// スケールを取得
	m_backScale = static_cast<float>(m_countAnimation["BACK_DEFAULT_SCALE"]);
	m_iconScale = static_cast<float>(m_countAnimation["ICON_DEFAULT_SCALE"]);
	m_countScale = static_cast<float>(m_countAnimation["NUMBER_DEFAULT_SCALE"]);

	// テクスチャ情報を取得
	Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"NumberSprite");
	m_spriteWidth = static_cast<float>(numberTexture.textureWidth / m_spriteColumn);
	m_spriteHeight = static_cast<float>(numberTexture.textureHeight / m_spriteRow);
	// アイコンの位置
	m_iconPosition = DirectX::SimpleMath::Vector2(
		m_windowSize.x / 2.0f - m_spriteWidth * 1.5f, m_spriteHeight);
	// 背景の位置
	m_backPosition = DirectX::SimpleMath::Vector2(m_windowSize.x / 2.0f, m_iconPosition.y);
	// 数字の位置
	m_countPosition = m_iconPosition;

	// 「ハイスコア！」オブジェクトの作成
	m_highScoreText = std::make_unique<HighScoreText>(m_windowSize.x, m_windowSize.y);

	// ハイスコア更新時の紙吹雪演出
	m_resultConfetti = std::make_unique<ResultConfetti>();
	m_resultConfetti->Create(pDR);


	// ジャンプ音
	m_highScoreSound = Resources::GetInstance()->GetSound(L"HighScore");
	// 拍手音
	m_clapSound = Resources::GetInstance()->GetSound(L"Clap");
}

//------------------------------------------
// デストラクタ
//------------------------------------------
Score::~Score()
{
	m_highScoreSound->Stop();
	m_clapSound->Stop();

	m_highScoreSound = nullptr;
	m_clapSound = nullptr;
}

//------------------------------------------
// 初期化
//------------------------------------------
void Score::Initialize()
{
	m_score = 0;
	m_color = DirectX::Colors::White;
}

//------------------------------------------
// 更新
//------------------------------------------
void Score::Update()
{
	// 経過時間取得
	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	// ハイスコア取得
	int highScore = GameManager::GetInstance()->GetHighScore();

	// 最高スコアより大きくなったかつハイスコアが0じゃないとき
	if (highScore < m_score && highScore != 0)
	{
		m_color = DirectX::Colors::LightYellow;
	}

	// シーンの状態
	if (m_sceneState == RESULT)
	{
		if (highScore < m_score)
		{
			m_color = DirectX::Colors::LightYellow;
			m_isHighScore = true;
			// スケール変更
			m_countScale = lerp(m_countScale, static_cast<float>(m_countAnimation["NUMBER_HIGHSCORE_SCALE"]), 0.5f);

			if (m_resultCountFinished)
			{
				// ハイスコア時のサウンド
				if (!m_highScoreSoundPlayed)
				{
					m_highScoreSound->Play();
					m_clapSound->Play();
					m_highScoreSoundPlayed = true;
				}

				// カウント終了したら
				// 「ハイスコア！」オブジェクト更新
				m_highScoreText->Update();

				// 紙吹雪演出作成
				m_resultConfetti->CreateConfetti(elapsedTime);
				m_resultConfetti->Update(elapsedTime);
			}
		}
		else
		{
			Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"NumberSprite");

			// 位置を上の方に調節
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
// 描画
//------------------------------------------
void Score::Render()
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// 画像取得
	Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"NumberSprite");
	Resources::TextureTable icon = Resources::GetInstance()->GetTexture(L"Bait");
	Resources::TextureTable back = Resources::GetInstance()->GetTexture(L"BaitNumBack");
	Resources::TextureTable highScore = Resources::GetInstance()->GetTexture(L"HighScore");

	// 数字を3桁に分解
	int number[3] = {};
	number[0] = m_score % 10;
	number[1] = m_score / 10 % 10;
	number[2] = m_score / 100 % 10;

	LONG spWidth =  static_cast<LONG>(m_spriteWidth);
	LONG spHeight = static_cast<LONG>(m_spriteHeight);
	// 1桁目
	RECT src1 = { spWidth * number[0], 0, spWidth * (number[0] + 1), spHeight };
	// 2桁目	
	RECT src2 = { spWidth * number[1], 0, spWidth * (number[1] + 1), spHeight };
	// 3桁目
	RECT src3 = { spWidth * number[2], 0, spWidth * (number[2] + 1), spHeight };

	// 数字の位置をiconの位置を中心に考える
	m_countPosition.x = m_iconPosition.x;

	spriteBatch->Begin();

	// 背景の描画
	spriteBatch->Draw(
		back.texture.Get(),
		m_backPosition,
		{},
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2(back.textureWidth / 2.0f, back.textureHeight / 2.0f),
		m_backScale
	);

	// アイコンの描画
	spriteBatch->Draw(
		icon.texture.Get(),
		m_iconPosition,
		{},
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2(icon.textureWidth / 2.0f, icon.textureHeight / 2.0f),
		m_iconScale
		);


	// 3桁目
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

	// 2桁目
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
	// 1桁目
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
// カウントアップ
//------------------------------------------
void Score::CountUp()
{
	m_score ++; 
	m_state = COUNTUP;
}

//------------------------------------------
// カウントアップのアニメーション
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
