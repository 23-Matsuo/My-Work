//--------------------------------------------------------------------------------------
// File: PlayScene.h
//
// �Q�[���̃��C���V�[���N���X
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
		COUNTDOWN,	// �J�E���g�_�E��
		STARTED,	// �X�^�[�g��
		RESULT,		// ���U���g
	};
public:
	
	// �R���X�g���N�^
	PlayScene();

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

private:

	// �Q�[�����C���̍X�V
	void MainUpdate();

	// �S�[����̏���
	void ResultUpdate(DirectX::Keyboard::KeyboardStateTracker* keyState);

	// �f�o�b�O�J�����X�V
	void DebugUpdate(DirectX::Keyboard::KeyboardStateTracker* key);

private:

	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// �O���b�h���ւ̃|�C���^
	std::unique_ptr<Imase::GridFloor> m_gridFloor;

private:

	// �J�����ւ̃|�C���^
	std::unique_ptr<GameCamera> m_gameCamera;
	// �t�F�[�h�ւ̃|�C���^
	std::unique_ptr<Transition> m_fade;
	// �v���C���[�ւ̃|�C���^
	std::unique_ptr<CatSphere> m_catBall;

	// �N���A���̃��j���[�ւ̃|�C���^
	std::unique_ptr<ClearMenu> m_clearMenu;
	// �J�E���g�_�E���ւ̃|�C���^
	std::unique_ptr<CountDown> m_countDown;
	// �J�E���g�_�E�����I��������̕����I�u�W�F�N�g
	std::unique_ptr<DoWhat> m_doWhat;
	// �I�����̕����I�u�W�F�N�g
	std::unique_ptr<Finish> m_finish;
	// �u���U���g�v�I�u�W�F�N�g
	std::unique_ptr<Result> m_result;
	// �^�C�}�[�ւ̃|�C���^
	std::unique_ptr<GameTimer> m_gameTimer;
	// �X�R�A�ւ̃|�C���^
	std::unique_ptr<Score> m_score;
	// �\�t�g�{�f�B
	std::unique_ptr<StageSoftBodyManager> m_softBodyManager;
	// ��
	std::unique_ptr<StageFloorManager> m_stageFloor;
	// �V���h�E�}�b�v
	std::unique_ptr<ShadowMap> m_shadowMap;

private:

	// �T�E���h���[�e�B���e�B�ւ̃|�C���^
	std::unique_ptr<SoundUtility> m_soundUtility;
	// bgm�ւ̃|�C���^
	DirectX::SoundEffectInstance* m_gameBGM;
	// �S�[��bgm�ւ̃|�C���^
	DirectX::SoundEffectInstance* m_resultBGM;
	// �{�^�����ւ̃|�C���^
	DirectX::SoundEffectInstance* m_pushButtonSound;
	// �J�n���ւ̃|�C���^
	DirectX::SoundEffectInstance* m_startSound;
	// �I�����ւ̃|�C���^
	DirectX::SoundEffectInstance* m_finishSound;

private:

	// �V�[���̏��
	SceneState m_sceneState;

	// �t���O
	bool m_isTimeOver;
	bool m_playedFinishedSound;

	// ���X�^�[�g�t���O
	bool m_reStart;
	// ���j���[�ɖ߂�t���O
	bool m_backMenu;

	// ���U���g�̎���
	float m_resultTime;
};