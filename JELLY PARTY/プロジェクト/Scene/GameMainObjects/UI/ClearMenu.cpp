//--------------------------------------------------------------------------------------
// File: ClearMenu.cpp
//
// �N���A���̃��j���[
//
// Last Update: 2023/10/30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "ClearMenu.h"

#include "MyLib/BinaryFile.h"
#include "MyLib/SoundUtility.h"

using namespace DirectX;

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
ClearMenu::ClearMenu(int width, int height)
    :
    m_state(ClearMenu::STATE::IDLE)
{
    UNREFERENCED_PARAMETER(height);

    // ���j���[�̈ʒu

    m_menuManager = std::make_unique<MenuManager>(
        L"ButtonBase", DirectX::SimpleMath::Vector2(static_cast<float>(width - width / 5), static_cast<float>(height / 2 + 200)));


    // �u������x�v
    m_menuManager->Add(L"ReStart"
        , SimpleMath::Vector2(static_cast<float>(width - width / 5), static_cast<float>(height / 2 + 100))
        , SimpleMath::Vector2(0.0f, 0.0f)
        , ANCHOR::MIDDLE_CENTER);

    // �u���j���[�ցv
    m_menuManager->Add(L"ToMenu"
        , SimpleMath::Vector2(static_cast<float>(width - width / 5), static_cast<float>(height / 2 + 200))
        , SimpleMath::Vector2(0.0f, 0.0f)
        , ANCHOR::MIDDLE_CENTER);
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
ClearMenu::~ClearMenu()
{
    m_menuManager.reset();
}

//------------------------------------------
// �X�V
//------------------------------------------
void ClearMenu::Update()
{
    m_menuManager->Update();
}

//------------------------------------------
// �`��
//------------------------------------------
void ClearMenu::Render()
{
    m_menuManager->Render();
}

//------------------------------------------
// �|�b�v�A�j���[�V����
//------------------------------------------
void ClearMenu::Pop()
{

    switch (m_state)
    {
    case ClearMenu::IDLE:
        break;
    case ClearMenu::POPOUT:
        m_menuManager->SetState(MenuManager::STATE::POPOUT);
        break;
    case ClearMenu::POPIN:
        m_menuManager->SetState(MenuManager::STATE::POPIN);

        break;
    default:
        break;
    }

    m_menuManager->Pop();

    if (m_menuManager->GetState() == MenuManager::STATE::IDLE)
    {
        m_state = IDLE;
    }

}
