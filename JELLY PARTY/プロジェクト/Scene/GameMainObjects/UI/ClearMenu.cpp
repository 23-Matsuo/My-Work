//--------------------------------------------------------------------------------------
// File: ClearMenu.cpp
//
// クリア時のメニュー
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
// コンストラクタ
//------------------------------------------
ClearMenu::ClearMenu(int width, int height)
    :
    m_state(ClearMenu::STATE::IDLE)
{
    UNREFERENCED_PARAMETER(height);

    // メニューの位置

    m_menuManager = std::make_unique<MenuManager>(
        L"ButtonBase", DirectX::SimpleMath::Vector2(static_cast<float>(width - width / 5), static_cast<float>(height / 2 + 200)));


    // 「もう一度」
    m_menuManager->Add(L"ReStart"
        , SimpleMath::Vector2(static_cast<float>(width - width / 5), static_cast<float>(height / 2 + 100))
        , SimpleMath::Vector2(0.0f, 0.0f)
        , ANCHOR::MIDDLE_CENTER);

    // 「メニューへ」
    m_menuManager->Add(L"ToMenu"
        , SimpleMath::Vector2(static_cast<float>(width - width / 5), static_cast<float>(height / 2 + 200))
        , SimpleMath::Vector2(0.0f, 0.0f)
        , ANCHOR::MIDDLE_CENTER);
}

//------------------------------------------
// デストラクタ
//------------------------------------------
ClearMenu::~ClearMenu()
{
    m_menuManager.reset();
}

//------------------------------------------
// 更新
//------------------------------------------
void ClearMenu::Update()
{
    m_menuManager->Update();
}

//------------------------------------------
// 描画
//------------------------------------------
void ClearMenu::Render()
{
    m_menuManager->Render();
}

//------------------------------------------
// ポップアニメーション
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
