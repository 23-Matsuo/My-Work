//--------------------------------------------------------------------------------------
// File: Loading.h
//
// ローディング中のスプライト
//
// Date: 2023.11.03
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef LOADING_DEFINED
#define LOADING_DEFINED

#include "MyLib/Resources.h"

class Loading
{
public:

	// 1つの画像の大きさ
	int SPRITE_WIDTH;
	int SPRITE_HEIGHT;
	// スプライトシートの列数
	const int SPRITE_COLUMN = 10;
	// スプライトシートの行数
	const int SPRITE_ROW = 3;

	const float SCALE = 1.5f;

public:
	// コンテキスト
	Loading();
	// デストラクタ
	~Loading();

	// 更新
	void Update();
	// 描画
	void Render();

private:
	// フレームをカウント
	DirectX::SimpleMath::Vector2 m_frameCount;

	// スプライトデータ
	Resources::TextureTable m_loadingSprite;

	// 位置
	DirectX::SimpleMath::Vector2 m_position;
};

#endif // LOADING_DEFINED