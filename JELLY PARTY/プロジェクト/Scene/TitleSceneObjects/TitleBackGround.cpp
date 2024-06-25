//--------------------------------------------------------------------------------------
// File: TitleBackGround.cpp
//
// タイトル背景クラス
//
// Date: 2023.12.02
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "TitleBackGround.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
TitleBack::TitleBack(const float& width, const float& height)
	:
	m_scale(1.0f, 1.0f)
{
	m_windowSize = { width, height };
	// 位置を初期化
	m_position = { 0.0f, 0.0f };
}


//------------------------------------------
// デストラクタ
//------------------------------------------
TitleBack::~TitleBack()
{
}


//------------------------------------------
// 更新
//------------------------------------------
bool TitleBack::Update(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	return false;
}


//------------------------------------------
// 描画
//------------------------------------------
void TitleBack::Render()
{
	auto* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// タイトルロゴを取得
	Resources::TextureTable backGroundTexture = Resources::GetInstance()->GetTexture(L"BackGround");

	// スプライトバッチ描画開始
	spriteBatch->Begin();

	spriteBatch->Draw(
		backGroundTexture.texture.Get(),
		m_position
	);

	spriteBatch->End();

}
