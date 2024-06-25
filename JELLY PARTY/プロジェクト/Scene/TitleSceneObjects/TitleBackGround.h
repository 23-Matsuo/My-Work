//--------------------------------------------------------------------------------------
// File: TitleBackGround.h
//
// タイトル背景クラス
//
// Date: 2023/12/02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef TITLE_BACKGROUND_DIFINED
#define TITLE_BACKGROUND_DIFINED

class TitleBack
{

public:
	// コンストラクタ
	TitleBack(const float& width, const float& height);

	// デストラクタ
	~TitleBack();

	// 更新
	bool Update(const float& elapsedTime);

	// 描画
	void Render(); 

private:
	DirectX::SimpleMath::Vector2 m_windowSize;

	DirectX::SimpleMath::Vector2 m_position;

	DirectX::SimpleMath::Vector2 m_scale;
};

#endif // TITLE_BACKGROUND_DIFINED
