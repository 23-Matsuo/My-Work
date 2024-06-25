#include "pch.h"
#include "SceneManager.h"

//------------------------------
// 更新
//------------------------------
void SceneManager::Update()
{
//#ifdef ESC_QUIT_ENABLE
//	// ESCキーで終了
//	auto kb = DirectX::Keyboard::Get().GetState();
//	if (kb.Escape) PostQuitMessage(0);
//#endif

	// シーンの切り替え処理
	if (!IsLoading() && m_nextScene)
	{
		DeleteScene();

		assert(m_scene == nullptr);

		// シーンを切り替え
		m_scene = std::move(m_nextScene);

		if (m_loadingScreen)
		{
			// ロード画面の破棄
			m_loadingScreen->Finalize();
			m_loadingScreen.reset();
		}
	}

	// ロード画面の更新
	if (m_loadingScreen)
	{
		m_loadingScreen->Update();
		return;
	}

	// シーンの更新
	if (m_scene) m_scene->Update();

}

//------------------------------
// 描画
//------------------------------
void SceneManager::Render()
{
	// ロード画面の描画
	if (m_loadingScreen)
	{
		m_loadingScreen->Render();
		return;
	}
	// シーンの描画
	if (m_scene) m_scene->Render();
}

//------------------------------------------
// デバイスに依存するリソースを作成する関数
//------------------------------------------
void SceneManager::CreateDeviceDependentResources()
{
	if (m_scene) m_scene->CreateDeviceDependentResources();
}

//------------------------------------------
// ウインドウサイズに依存するリソースを作成する関数
//------------------------------------------
void SceneManager::CreateWindowSizeDependentResources()
{
	if (m_scene) m_scene->CreateWindowSizeDependentResources();
}

//------------------------------------------
// デバイスロストした時に呼び出される関数
//------------------------------------------
void SceneManager::OnDeviceLost()
{
	if (m_scene) m_scene->OnDeviceLost();
}

//------------------------------------------
// ロード中かどうか
//------------------------------------------
inline bool SceneManager::IsLoading()
{
	m_loadingMutex.lock();
	bool isLoading = m_isLoading;
	m_loadingMutex.unlock();

	return isLoading;
}

//------------------------------------------
// シーンの削除
//------------------------------------------
void SceneManager::DeleteScene()
{
	if (m_scene)
	{
		m_scene->Finalize();

		m_scene.reset();
	}
}
