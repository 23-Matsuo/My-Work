//--------------------------------------------------------------------------------------
// File: PlayScene.h
//
// ゲームのメインシーンクラス
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#include "Model.h"

#include "ImaseLib/GridFloor.h"
#include "ImaseLib/ObjCollision.h"
#include "ImaseLib/DisplayCollision.h"
#include "MyLib/SceneManager.h"
#include "MyLib/Transition.h"
#include "MyLib/SoundUtility.h"
#include "MyLib/ShadowMap.h"
#include "GameMainObjects/Managers/StageSoftBodyManager.h"
#include "GameMainObjects/Managers/StageFloorManager.h"
#include "GameMainObjects/GameCamera.h"
#include "GameMainObjects/UI/ClearMenu.h"
#include "GameMainObjects/UI/CountDown.h"
#include "GameMainObjects/UI/DoWhat.h"
#include "GameMainObjects/UI/Finish.h"
#include "GameMainObjects/UI/Result.h"
#include "GameMainObjects/UI/GameTimer.h"
#include "GameMainObjects/UI/Score.h"
#include "Effects/HitParticle.h"

class PlayScene : public Scene
{

public:
	enum class SceneState
	{
		COUNTDOWN,	// カウントダウン
		STARTED,	// スタート後
		RESULT,		// リザルト
	};
public:
	
	// コンストラクタ
	PlayScene();

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

private:

	// ゲームメインの更新
	void MainUpdate();

	// ゴール後の処理
	void ResultUpdate(DirectX::Keyboard::KeyboardStateTracker* keyState);

	// デバッグカメラ更新
	void DebugUpdate(DirectX::Keyboard::KeyboardStateTracker* key);

private:

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// グリッド床へのポインタ
	std::unique_ptr<Imase::GridFloor> m_gridFloor;

private:

	// カメラへのポインタ
	std::unique_ptr<GameCamera> m_gameCamera;
	// フェードへのポインタ
	std::unique_ptr<Transition> m_fade;
	// プレイヤーへのポインタ
	std::unique_ptr<CatSphere> m_catBall;

	// クリア時のメニューへのポインタ
	std::unique_ptr<ClearMenu> m_clearMenu;
	// カウントダウンへのポインタ
	std::unique_ptr<CountDown> m_countDown;
	// カウントダウンが終わった時の文字オブジェクト
	std::unique_ptr<DoWhat> m_doWhat;
	// 終了時の文字オブジェクト
	std::unique_ptr<Finish> m_finish;
	// 「リザルト」オブジェクト
	std::unique_ptr<Result> m_result;
	// タイマーへのポインタ
	std::unique_ptr<GameTimer> m_gameTimer;
	// スコアへのポインタ
	std::unique_ptr<Score> m_score;
	// ソフトボディ
	std::unique_ptr<StageSoftBodyManager> m_softBodyManager;
	// 床
	std::unique_ptr<StageFloorManager> m_stageFloor;
	// シャドウマップ
	std::unique_ptr<ShadowMap> m_shadowMap;

private:

	// サウンドユーティリティへのポインタ
	std::unique_ptr<SoundUtility> m_soundUtility;
	// bgmへのポインタ
	DirectX::SoundEffectInstance* m_gameBGM;
	// ゴールbgmへのポインタ
	DirectX::SoundEffectInstance* m_resultBGM;
	// ボタン音へのポインタ
	DirectX::SoundEffectInstance* m_pushButtonSound;
	// 開始音へのポインタ
	DirectX::SoundEffectInstance* m_startSound;
	// 終了音へのポインタ
	DirectX::SoundEffectInstance* m_finishSound;

private:

	// シーンの状態
	SceneState m_sceneState;

	// フラグ
	bool m_isTimeOver;
	bool m_playedFinishedSound;

	// リスタートフラグ
	bool m_reStart;
	// メニューに戻るフラグ
	bool m_backMenu;

	// リザルトの時間
	float m_resultTime;
};