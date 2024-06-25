//--------------------------------------------------------------------------------------
// File: TitleScene.cpp
//
// タイトルシーンクラス
//
// Date: 2023.7.21
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "TitleScene.h"
#include "LoadScene.h"
#include "PlayScene.h"

#include "GameManager.h"

using namespace DirectX;

//------------------------------------------
// コンストラクタ
//------------------------------------------
TitleScene::TitleScene()
	:
	m_logoAnimated(false)
	, m_isStarted(false)
	, m_isSelected(false)
	, m_isQuit(false)
	, m_changed(false)
	, m_howToPlayPoppedIn(false)
	, m_howToPlayPoppedOut(false)
	, m_bgmInstance(nullptr)
	, m_pushButtonInstance(nullptr)
	, m_sceneState(GameManager::GetInstance()->GetLastState())
{
	// 最終ステートがセレクトならロゴシーンから始めない
	if (m_sceneState == Select)
	{
		m_changed = true;
	}

	// 最高スコアを設定する
	nlohmann::json data;
	Resources::GetInstance()->ReadJsonFile(&data, L"Resources/Datas/ScoreData");
	GameManager::GetInstance()->SetHighScore(data["MAX_SCORE"]);
}

//------------------------------------------
// 初期化
//------------------------------------------
void TitleScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
	// サウンドユーティリティ作成
	m_soundUtility = std::make_unique<SoundUtility>();

	// サウンドインスタンス作成
	m_bgmInstance = Resources::GetInstance()->GetSound(L"TitleBGM");
	m_pushButtonInstance = Resources::GetInstance()->GetSound(L"Button");
	// BGMを再生
	PlaySound(m_bgmInstance, true);
}

//------------------------------------------
// 更新
//------------------------------------------
void TitleScene::Update()
{
	// リスタートだったらすぐシーン切り替え
	if (GameManager::GetInstance()->GetReStarted()) ChangeScene<PlayScene, LoadScene>();

	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	
	// トランジションの更新
	m_fade->Update();

	// サウンドの更新
	m_soundUtility->Update(m_bgmInstance);

	// ボタンを押していなくてトランジションが待機状態の時の処理
	MainUpdate();

	// メニューボタンを押した時の処理
	PressedButton();

	// ESCを押しているかつトランジションが終わっていたら
	if (m_isQuit && (m_fade->GetState() == Transition::STATE::IDLE))
	{
		m_bgmInstance->Stop();
		// 終了
		PostQuitMessage(0);
	}
}

//------------------------------------------
// 描画
//------------------------------------------
void TitleScene::Render()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	//auto debugFont = Resources::GetInstance()->GetDebugFont();
	//debugFont->AddString(L"TitleScene", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));

	// タイトルの背景を描画
	m_backGround->Render();

	switch (m_sceneState)
	{
	case TitleScene::Title:
		// タイトルのロゴを描画
		m_titleLogo->Render();

		// スペース押して画像を描画
		m_pushSpace->Render();	
		break;
	case TitleScene::Select:
		// メニュー描画
		m_menu->Render();

		// 「メインメニュー」描画
		m_mainText->Render();

		// 遊び方描画
		m_howToPlay->Render();

		if (m_howToPlay->GetScale() == m_howToPlay->GetDefaultScale())
		{
			// 最高スコア描画
			m_maxScore->Render();
		}

		break;
	default:
		break;
	}

	// トランジションを描画
	m_fade->Render();


}

//------------------------------------------
// 終了処理
//------------------------------------------
void TitleScene::Finalize()
{
	m_bgmInstance->Stop();
	m_pushButtonInstance->Stop();
}

//------------------------------------------
// デバイスに依存するリソースを作成する関数
//------------------------------------------
void TitleScene::CreateDeviceDependentResources()
{
}

//------------------------------------------
// ウインドウサイズに依存するリソースを作成する関数
//------------------------------------------
void TitleScene::CreateWindowSizeDependentResources()
{
	// 画面のサイズを取得する
	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	// メニューの作成
	m_menu = std::make_unique<StartMenu>(rect.right, rect.bottom);

	// タイトルロゴの作成
	m_titleLogo = std::make_unique<TitleLogo>(rect.right, rect.bottom);

	// タイトル背景の作成
	m_backGround = std::make_unique<TitleBack>(rect.right, rect.bottom);

	// 最初の案内オブジェクトの作成
	m_pushSpace = std::make_unique<PushSpace>(rect.right, rect.bottom);

	// メニューテキストの作成
	m_mainText = std::make_unique<MainMenuText>(rect.right, rect.bottom);

	// 遊び方の作成
	m_howToPlay = std::make_unique<HowToPlay>(rect.right, rect.bottom);

	// 最高スコアの作成
	m_maxScore = std::make_unique<HighScore>(rect.right, rect.bottom);

	m_fade = std::make_unique<Transition>();
	m_fade->Initialize();

	if (m_sceneState == Select)
	{
		m_menu->SetState(StartMenu::STATE::POPIN);
		m_titleLogo->SetScale(SimpleMath::Vector2::Zero);
		m_pushSpace->SetScale(SimpleMath::Vector2::Zero);
	} 
}

//------------------------------------------
// デバイスロストした時に呼び出される関数
//------------------------------------------
void TitleScene::OnDeviceLost()
{
	Finalize();
}

//------------------------------------------
// メイン処理
//------------------------------------------
void TitleScene::MainUpdate()
{
	// スタートしていて、トランジションが待機状態じゃない
	if (m_isStarted) return;
	if (m_fade->GetState() != Transition::STATE::IDLE) return;

	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	// キーボード取得
	auto keyState = Graphics::GetInstance()->GetKeyboardStateTracker();
	auto key = keyState->GetLastState();

	switch (m_sceneState)
	{
	case TitleScene::Title:
		LogoScene(elapsedTime, keyState);
		break;
	case TitleScene::Select:
		SelectScene(elapsedTime, keyState);
		break;
	default:
		break;
	}
}

