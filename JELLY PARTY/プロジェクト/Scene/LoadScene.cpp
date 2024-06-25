//--------------------------------------------------------------------------------------
// File: LoadScene.cpp
//
// ロードシーンクラス
//
// Date: 2023.7.21
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "LoadScene.h"
#include "PlayScene.h"
#include "MyLib/Resources.h"
#include "MyLib/Graphics.h"

using namespace DirectX;

//------------------------------------------
// コンストラクタ
//------------------------------------------
LoadScene::LoadScene()
{
}

//------------------------------------------
// 初期化
//------------------------------------------
void LoadScene::Initialize()
{

	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	m_loading = std::make_unique<Loading>();
}

//------------------------------------------
/// 更新
//------------------------------------------
void LoadScene::Update()
{
	m_loading->Update();
}

//------------------------------------------
/// 描画
//------------------------------------------
void LoadScene::Render()
{
	m_loading->Render();
}

//------------------------------------------
/// 終了処理
//------------------------------------------
void LoadScene::Finalize()
{
	m_loading.reset();
}

//------------------------------------------
/// デバイスに依存するリソースを作成する関数
//------------------------------------------
void LoadScene::CreateDeviceDependentResources()
{

}

//------------------------------------------
/// ウインドウサイズに依存するリソースを作成する関数
//------------------------------------------
void LoadScene::CreateWindowSizeDependentResources()
{
}

//------------------------------------------
/// デバイスロストした時に呼び出される関数
//------------------------------------------
void LoadScene::OnDeviceLost()
{
	Finalize();
}
