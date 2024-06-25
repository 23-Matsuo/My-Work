//--------------------------------------------------------------------------------------
// File: SceneManager.h
//
// シーンを管理するクラス
//
// Date: 2023.10.08
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#pragma once

// ESCキーで終了したい場合有効にしてください
#define ESC_QUIT_ENABLE

#ifdef ESC_QUIT_ENABLE
#include "Keyboard.h"
#endif

class SceneManager;

// シーンの基底クラス
class Scene
{
public:

	// コンストラクタ
	Scene() : m_sceneManager(nullptr) {}

	// デストラクタ
	virtual ~Scene() = default;

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update() = 0;

	// 描画
	virtual void Render() = 0;

	// 終了処理
	virtual void Finalize() = 0;

	// デバイスに依存するリソースを作成する関数
	virtual void CreateDeviceDependentResources() {}

	// ウインドウサイズに依存するリソースを作成する関数
	virtual void CreateWindowSizeDependentResources() {}

	// デバイスロストした時に呼び出される関数
	virtual void OnDeviceLost() {}

public:

	// シーンマネージャー設定関数
	void SetSceneManager(SceneManager* sceneManager) { m_sceneManager = sceneManager; }

	template <typename U, typename V>
	void ChangeScene();

private:
	// シーンマネージャーへのポインタ
	SceneManager* m_sceneManager;
};


// ロード画面の基底クラス
using LoadingScreen = Scene;

// シーンマネージャークラス
class SceneManager
{
public:

	// コンストラクタ
	SceneManager()
		:
		m_scene()
		, m_nextScene()
		, m_loadingScreen()
		, m_isLoading(false)
	{
	};

	// デストラクタ
	virtual ~SceneManager()
	{
		DeleteScene();
		if (m_loadingThread.joinable())
		{
			m_loadingThread.join();
		}
	};

	// 更新
	void Update();

	// 描画
	void Render();

	// デバイスに依存するリソースを作成する関数
	void CreateDeviceDependentResources();
	
	// ウインドウサイズに依存するリソースを作成する関数
	void CreateWindowSizeDependentResources();

	// デバイスロストした時に呼び出される関数
	virtual void OnDeviceLost();

	// シーンの設定関数
	template <typename U>
	void SetScene();

	// 次のシーンのリクエスト関数
	template <typename U>
	bool RequestSceneChange();

	template <typename U, typename V>
	bool RequestSceneChange();

	// 次のシーンの準備
	void PrepareNextScene(std::function<std::unique_ptr<Scene>()> sceneFactory);

	// ロード中かどうか
	bool IsLoading();

private:

	// 実行中のシーンへのポインタ
	std::unique_ptr<Scene> m_scene;

	// 次のシーンへのポインタ
	std::unique_ptr<Scene> m_nextScene;

	// ロード画面 
	std::unique_ptr<LoadingScreen> m_loadingScreen;

	// ロードを並行処理するための変数
	std::thread m_loadingThread;
	std::mutex m_loadingMutex;
	bool m_isLoading;

	// シーン削除関数
	void DeleteScene();

};

// 次のシーンの準備
inline void SceneManager::PrepareNextScene(std::function<std::unique_ptr<Scene>()> sceneFactory)
{

	if (m_loadingThread.joinable())
	{
		m_loadingThread.join();
	}
	// ローディング中
	m_isLoading = true;
	m_loadingThread = std::thread
	{
		[=]()
		{
			m_nextScene = sceneFactory();
			m_nextScene->SetSceneManager(this);
			m_nextScene->Initialize();

			// 一応
			m_loadingMutex.lock();
			m_isLoading = false;
			m_loadingMutex.unlock();
		}
	};
}


// シーンの設定関数
template <typename U>
bool SceneManager::RequestSceneChange()
{
	if (!m_nextScene)
	{
		// 次のシーンの準備
		PrepareNextScene([]() {return std::make_unique<U>(); });
		return true;
	}
	
	return false;
}

template <typename U, typename V>
bool SceneManager::RequestSceneChange()
{
	if (RequestSceneChange<U>())
	{
		// ロード画面を作成
		m_loadingScreen = std::make_unique<V>();
		m_loadingScreen->SetSceneManager(this);
		m_loadingScreen->Initialize();
	}

	return false;
}
template <typename U, typename V>
void Scene::ChangeScene()
{
	m_sceneManager->RequestSceneChange<U, V>();
}

// シーンの設定関数
template <typename U>
void SceneManager::SetScene()
{
	assert(m_scene == nullptr);

	RequestSceneChange<U>();
}
