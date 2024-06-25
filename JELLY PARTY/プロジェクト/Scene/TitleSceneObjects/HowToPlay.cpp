//--------------------------------------------------------------------------------------
// File: HowToPlay.cpp
//
// タイトル背景クラス
//
// Date: 2023.12.02
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "HowToPlay.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
HowToPlay::HowToPlay(const float& width, const float& height)
	:
	m_scale{}
	, m_time(0.0f)
	, m_state(IDLE)
	, m_popped(false)
{
	m_windowSize = { width, height };
	// 位置を初期化
	m_position = { width / 2.0f + 150.0f, height - 75.0f};

	// アニメーションパラメータを読み込む
	Resources::GetInstance()->ReadJsonFile(&m_popAnimation, L"Resources/Datas/HowToPlayAnimation");

	// スケールを取得
	DirectX::SimpleMath::Vector2 defaultScale;
	for (const auto& scale : m_popAnimation["DEFAULT_SCALE"])
	{
		defaultScale = { scale[0], scale[1] };
	}
	m_defaultScale = defaultScale;

}


//------------------------------------------
// デストラクタ
//------------------------------------------
HowToPlay::~HowToPlay()
{
}


//------------------------------------------
// 更新
//------------------------------------------
void HowToPlay::Update(const float& elapsedTime)
{
}


//------------------------------------------
// 描画
//------------------------------------------
void HowToPlay::Render()
{
	auto* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// タイトルロゴを取得
	Resources::TextureTable howToPlay = Resources::GetInstance()->GetTexture(L"HowToPlay");

	// スプライトバッチ描画開始
	spriteBatch->Begin();

	// タイトル描画
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
// ポップアニメーション
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
// ポップアウト
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
// ポップイン
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
