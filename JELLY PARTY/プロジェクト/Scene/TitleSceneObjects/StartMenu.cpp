//--------------------------------------------------------------------------------------
// File: StartMenu.cpp
//
// タイトルのメニュー
//
// Date: 2023/07/26
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "StartMenu.h"

#include "MyLib/BinaryFile.h"
#include "MyLib/Resources.h"
#include "MyLib/Graphics.h"
#include "MyLib/SoundUtility.h"
using namespace DirectX;

//------------------------------------------
// コンストラクタ
//------------------------------------------
StartMenu::StartMenu(int width, int height)
    :
    m_state(IDLE)
{
    UNREFERENCED_PARAMETER(height);

    m_menuManager = std::make_unique<MenuManager>(
        L"ButtonBase",
        DirectX::SimpleMath::Vector2(static_cast<float>(width / 2), 430.0f)
    );

    m_menuManager->Add(L"Start"
        , SimpleMath::Vector2(static_cast<float>(width / 7), static_cast<float>(height / 3))
        , SimpleMath::Vector2(0.0f, 0.0f)
        , ANCHOR::MIDDLE_CENTER);
    m_menuManager->Add(L"Quit"
        , SimpleMath::Vector2(static_cast<float>(width / 7), static_cast<float>(height / 3 + 100))
        , SimpleMath::Vector2(0.0f, 0.0f)
        , ANCHOR::MIDDLE_CENTER);

}

//------------------------------------------
// デストラクタ
//------------------------------------------
StartMenu::~StartMenu()
{
    m_menuManager.reset();
}

//------------------------------------------
// 更新
//------------------------------------------
void StartMenu::Update()
{
    if (m_state == IDLE)
    {
        m_menuManager->Update();
    }
}


//------------------------------------------
// 描画
//------------------------------------------
void StartMenu::Render()
{
    m_menuManager->Render();
}

//------------------------------------------
// ポップアニメーション
//------------------------------------------
void StartMenu::Pop()
{

    switch (m_state)
    {
    case StartMenu::IDLE:
        break;
    case StartMenu::POPOUT:
        m_menuManager->SetState(MenuManager::STATE::POPOUT);
        break;
    case StartMenu::POPIN:
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
