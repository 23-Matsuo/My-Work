//--------------------------------------------------------------------------------------
// File: PlayScene.cpp
//
// ゲームのメインシーンクラス
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "PlayScene.h"
#include "TitleScene.h"
#include "LoadScene.h"
#include "Resources/Sounds/SoundIdx.h"
#include "GameManager.h"

using namespace DirectX;

//------------------------------------------
// コンストラクタ
//------------------------------------------
PlayScene::PlayScene()
	:
	m_sceneState(SceneState::COUNTDOWN)
	, m_gameBGM(nullptr)
	, m_resultBGM(nullptr)
	, m_pushButtonSound(nullptr)
	, m_finishSound(nullptr)
	, m_startSound(nullptr)
	, m_reStart(false)
	, m_backMenu(false)
	, m_isTimeOver(false)
	, m_playedFinishedSound(false)
	, m_resultTime(0.0f)
{
}

//------------------------------------------
// 初期化
//------------------------------------------
void PlayScene::Initialize()
{
	m_isTimeOver = false;
	m_playedFinishedSound = false;

	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// 球の作成
	m_catBall = std::make_unique<CatSphere>();
	m_catBall->Intialize();

	// カウントダウンの作成
	m_countDown = std::make_unique<CountDown>();

	// 終了時の文字の作成
	m_finish = std::make_unique<Finish>();

	// ゲームタイマーの作成
	m_gameTimer = std::make_unique<GameTimer>();

	m_shadowMap = std::make_unique<ShadowMap>();
	m_shadowMap->Initialize();

	// サウンドユーティリティ作成
	m_soundUtility = std::make_unique<SoundUtility>();

	// BGM作成
	m_gameBGM = Resources::GetInstance()->GetSound(L"PlayBGM");
	m_resultBGM = Resources::GetInstance()->GetSound(L"ResultBGM");
	// ボタン音作成
	m_pushButtonSound = Resources::GetInstance()->GetSound(L"Button");
	// 終了音作成
	m_finishSound = Resources::GetInstance()->GetSound(L"FinishGong");
	// 開始音作成
	m_startSound = Resources::GetInstance()->GetSound(L"Finish");

	// BGMを鳴らす
	PlaySound(m_gameBGM, true);

}

