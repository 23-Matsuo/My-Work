//--------------------------------------------------------------------------------------
// File: TitleScene.h
//
// �^�C�g���V�[���N���X
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
	// �V�[���̏��
	enum SceneState
	{
		Title,
		Select
	};

public:
	
	// �R���X�g���N�^
	TitleScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update() override;

	// �`��
	void Render() override;

	// �I������
	void Finalize() override;

	// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateDeviceDependentResources() override;

	// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateWindowSizeDependentResources() override;

	// �f�o�C�X���X�g�������ɌĂяo�����֐�
	void OnDeviceLost() override;

public:

	void SetState(const TitleScene::SceneState& state) { m_sceneState = state; }

	const TitleScene::SceneState& GetState() { return m_sceneState; }


private:

	// ���C������
	void MainUpdate();

	// �{�^�����������Ƃ��̏���
	void PressedButton();

	// ���S�V�[���̍X�V
	void LogoScene(const float& elapsedTime, DirectX::Keyboard::KeyboardStateTracker* keyState);

	// �Z���N�g�V�[���̍X�V
	void SelectScene(const float& elapsedTime, DirectX::Keyboard::KeyboardStateTracker* keyState);

private:

	// �T�E���h���[�e�B���e�B�ւ̃|�C���^
	std::unique_ptr<SoundUtility> m_soundUtility;
	// bgm�ւ̃|�C���^
	DirectX::SoundEffectInstance* m_bgmInstance;
	// �{�^�����ւ̃|�C���^
	DirectX::SoundEffectInstance* m_pushButtonInstance;

	// ���j���[�ւ̃|�C���^
	std::unique_ptr<StartMenu> m_menu;

	// �t�F�[�h�ւ̃|�C���^
	std::unique_ptr<Transition> m_fade;

	// �^�C�g���̃��S
	std::unique_ptr<TitleLogo> m_titleLogo;

	// �w�i
	std::unique_ptr<TitleBack> m_backGround;

	// �u�X�y�[�X�����ăX�^�[�g�v�I�u�W�F�N�g
	std::unique_ptr<PushSpace> m_pushSpace;

	// �u���C�����j���[�v�I�u�W�F�N�g
	std::unique_ptr<MainMenuText> m_mainText;

	// �V�ѕ�
	std::unique_ptr<HowToPlay> m_howToPlay;

	// �ō��X�R�A
	std::unique_ptr<HighScore> m_maxScore;

private:

	// �V�[���̏��
	SceneState m_sceneState;

	// ���S�A�j���[�V�����t���O
	bool m_logoAnimated;

	// �X�^�[�g�t���O
	bool m_isStarted;

	// �X�e�[�W�I���t���O
	bool m_isSelected;

	// �I���t���O
	bool m_isQuit;

	// �^�C�g���̃V�[���`�F���W�t���O
	bool m_changed;

	// �|�b�v����
	bool m_howToPlayPoppedIn;
	bool m_howToPlayPoppedOut;
};

