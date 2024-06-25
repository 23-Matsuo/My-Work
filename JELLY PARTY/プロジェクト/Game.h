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
    
    // ��ʃ��[�h�iTRUE:�t���X�N���[��)
    BOOL m_fullscreen;

    // �V�[���}�l�W���[�ւ̃|�C���^
    std::unique_ptr<SceneManager> m_sceneManager;

    // �L�[�{�[�h�X�e�[�g�g���b�J�[
    DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;

    // �}�E�X�X�e�[�g�g���b�J�[
    DirectX::Mouse::ButtonStateTracker m_mouseTracker;

    // ���ʃX�e�[�g�ւ̃|�C���^
    std::unique_ptr<DirectX::CommonStates> m_states;

    // �f�o�b�O�p������\���I�u�W�F�N�g�ւ̃|�C���^
    std::unique_ptr<Imase::DebugFont> m_debugFont;

    // �I�[�f�B�I�G���W���ւ̃|�C���^
    std::unique_ptr<DirectX::AudioEngine> m_audioEngine;
    // �E�F�[�u�o���N�ւ̃|�C���^
    std::unique_ptr<DirectX::WaveBank> m_waveBank;

    // �X�v���C�g�o�b�`�ւ̃|�C���^
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

    // �v���~�e�B�u�o�b�`(VPCT)�ւ̃|�C���^
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveVPCT;
    // �v���~�e�B�u�o�b�`(VPT)�ւ̃|�C���^
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveVPT;
    // �v���~�e�B�u�o�b�`(VPT)�ւ̃|�C���^
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveVPC;

    // �x�[�V�b�N�G�t�F�N�g�ւ̃|�C���^
    std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
    // �C���v�b�g���C�A�E�g�ւ̃|�C���^
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

public:

    // ��ʃ��[�h��ύX����֐��iTRUE:�t���X�N���[���j
    void SetFullscreenState(BOOL value)
    {
        m_fullscreen = value;
        m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);
        if (value) m_deviceResources->CreateWindowSizeDependentResources();
    }

};
