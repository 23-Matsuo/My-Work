//--------------------------------------------------------------------------------------
// File: PlayScene.cpp
//
// �Q�[���̃��C���V�[���N���X
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "PlayScene.h"
#include "TitleScene.h"
#include "LoadScene.h"
#include "Resources/Sounds/SoundIdx.h"
#include "GameManager.h"

using namespace DirectX;

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
PlayScene::PlayScene()
	:
	m_sceneState(SceneState::COUNTDOWN)
	, m_gameBGM(nullptr)
	, m_resultBGM(nullptr)
	, m_pushButtonSound(nullptr)
	, m_finishSound(nullptr)
	, m_startSound(nullptr)
	, m_reStart(false)
	, m_backMenu(false)
	, m_isTimeOver(false)
	, m_playedFinishedSound(false)
	, m_resultTime(0.0f)
{
}

//------------------------------------------
// ������
//------------------------------------------
void PlayScene::Initialize()
{
	m_isTimeOver = false;
	m_playedFinishedSound = false;

	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// ���̍쐬
	m_catBall = std::make_unique<CatSphere>();
	m_catBall->Intialize();

	// �J�E���g�_�E���̍쐬
	m_countDown = std::make_unique<CountDown>();

	// �I�����̕����̍쐬
	m_finish = std::make_unique<Finish>();

	// �Q�[���^�C�}�[�̍쐬
	m_gameTimer = std::make_unique<GameTimer>();

	m_shadowMap = std::make_unique<ShadowMap>();
	m_shadowMap->Initialize();

	// �T�E���h���[�e�B���e�B�쐬
	m_soundUtility = std::make_unique<SoundUtility>();

	// BGM�쐬
	m_gameBGM = Resources::GetInstance()->GetSound(L"PlayBGM");
	m_resultBGM = Resources::GetInstance()->GetSound(L"ResultBGM");
	// �{�^�����쐬
	m_pushButtonSound = Resources::GetInstance()->GetSound(L"Button");
	// �I�����쐬
	m_finishSound = Resources::GetInstance()->GetSound(L"FinishGong");
	// �J�n���쐬
	m_startSound = Resources::GetInstance()->GetSound(L"Finish");

	// BGM��炷
	PlaySound(m_gameBGM, true);

}

//------------------------------------------
// �X�V
//------------------------------------------
void PlayScene::Update()
{
	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	// �L�[�{�[�h�擾
	auto key = Graphics::GetInstance()->GetKeyboardStateTracker();

	auto keyboard = key->GetLastState();

	// �g�����W�V�����̍X�V
	m_fade->Update();

	// BGM�̉��ʍX�V
	m_soundUtility->Update(m_gameBGM);

	// �f�o�b�O���̍X�V
	DebugUpdate(key);

	switch (m_sceneState)
	{
	case SceneState::COUNTDOWN:			// �J�E���g�_�E����
	{
		// �t�F�[�h���I����Ă��Ȃ���Ώ������Ȃ�
		if (m_fade->GetState() != Transition::STATE::IDLE) break;

		// �J�E���g�_�E����0�ɂȂ�����
		if (m_countDown->Update())
		{
			// �X�^�[�g��
			m_startSound->Play(false);
			// �Q�[���X�^�[�g
			m_sceneState = SceneState::STARTED;
			m_softBodyManager->SetSceneState(StageSoftBodyManager::SceneState::MAIN);
		}
		break;
	}
	case SceneState::STARTED:
	{

		m_doWhat->Update();

		if (!m_catBall->GetIsDead())
		{
			// ���ԍX�V
			if (m_gameTimer->Update())
			{
				m_isTimeOver = true;
			}
		}

		if (m_catBall->GetPosition().y < -1.0f)
		{
			m_gameCamera->SetType(GameCamera::Type::DEAD);
		}

		// �������񂾏���
		if (m_catBall->GetIsDead() || m_isTimeOver)
		{

			m_soundUtility->SetFadeVolume(0.8f);
			m_soundUtility->FadeOut(m_gameBGM);

			if (!m_playedFinishedSound)
			{
				PlaySound(m_finishSound, false);
				m_playedFinishedSound = true;
			}
			// �u�t�B�j�b�V���v�����X�V
			if (m_finish->Update())
			{
				// BGM���~�߂�
				m_gameBGM->Stop();

				// ���̏�����
				m_catBall->Intialize();
				// ���̃��U���g���̏�Ԃ�ݒ�
				m_catBall->SetResultState(CatSphere::ResultState::LOOKUP);
				// �V�[���̏�Ԃ�ύX
				m_sceneState = SceneState::RESULT;
				// �J������ύX
				m_gameCamera->SetType(GameCamera::Type::RESULT);
				// �[���[�̐��\���I�u�W�F�N�g�̏�Ԃ�ύX
				m_score->SetSceneState(Score::SceneState::RESULT);
				// �X�e�[�W��̃\�t�g�{�f�B�̏�Ԃ�ύX
				m_softBodyManager->SetSceneState(StageSoftBodyManager::SceneState::RESULT);

				// �擾�����ʂ��L�^
				GameManager::GetInstance()->SetNowScore(m_score->GetScore());

				// ���U���g�̏�����
				m_softBodyManager->CubeResultInitialize(m_score.get());

				// �J�E���g��0��
				m_score->Initialize();


				// �|�b�v�C����ԂɕύX
				m_clearMenu->SetState(ClearMenu::STATE::POPIN);

				// ���U���g�̎��Ԃ�������
				m_resultTime = 0.0f;

				// ���U���g��BGN��炷
				PlaySound(m_resultBGM, true);

			}
		}
		else
		{
			// �Q�[�����C���̍X�V
			MainUpdate();
		}
		
		break;
	}
	case SceneState::RESULT:
	{
		// �u���U���g�I�v�X�V
		if (!m_result->Update()) break;

		// ���U���g�̃A�j���[�V�������I����Ă�����
		// �L�����̃��U���g����
		m_catBall->ResultUpdate();

		// �\�t�g�{�f�B�̃��U���g����
		m_softBodyManager->Update(0);

		// �J�E���g���I�������
		if (m_softBodyManager->GetCountFinished())
		{
			// ���U���g�̌o�ߎ��Ԃ��v�Z
			m_resultTime += elapsedTime;

			// ���U���g���
			m_catBall->SetResultState(CatSphere::ResultState::CLAP);

			// ���U���g�̃��j���[����
			ResultUpdate(key);
		}
		break;
	}
	default:
		break;
	}

	// �X�R�A�̍X�V
	m_score->Update();

	// �J�����ɋ��̈ʒu��ݒ�
	m_gameCamera->SetPlayerPosition(m_catBall->GetPosition());

	// �J�����̍X�V
	m_gameCamera->Update(elapsedTime);

}

