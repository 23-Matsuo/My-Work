//--------------------------------------------------------------------------------------
// File: CountDown.cpp
//
// カウントダウンクラス
//
// Date: 2023/11/01
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "CountDown.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
CountDown::CountDown()
	:
	m_countDown(180)
	, m_scale(1.0f)
	, m_alpha(0.0f)
	, m_animated(false)
{

}

//------------------------------------------
// デストラクタ
//------------------------------------------
CountDown::~CountDown()
{

}

//------------------------------------------
// 更新
//------------------------------------------
bool CountDown::Update()
{
	if (m_countDown % 60 == 0)
	{
		m_scale = 1.0f;
		m_alpha = 0.0f;
		m_animated = true;
	}

	if (m_animated)
	{
		m_scale -= 0.02f;
		m_alpha += 0.1f;

		if (m_scale <= 0.5f)
		{
			m_scale = 0.5f;
		}

		if (m_alpha >= 1)
		{
			m_animated = false;
		}
	}

	// カウントダウン
	m_countDown--;
	// カウントダウンが0になったらスタート
	if (m_countDown <= -1)
	{
		return true;
	}

	return false;
}

//------------------------------------------
// 描画
//------------------------------------------
void CountDown::Render()
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();
	Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"CountDownSprite");
	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	// 1マス分の画像サイズ
	LONG spriteWidth = static_cast<LONG>(numberTexture.textureWidth / 3);
	LONG spriteHeight = static_cast<LONG>(numberTexture.textureHeight);

	// フレーム数を秒に変換
	int count = m_countDown / 60;


	// 画像の切り取り位置
	RECT src = { spriteWidth * count, 0, spriteWidth * (count + 1), spriteHeight };

	// 中心位置
	DirectX::SimpleMath::Vector2 pos = DirectX::SimpleMath::Vector2(static_cast<float>(rect.right) / 2.0f, static_cast<float>(rect.bottom) / 2.0f);

	// 描画
	spriteBatch->Begin();

	spriteBatch->Draw(
		numberTexture.texture.Get(),
		pos,
		&src,
		DirectX::SimpleMath::Color(1.0f,1.0f,1.0f,m_alpha),
		0.0f,
		DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, spriteHeight / 2.0f),
		m_scale
		);

	spriteBatch->End();
}

