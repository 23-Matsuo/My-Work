//--------------------------------------------------------------------------------------
// File: LoadScene.h
//
// ロードシーンクラス
//
// Usage: シーン遷移の間にこのシーンをはさむ
// 
// Date: 2023.7.21
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#include "MyLib/SceneManager.h"

#include "LoadSceneObjects/Loading.h"
class LoadScene : public LoadingScreen
{
public:
	
	// コンストラクタ
	LoadScene();

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
	std::unique_ptr<Loading> m_loading;

};