//------------------------------------------
// �`��
//------------------------------------------
void PlayScene::Render()
{
	// �r���[�s��̍쐬
	m_view = m_gameCamera->GetView();

	//// �O���b�h���̕`��
	//m_gridFloor->Render(context, m_view, m_projection);

	m_shadowMap->Render(m_catBall.get(), m_stageFloor.get(), m_softBodyManager.get());

	switch (m_sceneState)
	{
	case SceneState::COUNTDOWN:		// �J�E���g�_�E����
	{
		// �J�E���g�_�E���`��
		m_countDown->Render();

		// �Q�[�������ԕ`��
		m_gameTimer->Render();

		break;
	}
	case SceneState::STARTED:		// �X�^�[�g����
	{
		// �u���߂�I�v�`��
		m_doWhat->Render();

		// �Q�[�������ԕ`��
		m_gameTimer->Render();

		// �L�������������������͎��Ԃ��I�������
		if (m_catBall->GetIsDead() || m_isTimeOver)
		{
			// �u�t�B�j�b�V��!�v�`��
			m_finish->Render();
		}
		break;
	}
	case SceneState::RESULT:
	{
		// ���U���g�`��
		m_result->Render();
		// ���j���[�`��
		m_clearMenu->Render();

		break;
	}
	default:
		break;
	}

	// �X�R�A�`��
	m_score->Render();

	// �t�F�[�h�`��
	m_fade->Render();

}

//------------------------------------------
// �I������
//------------------------------------------
void PlayScene::Finalize()
{
	m_startSound->Stop();
	m_finishSound->Stop();

	m_gameBGM = nullptr;
	m_resultBGM = nullptr;
	m_pushButtonSound = nullptr;
	m_finishSound = nullptr;
	m_startSound = nullptr;
}

//------------------------------------------
// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
//------------------------------------------
void PlayScene::CreateDeviceDependentResources()
{
	// �\�t�g�{�f�B�}�l�[�W���[�쐬
	m_softBodyManager = std::make_unique<StageSoftBodyManager>();
	// ���쐬
	m_stageFloor = std::make_unique<StageFloorManager>(Graphics::GetInstance()->GetDeviceResources());

	// �g�����W�V�����쐬
	m_fade = std::make_unique<Transition>();
	// �g�����W�V������������
	m_fade->Initialize();
}

