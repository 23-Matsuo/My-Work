//--------------------------------------------------------------------------------------
// File: TitleScene.cpp
//
// �^�C�g���V�[���N���X
//
// Date: 2023.7.21
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "TitleScene.h"
#include "LoadScene.h"
#include "PlayScene.h"

#include "GameManager.h"

using namespace DirectX;

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
TitleScene::TitleScene()
	:
	m_logoAnimated(false)
	, m_isStarted(false)
	, m_isSelected(false)
	, m_isQuit(false)
	, m_changed(false)
	, m_howToPlayPoppedIn(false)
	, m_howToPlayPoppedOut(false)
	, m_bgmInstance(nullptr)
	, m_pushButtonInstance(nullptr)
	, m_sceneState(GameManager::GetInstance()->GetLastState())
{
	// �ŏI�X�e�[�g���Z���N�g�Ȃ烍�S�V�[������n�߂Ȃ�
	if (m_sceneState == Select)
	{
		m_changed = true;
	}

	// �ō��X�R�A��ݒ肷��
	nlohmann::json data;
	Resources::GetInstance()->ReadJsonFile(&data, L"Resources/Datas/ScoreData");
	GameManager::GetInstance()->SetHighScore(data["MAX_SCORE"]);
}

//------------------------------------------
// ������
//------------------------------------------
void TitleScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
	// �T�E���h���[�e�B���e�B�쐬
	m_soundUtility = std::make_unique<SoundUtility>();

	// �T�E���h�C���X�^���X�쐬
	m_bgmInstance = Resources::GetInstance()->GetSound(L"TitleBGM");
	m_pushButtonInstance = Resources::GetInstance()->GetSound(L"Button");
	// BGM���Đ�
	PlaySound(m_bgmInstance, true);
}

//------------------------------------------
// �X�V
//------------------------------------------
void TitleScene::Update()
{
	// ���X�^�[�g�������炷���V�[���؂�ւ�
	if (GameManager::GetInstance()->GetReStarted()) ChangeScene<PlayScene, LoadScene>();

	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	
	// �g�����W�V�����̍X�V
	m_fade->Update();

	// �T�E���h�̍X�V
	m_soundUtility->Update(m_bgmInstance);

	// �{�^���������Ă��Ȃ��ăg�����W�V�������ҋ@��Ԃ̎��̏���
	MainUpdate();

	// ���j���[�{�^�������������̏���
	PressedButton();

	// ESC�������Ă��邩�g�����W�V�������I����Ă�����
	if (m_isQuit && (m_fade->GetState() == Transition::STATE::IDLE))
	{
		m_bgmInstance->Stop();
		// �I��
		PostQuitMessage(0);
	}
}

//------------------------------------------
// �`��
//------------------------------------------
void TitleScene::Render()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	//auto debugFont = Resources::GetInstance()->GetDebugFont();
	//debugFont->AddString(L"TitleScene", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));

	// �^�C�g���̔w�i��`��
	m_backGround->Render();

	switch (m_sceneState)
	{
	case TitleScene::Title:
		// �^�C�g���̃��S��`��
		m_titleLogo->Render();

		// �X�y�[�X�����ĉ摜��`��
		m_pushSpace->Render();	
		break;
	case TitleScene::Select:
		// ���j���[�`��
		m_menu->Render();

		// �u���C�����j���[�v�`��
		m_mainText->Render();

		// �V�ѕ��`��
		m_howToPlay->Render();

		if (m_howToPlay->GetScale() == m_howToPlay->GetDefaultScale())
		{
			// �ō��X�R�A�`��
			m_maxScore->Render();
		}

		break;
	default:
		break;
	}

	// �g�����W�V������`��
	m_fade->Render();


}

//------------------------------------------
// �I������
//------------------------------------------
void TitleScene::Finalize()
{
	m_bgmInstance->Stop();
	m_pushButtonInstance->Stop();
}

//------------------------------------------
// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
//------------------------------------------
void TitleScene::CreateDeviceDependentResources()
{
}

