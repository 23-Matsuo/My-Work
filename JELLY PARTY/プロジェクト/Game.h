//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "ImaseLib/DebugFont.h"
#include "MyLib/SceneManager.h"
#include "Audio.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

private:
    
    // 画面モード（TRUE:フルスクリーン)
    BOOL m_fullscreen;

    // シーンマネジャーへのポインタ
    std::unique_ptr<SceneManager> m_sceneManager;

    // キーボードステートトラッカー
    DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;

    // マウスステートトラッカー
    DirectX::Mouse::ButtonStateTracker m_mouseTracker;

    // 共通ステートへのポインタ
    std::unique_ptr<DirectX::CommonStates> m_states;

    // デバッグ用文字列表示オブジェクトへのポインタ
    std::unique_ptr<Imase::DebugFont> m_debugFont;

    // オーディオエンジンへのポインタ
    std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
    // ウェーブバンクへのポインタ
    std::unique_ptr<DirectX::WaveBank> m_waveBank;

    // スプライトバッチへのポインタ
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

    // プリミティブバッチ(VPCT)へのポインタ
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveVPCT;
    // プリミティブバッチ(VPT)へのポインタ
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveVPT;
    // プリミティブバッチ(VPT)へのポインタ
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveVPC;

    // ベーシックエフェクトへのポインタ
    std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
    // インプットレイアウトへのポインタ
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

public:

    // 画面モードを変更する関数（TRUE:フルスクリーン）
    void SetFullscreenState(BOOL value)
    {
        m_fullscreen = value;
        m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);
        if (value) m_deviceResources->CreateWindowSizeDependentResources();
    }

};
