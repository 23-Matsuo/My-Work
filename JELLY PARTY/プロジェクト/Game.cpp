//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "Scene/TitleScene.h"
#include "Scene/PlayScene.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
    : m_fullscreen(FALSE)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
 
    // オーディオエンジンの作成
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags |= AudioEngine_Debug;
#endif
    m_audioEngine = std::make_unique<AudioEngine>(eflags);
    m_waveBank = std::make_unique<WaveBank>(m_audioEngine.get(), L"Resources/Sounds/Audio.xwb");
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    

    // 起動シーン設定
    m_sceneManager->SetScene<TitleScene>();
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    UNREFERENCED_PARAMETER(timer);
    // TODO: Add your game logic here.
    // オーディオエンジンの更新
    if (!m_audioEngine->Update())
    {
        // No audio device is active
        if (m_audioEngine->IsCriticalError())
        {
            OutputDebugString(L"AudioEngine Error!\n");
        }
    }

    // キーボードの更新
    auto key = Graphics::GetInstance()->GetKeyboardStateTracker();
    auto keyboard = Keyboard::Get().GetState();
    key->Update(keyboard);

    // シーンの更新
    m_sceneManager->Update();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    context;

    // シーンの描画
    m_sceneManager->Render();

    // fpsの表示
    std::wostringstream oss;
    oss << "FPS:" << m_timer.GetFramesPerSecond();
    //m_debugFont->AddString(oss.str().c_str(), SimpleMath::Vector2(0.0f, 0.0f), DirectX::Colors::Black);

    // デバッグ用文字列の描画
    m_debugFont->Render(m_states.get());

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);

    // フルスクリーンか調べる
    BOOL fullscreen = FALSE;
    m_deviceResources->GetSwapChain()->GetFullscreenState(&fullscreen, nullptr);
    // フルスクリーンが解除されてしまった時の処理
    if (m_fullscreen != fullscreen)
    {
        m_fullscreen = fullscreen;
        // ResizeBuffers関数を呼び出す
        m_deviceResources->CreateWindowSizeDependentResources();
    }
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 1280;
    height = 720;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Initialize device dependent objects here (independent of window size).

    // 共通ステートオブジェクトの作成
    m_states = std::make_unique<CommonStates>(device);

    // デバッグ用文字列表示オブジェクトの作成
    m_debugFont = std::make_unique<Imase::DebugFont>(device, context, L"Resources/Font/SegoeUI_18.spritefont");
    // スプライトバッチの作成
    m_spriteBatch = std::make_unique<SpriteBatch>(context);
    // プリミティブバッチの作成
    m_primitiveVPCT = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
    // プリミティブバッチの作成
    m_primitiveVPT = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);
    // プリミティブバッチの作成
    m_primitiveVPC = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);
    // ベーシックエフェクトの作成
    m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);

    // 入力レイアウト生成 
    void const* shaderByteCode;
    size_t byteCodeLength;
    m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    device->CreateInputLayout(
        VertexPositionColor::InputElements,
        VertexPositionColor::InputElementCount,
        shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf()
    );

    // シーンマネージャーの作成
    if (!m_sceneManager) m_sceneManager =
        std::make_unique<SceneManager>();

    // リソースをロード
    Resources::GetInstance()->LoadResources(device);
    Resources::GetInstance()->LoadSounds(m_waveBank.get());
    // シーンへ渡すリソースの設定
    Graphics::GetInstance()->SetDeviceResources(m_deviceResources.get());
    Graphics::GetInstance()->SetKeyboardStateTracker(&m_keyboardTracker);
    Graphics::GetInstance()->SetMouseStateTracker(&m_mouseTracker);
    Graphics::GetInstance()->SetStepTimerStates(&m_timer);
    Graphics::GetInstance()->SetDebugFont(m_debugFont.get());
    Graphics::GetInstance()->SetCommonStates(m_states.get());
    Graphics::GetInstance()->SetSpriteBatch(m_spriteBatch.get());
    Graphics::GetInstance()->SetPrimitiveBatch(m_primitiveVPCT.get());
    Graphics::GetInstance()->SetPrimitiveBatch(m_primitiveVPT.get());
    Graphics::GetInstance()->SetPrimitiveBatch(m_primitiveVPC.get());
    Graphics::GetInstance()->SetBasicEffect(m_basicEffect.get());
    Graphics::GetInstance()->SetInputLayout(m_inputLayout.Get());

    // 実行中のシーンのCreateDeviceDependentResources関数を呼び出す
    m_sceneManager->CreateDeviceDependentResources();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

    // 実行中のシーンのCreateWindowSizeDependentResources関数を呼び出す
    m_sceneManager->CreateWindowSizeDependentResources();
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    // 実行中のシーンのOnDeviceLost関数を呼び出す
    m_sceneManager->OnDeviceLost();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