//------------------------------------------
// 更新
//------------------------------------------
void PlayScene::Update()
{
	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	// キーボード取得
	auto key = Graphics::GetInstance()->GetKeyboardStateTracker();

	auto keyboard = key->GetLastState();

	// トランジションの更新
	m_fade->Update();

	// BGMの音量更新
	m_soundUtility->Update(m_gameBGM);

	// デバッグ情報の更新
	DebugUpdate(key);

	switch (m_sceneState)
	{
	case SceneState::COUNTDOWN:			// カウントダウン中
	{
		// フェードが終わっていなければ処理しない
		if (m_fade->GetState() != Transition::STATE::IDLE) break;

		// カウントダウンが0になったら
		if (m_countDown->Update())
		{
			// スタート音
			m_startSound->Play(false);
			// ゲームスタート
			m_sceneState = SceneState::STARTED;
			m_softBodyManager->SetSceneState(StageSoftBodyManager::SceneState::MAIN);
		}
		break;
	}
	case SceneState::STARTED:
	{

		m_doWhat->Update();

		if (!m_catBall->GetIsDead())
		{
			// 時間更新
			if (m_gameTimer->Update())
			{
				m_isTimeOver = true;
			}
		}

		if (m_catBall->GetPosition().y < -1.0f)
		{
			m_gameCamera->SetType(GameCamera::Type::DEAD);
		}

		// 球が死んだ処理
		if (m_catBall->GetIsDead() || m_isTimeOver)
		{

			m_soundUtility->SetFadeVolume(0.8f);
			m_soundUtility->FadeOut(m_gameBGM);

			if (!m_playedFinishedSound)
			{
				PlaySound(m_finishSound, false);
				m_playedFinishedSound = true;
			}
			// 「フィニッシュ」文字更新
			if (m_finish->Update())
			{
				// BGMを止める
				m_gameBGM->Stop();

				// 球の初期化
				m_catBall->Intialize();
				// 球のリザルト時の状態を設定
				m_catBall->SetResultState(CatSphere::ResultState::LOOKUP);
				// シーンの状態を変更
				m_sceneState = SceneState::RESULT;
				// カメラを変更
				m_gameCamera->SetType(GameCamera::Type::RESULT);
				// ゼリーの数表示オブジェクトの状態を変更
				m_score->SetSceneState(Score::SceneState::RESULT);
				// ステージ上のソフトボディの状態を変更
				m_softBodyManager->SetSceneState(StageSoftBodyManager::SceneState::RESULT);

				// 取得した量を記録
				GameManager::GetInstance()->SetNowScore(m_score->GetScore());

				// リザルトの初期化
				m_softBodyManager->CubeResultInitialize(m_score.get());

				// カウントを0に
				m_score->Initialize();


				// ポップイン状態に変更
				m_clearMenu->SetState(ClearMenu::STATE::POPIN);

				// リザルトの時間を初期化
				m_resultTime = 0.0f;

				// リザルトのBGNを鳴らす
				PlaySound(m_resultBGM, true);

			}
		}
		else
		{
			// ゲームメインの更新
			MainUpdate();
		}
		
		break;
	}
	case SceneState::RESULT:
	{
		// 「リザルト！」更新
		if (!m_result->Update()) break;

		// リザルトのアニメーションが終わっていたら
		// キャラのリザルト処理
		m_catBall->ResultUpdate();

		// ソフトボディのリザルト処理
		m_softBodyManager->Update(0);

		// カウントが終わったら
		if (m_softBodyManager->GetCountFinished())
		{
			// リザルトの経過時間を計算
			m_resultTime += elapsedTime;

			// リザルト状態
			m_catBall->SetResultState(CatSphere::ResultState::CLAP);

			// リザルトのメニュー処理
			ResultUpdate(key);
		}
		break;
	}
	default:
		break;
	}

	// スコアの更新
	m_score->Update();

	// カメラに球の位置を設定
	m_gameCamera->SetPlayerPosition(m_catBall->GetPosition());

	// カメラの更新
	m_gameCamera->Update(elapsedTime);

}

//------------------------------------------
// 描画
//------------------------------------------
void PlayScene::Render()
{
	// ビュー行列の作成
	m_view = m_gameCamera->GetView();

	//// グリッド床の描画
	//m_gridFloor->Render(context, m_view, m_projection);

	m_shadowMap->Render(m_catBall.get(), m_stageFloor.get(), m_softBodyManager.get());

	switch (m_sceneState)
	{
	case SceneState::COUNTDOWN:		// カウントダウン中
	{
		// カウントダウン描画
		m_countDown->Render();

		// ゲーム内時間描画
		m_gameTimer->Render();

		break;
	}
	case SceneState::STARTED:		// スタートした
	{
		// 「あつめろ！」描画
		m_doWhat->Render();

		// ゲーム内時間描画
		m_gameTimer->Render();

		// キャラが落ちたもしくは時間が終わったら
		if (m_catBall->GetIsDead() || m_isTimeOver)
		{
			// 「フィニッシュ!」描画
			m_finish->Render();
		}
		break;
	}
	case SceneState::RESULT:
	{
		// リザルト描画
		m_result->Render();
		// メニュー描画
		m_clearMenu->Render();

		break;
	}
	default:
		break;
	}

	// スコア描画
	m_score->Render();

	// フェード描画
	m_fade->Render();

}

//------------------------------------------
// 終了処理
//------------------------------------------
void PlayScene::Finalize()
{
	m_startSound->Stop();
	m_finishSound->Stop();

	m_gameBGM = nullptr;
	m_resultBGM = nullptr;
	m_pushButtonSound = nullptr;
	m_finishSound = nullptr;
	m_startSound = nullptr;
}

//------------------------------------------
// デバイスに依存するリソースを作成する関数
//------------------------------------------
void PlayScene::CreateDeviceDependentResources()
{
	// ソフトボディマネージャー作成
	m_softBodyManager = std::make_unique<StageSoftBodyManager>();
	// 床作成
	m_stageFloor = std::make_unique<StageFloorManager>(Graphics::GetInstance()->GetDeviceResources());

	// トランジション作成
	m_fade = std::make_unique<Transition>();
	// トランジションを初期化
	m_fade->Initialize();
}

