//--------------------------------------------------------------------------------------
// File: MenuManager.cpp
//
// メニュー管理
//
// Last Update: 2023/10/29
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "MenuManager.h"

#include "MyLib/BinaryFile.h"
#include "MyLib/Resources.h"
#include "MyLib/Graphics.h"
#include "MyLib/SoundUtility.h"
using namespace DirectX;

//------------------------------------------
// コンストラクタ
//------------------------------------------
MenuManager::MenuManager(
    const wchar_t* baseTextureName,
    DirectX::SimpleMath::Vector2 position
)
    : 
    m_menuIndex(0)
    , m_baseTextureName(baseTextureName)
    , m_selectSound(nullptr)
    , m_position(position)
    , m_state(IDLE)
    , m_time(0.0f)
{
    m_userInterface.clear();
    m_base.clear();

    m_selectSound = Resources::GetInstance()->GetSound(L"Select");

    // アニメーションパラメータを読み込む
    Resources::GetInstance()->ReadJsonFile(&m_popAnimation, L"Resources/Datas/StartMenuAnimation");
}

//------------------------------------------
// デストラクタ
//------------------------------------------
MenuManager::~MenuManager()
{
    m_selectSound->Stop();
}


//------------------------------------------
// 初期化
//------------------------------------------
void MenuManager::Initialize(int width,int height)
{
    UNREFERENCED_PARAMETER(width);
    UNREFERENCED_PARAMETER(height);
}

//------------------------------------------
// 更新
//------------------------------------------
void MenuManager::Update()
{
    float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
    auto key = Graphics::GetInstance()->GetKeyboardStateTracker();
   
    // メニュー選択
    if (key->pressed.Up || key->pressed.W)
    {
        m_menuIndex += 1;
        m_menuIndex %= m_userInterface.size();

        PlaySound(m_selectSound, false);
    }
    if (key->pressed.Down || key->pressed.S)
    {
        m_menuIndex += static_cast<unsigned int>(m_userInterface.size()) - 1;
        m_menuIndex %= m_userInterface.size();

        PlaySound(m_selectSound, false);
    }

    // 画像を戻す
    for (int i = 0; i < m_userInterface.size(); i++)
    {
        m_userInterface[i]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    }
    // 画像を戻す
    for (int i = 0; i < m_userInterface.size(); i++)
    {
        m_base[i]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    }

    // 選択中のメニューの色を変更
    DirectX::SimpleMath::Vector4 color;
    color.x = 255.0f / (255.0f + 20.0f + 147.0f) * 1.5f;
    color.y = 20.0f / (255.0f + 20.0f + 147.0f) * 1.5f;
    color.z = 137.0f / (255.0f + 20.0f + 147.0f) * 1.5f;
    color.w = 1.0f;
    m_base[m_menuIndex]->SetColor(color);
}


//------------------------------------------
// 描画
//------------------------------------------
void MenuManager::Render()
{
    for (int i = 0;i < m_userInterface.size();i++)
    {
        m_base[i]->Render();
        m_userInterface[i]->Render();
    }

    //m_keyGuide->Render();
}

//------------------------------------------
// UIに追加
//------------------------------------------
void MenuManager::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, ANCHOR anchor)
{

    std::unique_ptr<UserInterface> userInterface = std::make_unique<UserInterface>();
    userInterface->Create(
        path
        , position
        , scale
        , anchor);

    m_userInterface.push_back(std::move(userInterface));

    std::unique_ptr<UserInterface> base = std::make_unique<UserInterface>();
    base->Create(
        m_baseTextureName
        , position
        , scale
        , anchor);

    m_base.push_back(std::move(base));

}
//------------------------------------------
// ポップアニメーション
//------------------------------------------
void MenuManager::Pop()
{
    float speed = static_cast<float>(m_popAnimation["ANIMATION_SPEED"]);

    switch (m_state)
    {
    case MenuManager::IDLE:
        m_state = IDLE;
        break;
    case MenuManager::POPOUT:
        m_time += speed;
        PopOut();
        break;
    case MenuManager::POPIN:
        m_time += speed;
        PopIn();
        break;
    default:
        break;
    }
}

//------------------------------------------
// ポップアウト
//------------------------------------------
void MenuManager::PopOut()
{
    DirectX::SimpleMath::Vector2 baseScale;
    DirectX::SimpleMath::Vector2 uiScale;
    
    baseScale = m_base[0]->GetScale();
    uiScale = m_userInterface[0]->GetScale();

    float scaleChangeAmount = 0;
    DirectX::SimpleMath::Vector2 newScale;

    if (m_time > m_popAnimation["DURATION"])
    {
        m_time = 0.0f;
        baseScale = {};
        uiScale   = {};
        m_state = IDLE;
    }
    else if (m_time > m_popAnimation["POP_OUT"]["KEY2"]["FRAME"])
    {
        scaleChangeAmount = m_popAnimation["POP_OUT"]["KEY2"]["AMOUNT"];
        newScale = { scaleChangeAmount, scaleChangeAmount };

        baseScale -= newScale;
        uiScale   -= newScale;
    }
    else if (m_time > m_popAnimation["POP_OUT"]["KEY1"]["FRAME"])
    {
        scaleChangeAmount = m_popAnimation["POP_OUT"]["KEY1"]["AMOUNT"];
        newScale = { scaleChangeAmount, scaleChangeAmount };

        baseScale += newScale;
        uiScale   += newScale;
    }

    for (int i = 0; i < m_userInterface.size(); i++)
    {
        m_base[i]->SetScale(baseScale);
        m_userInterface[i]->SetScale(uiScale);
    }

}

//------------------------------------------
// ポップイン
//------------------------------------------
void MenuManager::PopIn()
{
    DirectX::SimpleMath::Vector2 baseScale;
    DirectX::SimpleMath::Vector2 uiScale;
    
    baseScale = m_base[0]->GetScale();
    uiScale = m_userInterface[0]->GetScale();

    float scaleChangeAmount = 0;
    DirectX::SimpleMath::Vector2 newScale;

    if (m_time > m_popAnimation["DURATION"])
    {
        baseScale = DirectX::SimpleMath::Vector2(UserInterface::DEFAULT_SCALE.x, UserInterface::DEFAULT_SCALE.y);
        uiScale = DirectX::SimpleMath::Vector2(UserInterface::DEFAULT_SCALE.x, UserInterface::DEFAULT_SCALE.y);
        m_state = IDLE;
        m_time = 0.0f;
    }
    else if (m_time > m_popAnimation["POP_IN"]["KEY2"]["FRAME"])
    {
        scaleChangeAmount = m_popAnimation["POP_IN"]["KEY2"]["AMOUNT"];
        newScale = { scaleChangeAmount, scaleChangeAmount };

        baseScale -= newScale;
        uiScale   -= newScale;
    }
    else if (m_time > m_popAnimation["POP_IN"]["KEY1"]["FRAME"])
    {
        scaleChangeAmount = m_popAnimation["POP_IN"]["KEY1"]["AMOUNT"];
        newScale = { scaleChangeAmount, scaleChangeAmount };

        baseScale += newScale;
        uiScale   += newScale;
    }

    for (int i = 0; i < m_userInterface.size(); i++)
    {
        m_base[i]->SetScale(baseScale);
        m_userInterface[i]->SetScale(uiScale);
    }
}
