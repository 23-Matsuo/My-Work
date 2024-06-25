//--------------------------------------------------------------------------------------
// File: TitleScene.h
//
// タイトルシーンクラス
//
// Date: 2023.7.21
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#include "Model.h"
#include <SpriteBatch.h>

#include "MyLib/DebugCamera.h"
#include "MyLib/SceneManager.h"
#include "MyLib/Transition.h"
#include "MyLib/SoftBody/SoftBodyBox.h"
#include "MyLib/SoundUtility.h"
#include "TitleSceneObjects/StartMenu.h"
#include "TitleSceneObjects/TitleLogo.h"
#include "TitleSceneObjects/TitleBackGround.h"
#include "TitleSceneObjects/PushSpace.h"
#include "TitleSceneObjects/MainMenuText.h"
#include "TitleSceneObjects/HowToPlay.h"
#include "TitleSceneObjects/HighScore.h"

class TitleScene : public Scene
{
public:
	// シーンの状態
	enum SceneState
	{
		Title,
		Select
	};

public:
	
	// コンストラクタ
	TitleScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Render() override;

	// 終了処理
	void Finalize() override;

	// デバイスに依存するリソースを作成する関数
	void CreateDeviceDependentResources() override;

	// ウインドウサイズに依存するリソースを作成する関数
	void CreateWindowSizeDependentResources() override;

	// デバイスロストした時に呼び出される関数
	void OnDeviceLost() override;

public:

	void SetState(const TitleScene::SceneState& state) { m_sceneState = state; }

	const TitleScene::SceneState& GetState() { return m_sceneState; }


private:

	// メイン処理
	void MainUpdate();

	// ボタンを押したときの処理
	void PressedButton();

	// ロゴシーンの更新
	void LogoScene(const float& elapsedTime, DirectX::Keyboard::KeyboardStateTracker* keyState);

	// セレクトシーンの更新
	void SelectScene(const float& elapsedTime, DirectX::Keyboard::KeyboardStateTracker* keyState);

private:

	// サウンドユーティリティへのポインタ
	std::unique_ptr<SoundUtility> m_soundUtility;
	// bgmへのポインタ
	DirectX::SoundEffectInstance* m_bgmInstance;
	// ボタン音へのポインタ
	DirectX::SoundEffectInstance* m_pushButtonInstance;

	// メニューへのポインタ
	std::unique_ptr<StartMenu> m_menu;

	// フェードへのポインタ
	std::unique_ptr<Transition> m_fade;

	// タイトルのロゴ
	std::unique_ptr<TitleLogo> m_titleLogo;

	// 背景
	std::unique_ptr<TitleBack> m_backGround;

	// 「スペースおしてスタート」オブジェクト
	std::unique_ptr<PushSpace> m_pushSpace;

	// 「メインメニュー」オブジェクト
	std::unique_ptr<MainMenuText> m_mainText;

	// 遊び方
	std::unique_ptr<HowToPlay> m_howToPlay;

	// 最高スコア
	std::unique_ptr<HighScore> m_maxScore;

private:

	// シーンの状態
	SceneState m_sceneState;

	// ロゴアニメーションフラグ
	bool m_logoAnimated;

	// スタートフラグ
	bool m_isStarted;

	// ステージ選択フラグ
	bool m_isSelected;

	// 終了フラグ
	bool m_isQuit;

	// タイトルのシーンチェンジフラグ
	bool m_changed;

	// ポップした
	bool m_howToPlayPoppedIn;
	bool m_howToPlayPoppedOut;
};

