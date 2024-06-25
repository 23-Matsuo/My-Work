//--------------------------------------------------------------------------------------
// File: TitleBackGround.h
//
// �^�C�g���w�i�N���X
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
	// �R���X�g���N�^
	TitleBack(const float& width, const float& height);

	// �f�X�g���N�^
	~TitleBack();

	// �X�V
	bool Update(const float& elapsedTime);

	// �`��
	void Render(); 

private:
	DirectX::SimpleMath::Vector2 m_windowSize;

	DirectX::SimpleMath::Vector2 m_position;

	DirectX::SimpleMath::Vector2 m_scale;
};

#endif // TITLE_BACKGROUND_DIFINED