//------------------------------------------
// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
//------------------------------------------
void TitleScene::CreateWindowSizeDependentResources()
{
	// ��ʂ̃T�C�Y���擾����
	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	// ���j���[�̍쐬
	m_menu = std::make_unique<StartMenu>(rect.right, rect.bottom);

	// �^�C�g�����S�̍쐬
	m_titleLogo = std::make_unique<TitleLogo>(rect.right, rect.bottom);

	// �^�C�g���w�i�̍쐬
	m_backGround = std::make_unique<TitleBack>(rect.right, rect.bottom);

	// �ŏ��̈ē��I�u�W�F�N�g�̍쐬
	m_pushSpace = std::make_unique<PushSpace>(rect.right, rect.bottom);

	// ���j���[�e�L�X�g�̍쐬
	m_mainText = std::make_unique<MainMenuText>(rect.right, rect.bottom);

	// �V�ѕ��̍쐬
	m_howToPlay = std::make_unique<HowToPlay>(rect.right, rect.bottom);

	// �ō��X�R�A�̍쐬
	m_maxScore = std::make_unique<HighScore>(rect.right, rect.bottom);

	m_fade = std::make_unique<Transition>();
	m_fade->Initialize();

	if (m_sceneState == Select)
	{
		m_menu->SetState(StartMenu::STATE::POPIN);
		m_titleLogo->SetScale(SimpleMath::Vector2::Zero);
		m_pushSpace->SetScale(SimpleMath::Vector2::Zero);
	} 
}

//------------------------------------------
// �f�o�C�X���X�g�������ɌĂяo�����֐�
//------------------------------------------
void TitleScene::OnDeviceLost()
{
	Finalize();
}

//------------------------------------------
// ���C������
//------------------------------------------
void TitleScene::MainUpdate()
{
	// �X�^�[�g���Ă��āA�g�����W�V�������ҋ@��Ԃ���Ȃ�
	if (m_isStarted) return;
	if (m_fade->GetState() != Transition::STATE::IDLE) return;

	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	// �L�[�{�[�h�擾
	auto keyState = Graphics::GetInstance()->GetKeyboardStateTracker();
	auto key = keyState->GetLastState();

	switch (m_sceneState)
	{
	case TitleScene::Title:
		LogoScene(elapsedTime, keyState);
		break;
	case TitleScene::Select:
		SelectScene(elapsedTime, keyState);
		break;
	default:
		break;
	}
}

//------------------------------------------
// �{�^�����������Ƃ��̏���
//------------------------------------------
void TitleScene::PressedButton()
{
	// �{�^�������Ă��邩�ҋ@��ԂȂ�
	if (!m_isStarted) return;
	if (m_fade->GetState() != Transition::STATE::IDLE) return;
	if (m_menu->GetState() != MenuManager::STATE::IDLE) return;

	switch (m_menu->GetMenuManager()->GetMenuIndex())
	{
	case StartMenu::SelectedButton::PLAY:
		m_bgmInstance->Stop();
		// �v���C�V�[���ǂݍ���
		ChangeScene<PlayScene, LoadScene>();
		break;
	case StartMenu::SelectedButton::QUIT:
		break;
	default:
		break;
	}

}