//------------------------------------------
// ボタンを押したときの処理
//------------------------------------------
void TitleScene::PressedButton()
{
	// ボタン押しているかつ待機状態なら
	if (!m_isStarted) return;
	if (m_fade->GetState() != Transition::STATE::IDLE) return;
	if (m_menu->GetState() != MenuManager::STATE::IDLE) return;

	switch (m_menu->GetMenuManager()->GetMenuIndex())
	{
	case StartMenu::SelectedButton::PLAY:
		m_bgmInstance->Stop();
		// プレイシーン読み込む
		ChangeScene<PlayScene, LoadScene>();
		break;
	case StartMenu::SelectedButton::QUIT:
		break;
	default:
		break;
	}

}

//------------------------------------------
// ロゴシーンの更新
//------------------------------------------
void TitleScene::LogoScene(const float& elapsedTime, DirectX::Keyboard::KeyboardStateTracker* keyState)
{
	// 上下に移動
	m_pushSpace->Update(elapsedTime);

	// ロゴのアニメーションが終わっているかどうか
	if (!m_titleLogo->Update(elapsedTime)) return;
	else
	{
		if (!m_logoAnimated)
		{
			m_pushSpace->SetState(PushSpace::STATE::POPIN);
			m_logoAnimated = true;
		}
	}

	// アニメーション待機状態
	if (m_titleLogo->GetState() == TitleLogo::STATE::IDLE && m_pushSpace->GetState() == PushSpace::STATE::IDLE)
	{
		if (keyState->pressed.Escape)
		{
			// SlideInさせる
			m_fade->SetState(Transition::STATE::SLIDEIN);
			m_soundUtility->SetState(SoundUtility::State::FADEOUT);

			// ESCを押したら終了処理
			// 終了フラグをtrue
			m_isQuit = true;
		}

		if (keyState->pressed.Space)
		{
			// スペースを押したらポップアウト
			m_titleLogo->SetState(TitleLogo::STATE::POPOUT);
			m_pushSpace->SetState(PushSpace::STATE::POPOUT);

			m_changed = true;
			// 音を鳴らす
			PlaySound(m_pushButtonInstance, false);

		}
	}

	// ポップの更新
	m_titleLogo->Pop(elapsedTime);
	m_pushSpace->Pop(elapsedTime);

	// ポップアウトが終わっていたら変更
	if (m_titleLogo->GetState() == TitleLogo::STATE::IDLE && m_changed)
	{
		m_sceneState = Select;
		m_menu->SetState(StartMenu::STATE::POPIN);
	}
}

//------------------------------------------
// セレクトシーンの更新
//------------------------------------------
void TitleScene::SelectScene(const float& elapsedTime, DirectX::Keyboard::KeyboardStateTracker* keyState)
{
	// ロゴのアニメーションが終わっているかどうか
	m_mainText->Update(elapsedTime);

	if (m_menu->GetState() == StartMenu::STATE::IDLE && m_howToPlay->GetState() == HowToPlay::STATE::IDLE)
	{
		// メニュー更新
		m_menu->Update();

		// 遊び方画像のポップアニメーション制御
		if (m_menu->GetMenuManager()->GetMenuIndex() == StartMenu::SelectedButton::PLAY && !m_howToPlayPoppedIn)
		{
			m_howToPlay->SetState(HowToPlay::STATE::POPIN);
			m_howToPlayPoppedIn = true;
			m_howToPlayPoppedOut = false;
		}
		else if (m_menu->GetMenuManager()->GetMenuIndex() == StartMenu::SelectedButton::QUIT && !m_howToPlayPoppedOut)
		{
			m_howToPlay->SetState(HowToPlay::STATE::POPOUT);
			m_howToPlayPoppedOut = true;
			m_howToPlayPoppedIn = false;
		}

		// SpaceかEを押したら
		if (keyState->pressed.Space || keyState->pressed.E || keyState->pressed.Enter)
		{
			// SlideInさせる
			m_fade->SetState(Transition::STATE::SLIDEIN);
			// BGMフェードアウト
			m_soundUtility->SetState(SoundUtility::State::FADEOUT);
			// 音を鳴らす
			PlaySound(m_pushButtonInstance, false);
			if (m_menu->GetMenuManager()->GetMenuIndex() == StartMenu::SelectedButton::PLAY)
			{
				// ボタン押した
				m_isStarted = true;

				// 最終シーンステートを設定
				GameManager::GetInstance()->SetTitleLastState(m_sceneState);
			}
			else
			{
				m_isQuit = true;
			}
		}

		// ESCを押したらロゴシーンに戻る
		if (keyState->pressed.Escape || keyState->pressed.Back)
		{
			m_menu->SetState(StartMenu::STATE::POPOUT);
			if (m_menu->GetMenuManager()->GetMenuIndex() == StartMenu::SelectedButton::PLAY)
			{
				m_howToPlay->SetState(HowToPlay::STATE::POPOUT);
			}
			m_changed = false;
			m_howToPlayPoppedIn = false;
		}

	}
	// ポップアニメーションの更新
	m_menu->Pop();
	m_howToPlay->Pop(elapsedTime);

	// メニューが待機状態でESCを押していたら
	if (m_menu->GetState() == MenuManager::STATE::IDLE && !m_changed)
	{
		m_sceneState = Title;
		m_titleLogo->SetState(TitleLogo::STATE::POPIN);
		m_pushSpace->SetState(PushSpace::STATE::POPIN);
	}
}
