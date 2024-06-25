#include "pch.h"
#include "SceneManager.h"

//------------------------------
// �X�V
//------------------------------
void SceneManager::Update()
{
//#ifdef ESC_QUIT_ENABLE
//	// ESC�L�[�ŏI��
//	auto kb = DirectX::Keyboard::Get().GetState();
//	if (kb.Escape) PostQuitMessage(0);
//#endif

	// �V�[���̐؂�ւ�����
	if (!IsLoading() && m_nextScene)
	{
		DeleteScene();

		assert(m_scene == nullptr);

		// �V�[����؂�ւ�
		m_scene = std::move(m_nextScene);

		if (m_loadingScreen)
		{
			// ���[�h��ʂ̔j��
			m_loadingScreen->Finalize();
			m_loadingScreen.reset();
		}
	}

	// ���[�h��ʂ̍X�V
	if (m_loadingScreen)
	{
		m_loadingScreen->Update();
		return;
	}

	// �V�[���̍X�V
	if (m_scene) m_scene->Update();

}

//------------------------------
// �`��
//------------------------------
void SceneManager::Render()
{
	// ���[�h��ʂ̕`��
	if (m_loadingScreen)
	{
		m_loadingScreen->Render();
		return;
	}
	// �V�[���̕`��
	if (m_scene) m_scene->Render();
}

//------------------------------------------
// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
//------------------------------------------
void SceneManager::CreateDeviceDependentResources()
{
	if (m_scene) m_scene->CreateDeviceDependentResources();
}

//------------------------------------------
// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
//------------------------------------------
void SceneManager::CreateWindowSizeDependentResources()
{
	if (m_scene) m_scene->CreateWindowSizeDependentResources();
}

//------------------------------------------
// �f�o�C�X���X�g�������ɌĂяo�����֐�
//------------------------------------------
void SceneManager::OnDeviceLost()
{
	if (m_scene) m_scene->OnDeviceLost();
}

//------------------------------------------
// ���[�h�����ǂ���
//------------------------------------------
inline bool SceneManager::IsLoading()
{
	m_loadingMutex.lock();
	bool isLoading = m_isLoading;
	m_loadingMutex.unlock();

	return isLoading;
}

//------------------------------------------
// �V�[���̍폜
//------------------------------------------
void SceneManager::DeleteScene()
{
	if (m_scene)
	{
		m_scene->Finalize();

		m_scene.reset();
	}
}
