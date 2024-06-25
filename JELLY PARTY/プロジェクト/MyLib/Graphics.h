//--------------------------------------------------------------------------------------
// File: Graphics.h
//
// ���ʃ��\�[�X
//
// Last Update: 2023.10.24
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#pragma once

#ifndef GRAPHICS_DEFINED
#define GRAPHICS_DEFINED

#include "StepTimer.h"
#include "DeviceResources.h"
#include "ImaseLib/DebugFont.h"

// Graphics�N���X���`����
class Graphics
{
public:

	// Graphics�N���X�̃C���X�^���X���擾����
	static Graphics* const GetInstance();
	// �f�X�g���N�^
	~Graphics();

public: // �Z�b�^�[

	// �X�e�b�v�^�C�}�[��ݒ�
	void SetStepTimerStates(DX::StepTimer* timer)												{ m_timer = timer; }
	// �f�o�C�X���\�[�X��ݒ�
	void SetDeviceResources(DX::DeviceResources* deviceResources)								{ m_deviceResources = deviceResources; }
	// �L�[�{�[�h�X�e�[�g�g���b�J�[��ݒ�
	void SetKeyboardStateTracker(DirectX::Keyboard::KeyboardStateTracker* tracker)				{ m_keyboardTracker = tracker; }
	// �}�E�X�X�e�[�g�g���b�J�[��ݒ�
	void SetMouseStateTracker(DirectX::Mouse::ButtonStateTracker* tracker)						{ m_mouseTracker = tracker; }
	// ���ʃX�e�[�g��ݒ�
	void SetCommonStates(DirectX::CommonStates* states)											{ m_states = states; }
	// �X�v���C�g�o�b�`��ݒ�
	void SetSpriteBatch(DirectX::SpriteBatch* spriteBatch)										{ m_spriteBatch = spriteBatch; }
	// �v���~�e�B�u�o�b�`��ݒ�(VPCT)
	void SetPrimitiveBatch(DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* vpct)	{ m_primitiveVPCT = vpct; }
	// �v���~�e�B�u�o�b�`��ݒ�(VPT)
	void SetPrimitiveBatch(DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* vpt)		{ m_primitiveVPT = vpt; }
	// �v���~�e�B�u�o�b�`��ݒ�(VPC)
	void SetPrimitiveBatch(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* vpc)			{ m_primitiveVPC = vpc; }
	// �x�[�V�b�N�G�t�F�N�g��ݒ�
	void SetBasicEffect(DirectX::BasicEffect* effect)											{ m_basicEffect = effect; }
	// �C���v�b�g���C�A�E�g��ݒ�
	void SetInputLayout(ID3D11InputLayout* inputLayout)											{ m_inputLayout = inputLayout; }
	// �f�o�b�O�t�H���g��ݒ�
	void SetDebugFont(Imase::DebugFont* debugFont)												{ m_debugFont = debugFont; }
	// �r���[�s���ݒ�
	void SetViewMatrix(DirectX::SimpleMath::Matrix view)										{ m_view = view; }
	// �ˉe�s���ݒ�
	void SetProjectionMatrix(DirectX::SimpleMath::Matrix projection)							{ m_projection = projection; }
	// �J�����̃^�[�Q�b�g�ʒu��ݒ�
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target)									{ m_cameraTarget = target; }
	// �J�����̖ڂ̈ʒu��ݒ�
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye)											{ m_cameraEye = eye; }

public:	// �Q�b�^�[

	// �X�e�b�v�^�C�}�[���擾
	DX::StepTimer* GetStepTimer()																{ return m_timer; }
	// �f�o�C�X���\�[�X���擾
	DX::DeviceResources* GetDeviceResources()													{ return m_deviceResources; }
	// �L�[�{�[�h�X�e�[�g�g���b�J�[���擾
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardStateTracker()							{ return m_keyboardTracker; }
	// �}�E�X�X�e�[�g�g���b�J�[���擾
	DirectX::Mouse::ButtonStateTracker* GetMouseStateTracker()									{ return m_mouseTracker; }
	// ���ʃX�e�[�g���擾
	DirectX::CommonStates* GetCommonStates()													{ return m_states; }
	// �X�v���C�g�o�b�`���擾
	DirectX::SpriteBatch* GetSpriteBatch()														{ return m_spriteBatch; }
	// �v���~�e�B�u�o�b�`(VPCT)���擾
	DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* GetPrimitiveBatchVPCT()		{ return m_primitiveVPCT; }
	// �v���~�e�B�u�o�b�`(VPT)���擾
	DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* GetPrimitiveBatchVPT()				{ return m_primitiveVPT; }
	// �v���~�e�B�u�o�b�`(VPC)���擾
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetPrimitiveBatchVPC()				{ return m_primitiveVPC; }
	// �x�[�V�b�N�G�t�F�N�g���擾
	DirectX::BasicEffect* GetBasicEffect()														{ return m_basicEffect; }
	// �C���v�b�g���C�A�E�g���擾
	ID3D11InputLayout* GetInputLayout()															{ return m_inputLayout; }
	// �f�o�b�O�t�H���g���擾
	Imase::DebugFont* GetDebugFont()															{ return m_debugFont; }
	// �r���[�s����擾
	const DirectX::SimpleMath::Matrix& GetViewMatrix()											{ return m_view; }
	// �ˉe�s����擾
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix()									{ return m_projection; }
	// �J�����̃^�[�Q�b�g���擾
	const DirectX::SimpleMath::Vector3& GetCameraTarget()										{ return m_cameraTarget; }
	// �J�����̃A�C���擾
	const DirectX::SimpleMath::Vector3& GetCameraEye()											{ return m_cameraEye; }

private:

	// �R���X�g���N�^
	Graphics();
	// ����͋��e���Ȃ�
	void operator=(const Graphics& object) = delete;
	// �R�s�[�R���X�g���N�^�͋��e���Ȃ�
	Graphics(const Graphics& object) = delete;

private:

	// Graphics�N���X�̃C���X�^���X�ւ̃|�C���^
	static std::unique_ptr<Graphics> m_graphics;

private:
	// �X�e�b�v�^�C�}�[
	DX::StepTimer* m_timer;

	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_deviceResources;

	// �L�[�{�[�h�X�e�[�g�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker* m_keyboardTracker;

	// �}�E�X�X�e�[�g�g���b�J�[
	DirectX::Mouse::ButtonStateTracker* m_mouseTracker;

	// �X�e�[�g�ւ̃|�C���^
	DirectX::CommonStates* m_states;

	// �X�v���C�g�o�b�`�ւ̃|�C���^
	DirectX::SpriteBatch* m_spriteBatch;

	// �v���~�e�B�u�o�b�`
	DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* m_primitiveVPCT;
	DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* m_primitiveVPT;
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* m_primitiveVPC;

	// �x�[�V�b�N�G�t�F�N�g
	DirectX::BasicEffect* m_basicEffect;

	// �C���v�b�g���C�A�E�g
	ID3D11InputLayout* m_inputLayout;

	// �f�o�b�O�p������\���ւ̃|�C���^
	Imase::DebugFont* m_debugFont;

	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// �ˉe�s��
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// �ˉe�s��
	DirectX::SimpleMath::Vector3 m_cameraEye;
};

#endif // GRAPHICS_DIFINED