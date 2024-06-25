//--------------------------------------------------------------------------------------
// File: StageIntroUI.h
//
// �C���g����UI�Ǘ��N���X
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
	// �X�e�[�W���摜�̈ʒu
	DirectX::SimpleMath::Vector2 m_tutorialTexPos;

	// �C���g���̑�������摜�̈ʒu
	DirectX::SimpleMath::Vector2 m_introGuidePosition;
};

#endif // STAGE_INTRO_UI_DEFINED