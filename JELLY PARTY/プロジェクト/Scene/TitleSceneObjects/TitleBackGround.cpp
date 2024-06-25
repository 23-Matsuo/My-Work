//--------------------------------------------------------------------------------------
// File: TitleBackGround.cpp
//
// �^�C�g���w�i�N���X
//
// Date: 2023.12.02
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "TitleBackGround.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
TitleBack::TitleBack(const float& width, const float& height)
	:
	m_scale(1.0f, 1.0f)
{
	m_windowSize = { width, height };
	// �ʒu��������
	m_position = { 0.0f, 0.0f };
}


//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
TitleBack::~TitleBack()
{
}


//------------------------------------------
// �X�V
//------------------------------------------
bool TitleBack::Update(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
	return false;
}


//------------------------------------------
// �`��
//------------------------------------------
void TitleBack::Render()
{
	auto* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	// �^�C�g�����S���擾
	Resources::TextureTable backGroundTexture = Resources::GetInstance()->GetTexture(L"BackGround");

	// �X�v���C�g�o�b�`�`��J�n
	spriteBatch->Begin();

	spriteBatch->Draw(
		backGroundTexture.texture.Get(),
		m_position
	);

	spriteBatch->End();

}
