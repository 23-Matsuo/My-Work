//--------------------------------------------------------------------------------------
// File: StageIntroUI.cpp
//
// イントロのUI管理クラス
//
// Date: 2023.10.15
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"

#include "StageIntroUI.h"
#include "MyLib/Resources.h"
#include "MyLib/Graphics.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
StageIntroUI::StageIntroUI()
{
	// 画面のサイズを取得する
	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_tutorialTexPos = { static_cast<float>(rect.right) / 2.0f, -50.0f };
	m_introGuidePosition = { -150.0f,  static_cast<float>(rect.bottom) / 2.0f };
}

//------------------------------------------
// デストラクタ
//------------------------------------------
StageIntroUI::~StageIntroUI()
{
}

//------------------------------------------
// 更新
//------------------------------------------
void StageIntroUI::Update(GameCamera* pGameCamera)
{
	//// 線形補間でUIのアニメーション
	//if (pGameCamera->GetEyePosition().z <= pStage->GetStageFloorManager()->GetGoalPositonZ() + 80.0f)
	//{
	//	// 戻る
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
	//	// 出現
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
// 描画
//------------------------------------------
void StageIntroUI::Render()
{
	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();

	Resources::TextureTable stageNumBack = Resources::GetInstance()->GetTexture(L"StageNumBack");
	Resources::TextureTable stageNum	 = Resources::GetInstance()->GetTexture(L"StageNum");
	Resources::TextureTable introGuide	 = Resources::GetInstance()->GetTexture(L"IntroGuide");

	RECT src;
	// 切り取り位置
	src = { 0, 0, stageNumBack.textureWidth, stageNumBack.textureHeight };

	spriteBatch->Begin();
	// ステージ情報画像描画
	spriteBatch->Draw(
		stageNumBack.texture.Get(),
		m_tutorialTexPos,
		&src,
		DirectX::Colors::White,
		0.0f,
		DirectX::SimpleMath::Vector2(static_cast<float>(src.right) / 2.0f, static_cast<float>(src.bottom) / 2.0f),
		1.0f
	);
	// 切り取り位置
	src = { 0, 0, stageNum.textureWidth, stageNum.textureHeight };
	// ステージ情報画像描画
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
	// チュートリアル画像描画
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