//------------------------------------------
// ウインドウサイズに依存するリソースを作成する関数
//------------------------------------------
void PlayScene::CreateWindowSizeDependentResources()
{
	// 画面のサイズを取得する
	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	// カメラの作成
	m_gameCamera = std::make_unique<GameCamera>(rect);
	// カメラの位置を初期化
	SimpleMath::Vector3 eyePos = SimpleMath::Vector3::Zero;
	// カメラの視点を初期化
	SimpleMath::Vector3 targetPos = SimpleMath::Vector3::Zero;
	// カメラに設定
	m_gameCamera->SetPositionTarget(eyePos, targetPos);

	// 射影行列の作成
	m_projection = m_gameCamera->GetProjection();

	// メニューの作成
	m_clearMenu = std::make_unique<ClearMenu>(rect.right, rect.bottom);

	// カウントダウン終了後の文字の作成
	m_doWhat = std::make_unique<DoWhat>(rect.right, rect.bottom);

	// スコアの作成
	m_score = std::make_unique<Score>(rect.right, rect.bottom);

	// 「リザルト」の作成
	m_result = std::make_unique<Result>(rect.right, rect.bottom);
}

//------------------------------------------
// デバイスロストした時に呼び出される関数
//------------------------------------------
void PlayScene::OnDeviceLost()
{
	Finalize();
}

//------------------------------------------
// ゲームメインの更新
//------------------------------------------
void PlayScene::MainUpdate()
{
	auto timer = Graphics::GetInstance()->GetStepTimer();

	// ソフトボディを更新
	m_softBodyManager->Update(m_gameTimer->GetSpendingTime());

	// 床を更新
	m_stageFloor->Update();

	// 球の更新
	m_catBall->Update(*timer);
}

//------------------------------------------
// リザルトのメニュー処理
//------------------------------------------
void PlayScene::ResultUpdate(DirectX::Keyboard::KeyboardStateTracker* keyState)
{

	if (m_clearMenu->GetState() == StartMenu::STATE::IDLE)
	{
		// メニュー更新
		m_clearMenu->Update();

		// SpaceかEを押したら
		if (keyState->pressed.Space || keyState->pressed.E || keyState->pressed.Enter || m_resultTime > 60.0f)
		{

			// SlideInさせる
			m_fade->SetState(Transition::STATE::SLIDEIN);
			// BGMフェードアウト
			m_soundUtility->SetState(SoundUtility::State::FADEOUT);
			// スペースを押したらポップアウト
			m_clearMenu->SetState(ClearMenu::STATE::POPOUT);

			if (m_clearMenu->GetMenuManager()->GetMenuIndex() == m_clearMenu->MENU || m_resultTime > 60.0f)
			{
				// メニューにもどる
				if (!m_backMenu)
				{
					m_backMenu = true;
					// 音を鳴らす
					PlaySound(m_pushButtonSound, false);
				}
			}
			else if (m_clearMenu->GetMenuManager()->GetMenuIndex() == m_clearMenu->RESTART)
			{
				// リスタート
				if (!m_reStart)
				{
					m_reStart = true;
					// 音を鳴らす
					PlaySound(m_pushButtonSound, false);
				}
			}
		}
	}

	if ((m_reStart || m_backMenu) && m_fade->GetState() == Transition::STATE::IDLE)
	{
		// タイトルを読み込む
		ChangeScene<TitleScene, LoadScene>();

		// BGM終了
		m_resultBGM->Stop();

		// リスタートかどうか渡す
		GameManager::GetInstance()->SetReStarted(m_reStart);
	}
	// ポップアニメーションの更新
	m_clearMenu->Pop();

}

//------------------------------------------
// デバッグの操作
//------------------------------------------
void PlayScene::DebugUpdate(DirectX::Keyboard::KeyboardStateTracker* key)
{
	if (key->pressed.L)
	{
		// デバッグ用カメラに変更
		m_gameCamera->SetType(GameCamera::Type::DEBUG);
	}
	if (key->pressed.K)
	{
		// 固定カメラに変更
		m_gameCamera->SetType(GameCamera::Type::GAMEMAIN);
	}
	if (key->pressed.J)
	{
		// 固定カメラに変更
		m_gameCamera->SetType(GameCamera::Type::RESULT);
	}
}