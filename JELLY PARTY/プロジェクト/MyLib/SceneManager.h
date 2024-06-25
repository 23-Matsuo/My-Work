//--------------------------------------------------------------------------------------
// File: SceneManager.h
//
// �V�[�����Ǘ�����N���X
//
// Date: 2023.10.08
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#pragma once

// ESC�L�[�ŏI���������ꍇ�L���ɂ��Ă�������
#define ESC_QUIT_ENABLE

#ifdef ESC_QUIT_ENABLE
#include "Keyboard.h"
#endif

class SceneManager;

// �V�[���̊��N���X
class Scene
{
public:

	// �R���X�g���N�^
	Scene() : m_sceneManager(nullptr) {}

	// �f�X�g���N�^
	virtual ~Scene() = default;

	// ������
	virtual void Initialize() = 0;

	// �X�V
	virtual void Update() = 0;

	// �`��
	virtual void Render() = 0;

	// �I������
	virtual void Finalize() = 0;

	// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
	virtual void CreateDeviceDependentResources() {}

	// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
	virtual void CreateWindowSizeDependentResources() {}

	// �f�o�C�X���X�g�������ɌĂяo�����֐�
	virtual void OnDeviceLost() {}

public:

	// �V�[���}�l�[�W���[�ݒ�֐�
	void SetSceneManager(SceneManager* sceneManager) { m_sceneManager = sceneManager; }

	template <typename U, typename V>
	void ChangeScene();

private:
	// �V�[���}�l�[�W���[�ւ̃|�C���^
	SceneManager* m_sceneManager;
};


// ���[�h��ʂ̊��N���X
using LoadingScreen = Scene;

// �V�[���}�l�[�W���[�N���X
class SceneManager
{
public:

	// �R���X�g���N�^
	SceneManager()
		:
		m_scene()
		, m_nextScene()
		, m_loadingScreen()
		, m_isLoading(false)
	{
	};

	// �f�X�g���N�^
	virtual ~SceneManager()
	{
		DeleteScene();
		if (m_loadingThread.joinable())
		{
			m_loadingThread.join();
		}
	};

	// �X�V
	void Update();

	// �`��
	void Render();

	// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateDeviceDependentResources();
	
	// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateWindowSizeDependentResources();

	// �f�o�C�X���X�g�������ɌĂяo�����֐�
	virtual void OnDeviceLost();

	// �V�[���̐ݒ�֐�
	template <typename U>
	void SetScene();

	// ���̃V�[���̃��N�G�X�g�֐�
	template <typename U>
	bool RequestSceneChange();

	template <typename U, typename V>
	bool RequestSceneChange();

	// ���̃V�[���̏���
	void PrepareNextScene(std::function<std::unique_ptr<Scene>()> sceneFactory);

	// ���[�h�����ǂ���
	bool IsLoading();

private:

	// ���s���̃V�[���ւ̃|�C���^
	std::unique_ptr<Scene> m_scene;

	// ���̃V�[���ւ̃|�C���^
	std::unique_ptr<Scene> m_nextScene;

	// ���[�h��� 
	std::unique_ptr<LoadingScreen> m_loadingScreen;

	// ���[�h����s�������邽�߂̕ϐ�
	std::thread m_loadingThread;
	std::mutex m_loadingMutex;
	bool m_isLoading;

	// �V�[���폜�֐�
	void DeleteScene();

};

// ���̃V�[���̏���
inline void SceneManager::PrepareNextScene(std::function<std::unique_ptr<Scene>()> sceneFactory)
{

	if (m_loadingThread.joinable())
	{
		m_loadingThread.join();
	}
	// ���[�f�B���O��
	m_isLoading = true;
	m_loadingThread = std::thread
	{
		[=]()
		{
			m_nextScene = sceneFactory();
			m_nextScene->SetSceneManager(this);
			m_nextScene->Initialize();

			// �ꉞ
			m_loadingMutex.lock();
			m_isLoading = false;
			m_loadingMutex.unlock();
		}
	};
}


// �V�[���̐ݒ�֐�
template <typename U>
bool SceneManager::RequestSceneChange()
{
	if (!m_nextScene)
	{
		// ���̃V�[���̏���
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
		// ���[�h��ʂ��쐬
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

// �V�[���̐ݒ�֐�
template <typename U>
void SceneManager::SetScene()
{
	assert(m_scene == nullptr);

	RequestSceneChange<U>();
}
