//--------------------------------------------------------------------------------------
// File: HighScoreText.cpp
//
// 「ハイスコア！」クラス
//
// Date: 2023/01/04
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "HighScoreText.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
HighScoreText::HighScoreText(const float& width, const float& height)
	:
	m_defaultScale(0.8f)
	, m_windowSize(width, height)
	, m_alpha(0.0f)
	, m_timer(0.0f)
{
	// 初期位置
	m_position = { m_windowSize.x / 1.5f, m_windowSize.y / 3.0f };

	// スケール
	m_scale = m_defaultScale;
}

//------------------------------------------
// デストラクタ
//------------------------------------------
HighScoreText::~HighScoreText()
{

}

//------------------------------------------
// 更新
//------------------------------------------
bool HighScoreText::Update()
{
	if (m_timer >= 1.5f) return true;
	
	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	m_timer += elapsedTime;

	// スケール変化
	if (m_scale >= 0.45f)
	{
		m_scale -= 0.05f;
	}
	// 透明度変化
	if (m_alpha <= 1.0f)
	{
		m_alpha += 0.2f;
	}

	return false;
}

//------------------------------------------
// 描画
//------------------------------------------
void HighScoreText::Render()
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();
	Resources::TextureTable highScoreTexture = Resources::GetInstance()->GetTexture(L"HighScore");


	// 1マス分の画像サイズ
	LONG spriteWidth = static_cast<LONG>(highScoreTexture.textureWidth);
	LONG spriteHeight = static_cast<LONG>(highScoreTexture.textureHeight);

	// 画像の切り取り位置
	RECT src = { 0, 0, spriteWidth, spriteHeight };

	// 描画
	spriteBatch->Begin();

	spriteBatch->Draw(
		highScoreTexture.texture.Get(),
		m_position,
		&src,
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, m_alpha),
		0.2f,
		DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, spriteHeight),
		m_scale
	);

	spriteBatch->End();
}