//--------------------------------------------------------------------------------------
// File: Loading.cpp
//
// ローディング中のスプライト
//
// Date: 2023.11.03
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Loading.h"
#include "MyLib/Graphics.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
Loading::Loading()
	:
	m_position(920.0f , 500.0f)
{
	m_loadingSprite = Resources::GetInstance()->GetTexture(L"LoadingSprite");
	SPRITE_WIDTH = m_loadingSprite.textureWidth / SPRITE_COLUMN;
	SPRITE_HEIGHT = m_loadingSprite.textureHeight / SPRITE_ROW;

}

//------------------------------------------
// デストラクタ
//------------------------------------------
Loading::~Loading()
{
}

//------------------------------------------
// 更新
//------------------------------------------
void Loading::Update()
{
	// 描画速度をカウント
	m_frameCount.x += 1.0f / 3.0f;
	// Spriteシートの右端に行ったら
	if (m_frameCount.x >= SPRITE_COLUMN)
	{
		// xを初期化
		m_frameCount.x = 1.0f;
		// yをカウント
		m_frameCount.y += 1.0f;
	}

	// SpriteSheetの一番下に行ったら
	if (m_frameCount.y >= SPRITE_ROW)
	{
		// 初期化
		m_frameCount.x = 1.0f;
		m_frameCount.y = 0.0f;
	}

}

//------------------------------------------
// 描画
//------------------------------------------
void Loading::Render()
{
	auto state = Graphics::GetInstance()->GetCommonStates();
	auto spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// 切り取り開始位置
	int startCut = static_cast<int>(m_frameCount.x - 1.0f);
	// 切り取り終了位置
	int endCut = static_cast<int>(m_frameCount.x);
	// 切り取り位置計算
	RECT src = {
		SPRITE_WIDTH * startCut,
		SPRITE_HEIGHT * static_cast<int>(m_frameCount.y),
		SPRITE_WIDTH * endCut,
		SPRITE_HEIGHT * static_cast<int>((m_frameCount.y + 1.0f))
	};
	// スプライトバッチ描画開始
	spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, state->NonPremultiplied());

	// ロード画像描画
	spriteBatch->Draw(
		m_loadingSprite.texture.Get(),				// テクスチャ
		m_position,									// 描画座標
		&src,										// 切り取り座標
		DirectX::Colors::Yellow,					// 色
		0.0f,										// 回転
		DirectX::SimpleMath::Vector2::Zero,			// 原点
		SCALE										// スケール
	);

	spriteBatch->End();

}