//------------------------------------------
// ���S�V�[���̍X�V
//------------------------------------------
void TitleScene::LogoScene(const float& elapsedTime, DirectX::Keyboard::KeyboardStateTracker* keyState)
{
	// �㉺�Ɉړ�
	m_pushSpace->Update(elapsedTime);

	// ���S�̃A�j���[�V�������I����Ă��邩�ǂ���
	if (!m_titleLogo->Update(elapsedTime)) return;
	else
	{
		if (!m_logoAnimated)
		{
			m_pushSpace->SetState(PushSpace::STATE::POPIN);
			m_logoAnimated = true;
		}
	}

	// �A�j���[�V�����ҋ@���
	if (m_titleLogo->GetState() == TitleLogo::STATE::IDLE && m_pushSpace->GetState() == PushSpace::STATE::IDLE)
	{
		if (keyState->pressed.Escape)
		{
			// SlideIn������
			m_fade->SetState(Transition::STATE::SLIDEIN);
			m_soundUtility->SetState(SoundUtility::State::FADEOUT);

			// ESC����������I������
			// �I���t���O��true
			m_isQuit = true;
		}

		if (keyState->pressed.Space)
		{
			// �X�y�[�X����������|�b�v�A�E�g
			m_titleLogo->SetState(TitleLogo::STATE::POPOUT);
			m_pushSpace->SetState(PushSpace::STATE::POPOUT);

			m_changed = true;
			// ����炷
			PlaySound(m_pushButtonInstance, false);

		}
	}

	// �|�b�v�̍X�V
	m_titleLogo->Pop(elapsedTime);
	m_pushSpace->Pop(elapsedTime);

	// �|�b�v�A�E�g���I����Ă�����ύX
	if (m_titleLogo->GetState() == TitleLogo::STATE::IDLE && m_changed)
	{
		m_sceneState = Select;
		m_menu->SetState(StartMenu::STATE::POPIN);
	}
}

//------------------------------------------
// �Z���N�g�V�[���̍X�V
//------------------------------------------
void TitleScene::SelectScene(const float& elapsedTime, DirectX::Keyboard::KeyboardStateTracker* keyState)
{
	// ���S�̃A�j���[�V�������I����Ă��邩�ǂ���
	m_mainText->Update(elapsedTime);

	if (m_menu->GetState() == StartMenu::STATE::IDLE && m_howToPlay->GetState() == HowToPlay::STATE::IDLE)
	{
		// ���j���[�X�V
		m_menu->Update();

		// �V�ѕ��摜�̃|�b�v�A�j���[�V��������
		if (m_menu->GetMenuManager()->GetMenuIndex() == StartMenu::SelectedButton::PLAY && !m_howToPlayPoppedIn)
		{
			m_howToPlay->SetState(HowToPlay::STATE::POPIN);
			m_howToPlayPoppedIn = true;
			m_howToPlayPoppedOut = false;
		}
		else if (m_menu->GetMenuManager()->GetMenuIndex() == StartMenu::SelectedButton::QUIT && !m_howToPlayPoppedOut)
		{
			m_howToPlay->SetState(HowToPlay::STATE::POPOUT);
			m_howToPlayPoppedOut = true;
			m_howToPlayPoppedIn = false;
		}

		// Space��E����������
		if (keyState->pressed.Space || keyState->pressed.E || keyState->pressed.Enter)
		{
			// SlideIn������
			m_fade->SetState(Transition::STATE::SLIDEIN);
			// BGM�t�F�[�h�A�E�g
			m_soundUtility->SetState(SoundUtility::State::FADEOUT);
			// ����炷
			PlaySound(m_pushButtonInstance, false);
			if (m_menu->GetMenuManager()->GetMenuIndex() == StartMenu::SelectedButton::PLAY)
			{
				// �{�^��������
				m_isStarted = true;

				// �ŏI�V�[���X�e�[�g��ݒ�
				GameManager::GetInstance()->SetTitleLastState(m_sceneState);
			}
			else
			{
				m_isQuit = true;
			}
		}

		// ESC���������烍�S�V�[���ɖ߂�
		if (keyState->pressed.Escape || keyState->pressed.Back)
		{
			m_menu->SetState(StartMenu::STATE::POPOUT);
			if (m_menu->GetMenuManager()->GetMenuIndex() == StartMenu::SelectedButton::PLAY)
			{
				m_howToPlay->SetState(HowToPlay::STATE::POPOUT);
			}
			m_changed = false;
			m_howToPlayPoppedIn = false;
		}

	}
	// �|�b�v�A�j���[�V�����̍X�V
	m_menu->Pop();
	m_howToPlay->Pop(elapsedTime);

	// ���j���[���ҋ@��Ԃ�ESC�������Ă�����
	if (m_menu->GetState() == MenuManager::STATE::IDLE && !m_changed)
	{
		m_sceneState = Title;
		m_titleLogo->SetState(TitleLogo::STATE::POPIN);
		m_pushSpace->SetState(PushSpace::STATE::POPIN);
	}
}
