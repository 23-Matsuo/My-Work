//--------------------------------------------------------------------------------------
// File: GameTimer.cpp
//
// ゲームのタイマークラス
//
// Date: 2023/11/01
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "GameTimer.h"
#include "MyLib/Graphics.h"
#include "MyLib/Resources.h"


// 残り時間の始まりの値（秒）
const int GameTimer::TOTAL_SECONDS = 90;
// スプライトの列数
const float GameTimer::SPRITE_COLUMN = 10;
// スプライトの行数
const float GameTimer::SPRITE_ROW = 2;

//------------------------------------------
// コンストラクタ
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
// デストラクタ
//------------------------------------------
GameTimer::~GameTimer()
{

}

//------------------------------------------
// 更新
//------------------------------------------
bool GameTimer::Update()
{
	// 経過時間を計測
	m_spendingTime++;

	// 経過時間を秒に直す
	int second = m_spendingTime / 60;

	// 残り時間を計算
	m_remainingTime = TOTAL_SECONDS - second;

	// 残り時間が0になったらゲームオーバー
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
// 描画
//------------------------------------------
void GameTimer::Render()
{
	if (m_remainingTime <= 0) return;

	Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"NumberSprite");
	RECT windowSize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	// 1マス分の画像サイズ
	LONG spriteWidth  = static_cast<LONG>(numberTexture.textureWidth / SPRITE_COLUMN);
	LONG spriteHeight = static_cast<LONG>(numberTexture.textureHeight / SPRITE_ROW);


	int time[4];
	// 秒
	time[0] = (m_remainingTime % 60) % 10;
	time[1] = (m_remainingTime % 60) / 10;
	// 分
	time[2] = (m_remainingTime / 60) % 10;
	time[3] = (m_remainingTime / 60) / 10;
	// 切り取り位置
	RECT src0 = { 0, spriteHeight, spriteWidth, spriteHeight * 2 };
	RECT src1 = { spriteWidth * time[0], 0, spriteWidth * (time[0] + 1), spriteHeight };
	RECT src2 = { spriteWidth * time[1], 0, spriteWidth * (time[1] + 1), spriteHeight };
	RECT src3 = { spriteWidth * time[2], 0, spriteWidth * (time[2] + 1), spriteHeight };
	RECT src4 = { spriteWidth * time[3], 0, spriteWidth * (time[3] + 1), spriteHeight };

	// 基準位置
	DirectX::SimpleMath::Vector2 basePosition = { static_cast<float>(windowSize.right) / 10.0f, 30.0f };

	// 中心位置
	DirectX::SimpleMath::Vector2 pos = basePosition;
	RenderTimer(pos, src0);
	// 秒
	pos.x += spriteWidth * m_scale;
	RenderTimer(pos, src2);
	pos.x += spriteWidth * m_scale;
	RenderTimer(pos, src1);
	// 分
	pos = basePosition;
	pos.x -= spriteWidth * m_scale;
	RenderTimer(pos, src3);
	pos.x -= spriteWidth * m_scale;
	RenderTimer(pos, src4);
}

//------------------------------------------
// タイマーを描画
//------------------------------------------
void GameTimer::RenderTimer(const DirectX::SimpleMath::Vector2& position, const RECT& cutPos)
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	Resources::TextureTable numberTexture = Resources::GetInstance()->GetTexture(L"NumberSprite");
	// 1マス分の画像サイズ
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