//------------------------------------------
// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
//------------------------------------------
void PlayScene::CreateWindowSizeDependentResources()
{
	// ��ʂ̃T�C�Y���擾����
	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	// �J�����̍쐬
	m_gameCamera = std::make_unique<GameCamera>(rect);
	// �J�����̈ʒu��������
	SimpleMath::Vector3 eyePos = SimpleMath::Vector3::Zero;
	// �J�����̎��_��������
	SimpleMath::Vector3 targetPos = SimpleMath::Vector3::Zero;
	// �J�����ɐݒ�
	m_gameCamera->SetPositionTarget(eyePos, targetPos);

	// �ˉe�s��̍쐬
	m_projection = m_gameCamera->GetProjection();

	// ���j���[�̍쐬
	m_clearMenu = std::make_unique<ClearMenu>(rect.right, rect.bottom);

	// �J�E���g�_�E���I����̕����̍쐬
	m_doWhat = std::make_unique<DoWhat>(rect.right, rect.bottom);

	// �X�R�A�̍쐬
	m_score = std::make_unique<Score>(rect.right, rect.bottom);

	// �u���U���g�v�̍쐬
	m_result = std::make_unique<Result>(rect.right, rect.bottom);
}

//------------------------------------------
// �f�o�C�X���X�g�������ɌĂяo�����֐�
//------------------------------------------
void PlayScene::OnDeviceLost()
{
	Finalize();
}

//------------------------------------------
// �Q�[�����C���̍X�V
//------------------------------------------
void PlayScene::MainUpdate()
{
	auto timer = Graphics::GetInstance()->GetStepTimer();

	// �\�t�g�{�f�B���X�V
	m_softBodyManager->Update(m_gameTimer->GetSpendingTime());

	// �����X�V
	m_stageFloor->Update();

	// ���̍X�V
	m_catBall->Update(*timer);
}

//------------------------------------------
// ���U���g�̃��j���[����
//------------------------------------------
void PlayScene::ResultUpdate(DirectX::Keyboard::KeyboardStateTracker* keyState)
{

	if (m_clearMenu->GetState() == StartMenu::STATE::IDLE)
	{
		// ���j���[�X�V
		m_clearMenu->Update();

		// Space��E����������
		if (keyState->pressed.Space || keyState->pressed.E || keyState->pressed.Enter || m_resultTime > 60.0f)
		{

			// SlideIn������
			m_fade->SetState(Transition::STATE::SLIDEIN);
			// BGM�t�F�[�h�A�E�g
			m_soundUtility->SetState(SoundUtility::State::FADEOUT);
			// �X�y�[�X����������|�b�v�A�E�g
			m_clearMenu->SetState(ClearMenu::STATE::POPOUT);

			if (m_clearMenu->GetMenuManager()->GetMenuIndex() == m_clearMenu->MENU || m_resultTime > 60.0f)
			{
				// ���j���[�ɂ��ǂ�
				if (!m_backMenu)
				{
					m_backMenu = true;
					// ����炷
					PlaySound(m_pushButtonSound, false);
				}
			}
			else if (m_clearMenu->GetMenuManager()->GetMenuIndex() == m_clearMenu->RESTART)
			{
				// ���X�^�[�g
				if (!m_reStart)
				{
					m_reStart = true;
					// ����炷
					PlaySound(m_pushButtonSound, false);
				}
			}
		}
	}

	if ((m_reStart || m_backMenu) && m_fade->GetState() == Transition::STATE::IDLE)
	{
		// �^�C�g����ǂݍ���
		ChangeScene<TitleScene, LoadScene>();

		// BGM�I��
		m_resultBGM->Stop();

		// ���X�^�[�g���ǂ����n��
		GameManager::GetInstance()->SetReStarted(m_reStart);
	}
	// �|�b�v�A�j���[�V�����̍X�V
	m_clearMenu->Pop();

}

//------------------------------------------
// �f�o�b�O�̑���
//------------------------------------------
void PlayScene::DebugUpdate(DirectX::Keyboard::KeyboardStateTracker* key)
{
	if (key->pressed.L)
	{
		// �f�o�b�O�p�J�����ɕύX
		m_gameCamera->SetType(GameCamera::Type::DEBUG);
	}
	if (key->pressed.K)
	{
		// �Œ�J�����ɕύX
		m_gameCamera->SetType(GameCamera::Type::GAMEMAIN);
	}
	if (key->pressed.J)
	{
		// �Œ�J�����ɕύX
		m_gameCamera->SetType(GameCamera::Type::RESULT);
	}
}