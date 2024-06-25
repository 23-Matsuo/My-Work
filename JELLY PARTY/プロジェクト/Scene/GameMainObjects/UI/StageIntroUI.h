//--------------------------------------------------------------------------------------
// File: StageIntroUI.h
//
// イントロのUI管理クラス
//
// Date: 2023.10.15
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#pragma once
#ifndef STAGE_INTRO_UI_DEFINED
#define STAGE_INTRO_UI_DEFINED

#include "../GameCamera.h"

class StageIntroUI
{
public:
	StageIntroUI();
	~StageIntroUI();

	void Update(GameCamera* pGameCamera);
	void Render();

private:
	// ステージ情報画像の位置
	DirectX::SimpleMath::Vector2 m_tutorialTexPos;

	// イントロの操作説明画像の位置
	DirectX::SimpleMath::Vector2 m_introGuidePosition;
};

#endif // STAGE_INTRO_UI_DEFINED