//--------------------------------------------------------------------------------------
// File: StageIntroUI.cpp
//
// �C���g����UI�Ǘ��N���X
//
// Date: 2023.10.15
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"

#include "StageIntroUI.h"
#include "MyLib/Resources.h"
#include "MyLib/Graphics.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
StageIntroUI::StageIntroUI()
{
	// ��ʂ̃T�C�Y���擾����
	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_tutorialTexPos = { static_cast<float>(rect.right) / 2.0f, -50.0f };
	m_introGuidePosition = { -150.0f,  static_cast<float>(rect.bottom) / 2.0f };
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
StageIntroUI::~StageIntroUI()
{
}

//------------------------------------------
// �X�V
//------------------------------------------
void StageIntroUI::Update(GameCamera* pGameCamera)
{
	//// ���`��Ԃ�UI�̃A�j���[�V����
	//if (pGameCamera->GetEyePosition().z <= pStage->GetStageFloorManager()->GetGoalPositonZ() + 80.0f)
	//{
	//	// �߂�
	//	m_tutorialTexPos =
	//		DirectX::SimpleMath::Vector2::Lerp(
	//			m_tutorialTexPos,
	//			{ m_tutorialTexPos.x, -50.0f },
	//			0.075f
	//		);
	//	m_introGuidePosition =
	//		DirectX::SimpleMath::Vector2::Lerp(
	//			m_introGuidePosition,
	//			{ -150.0f, m_introGuidePosition.y },
	//			0.075f
	//		);
	//}
	//else
	//{
	//	// �o��
	//	m_tutorialTexPos =
	//		DirectX::SimpleMath::Vector2::Lerp(
	//			m_tutorialTexPos,
	//			{ m_tutorialTexPos.x, 50.0 },
	//			0.1f
	//		);
	//	m_introGuidePosition =
	//		DirectX::SimpleMath::Vector2::Lerp(
	//			m_introGuidePosition,
	//			{ 150.0f, m_introGuidePosition.y },
	//			0.1f
	//		);
	//}

}

//------------------------------------------
// �`��
//------------------------------------------
void StageIntroUI::Render()
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	Resources::TextureTable stageNumBack = Resources::GetInstance()->GetTexture(L"StageNumBack");
	Resources::TextureTable stageNum	 = Resources::GetInstance()->GetTexture(L"StageNum");
	Resources::TextureTable introGuide	 = Resources::GetInstance()->GetTexture(L"IntroGuide");

	RECT src;
	// �؂���ʒu
	src = { 0, 0, stageNumBack.textureWidth, stageNumBack.textureHeight };

	spriteBatch->Begin();
	// �X�e�[�W���摜�`��
	spriteBatch->Draw(
		stageNumBack.texture.Get(),
		m_tutorialTexPos,
		&src,
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2(static_cast<float>(src.right) / 2.0f, static_cast<float>(src.bottom) / 2.0f),
		1.0f
	);
	// �؂���ʒu
	src = { 0, 0, stageNum.textureWidth, stageNum.textureHeight };
	// �X�e�[�W���摜�`��
	spriteBatch->Draw(
		stageNum.texture.Get(),
		m_tutorialTexPos,
		&src,
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2(static_cast<float>(src.right) / 2.0f, static_cast<float>(src.bottom) / 2.0f),
		1.0f
	);

	src = { 0, 0, introGuide.textureWidth, introGuide.textureHeight };
	// �`���[�g���A���摜�`��
	spriteBatch->Draw(
		introGuide.texture.Get(),
		m_introGuidePosition,
		&src,
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2(static_cast<float>(src.right) / 2.0f, static_cast<float>(src.bottom) / 2.0f),
		1.0f
	);

	spriteBatch->End();

}