//--------------------------------------------------------------------------------------
// File: MenuManager.h
//
// ���j���[�Ǘ�
//
// Date: 2023/10/29
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef MENU_MANAGER_DEFINED
#define MENU_MANAGER_DEFINED

#include "StepTimer.h"
#include "MyLib/UserInterface.h"
#include <vector>
#include "Keyboard.h"
#include "Audio.h"

class MenuManager
{
public:
	enum STATE
	{
		IDLE,
		POPOUT,
		POPIN,
	};

public:
	// �R���X�g���N�^
	MenuManager(
		const wchar_t* baseTextureName,
		DirectX::SimpleMath::Vector2 position
	);
	// �f�X�g���N�^
	~MenuManager();
	// ������
	void Initialize(int width, int height);
	// �X�V
	void Update();
	// �`��
	void Render();

	// ���j���[��ǉ�
	void Add(const wchar_t* name
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, ANCHOR anchor);

	// �I�𒆂̃��j���[�ԍ����擾
	unsigned int GetMenuIndex() { return m_menuIndex; }

public:

	// �|�b�v�A�j���[�V����
	void Pop();

	// ��Ԑݒ�
	void SetState(const STATE& state) { m_state = state; }
	// ��Ԃ��擾
	const STATE& GetState() { return m_state; }



private:

	// �|�b�v�C��
	void PopIn();
	// �|�b�v�A�E�g
	void PopOut();

private:

	// �I�𒆂̃��j���[�ԍ�
	unsigned int m_menuIndex;
	// UI
	std::vector<std::unique_ptr<UserInterface>> m_userInterface;
	// �x�[�X
	std::vector<std::unique_ptr<UserInterface>> m_base;

	// �x�[�X�摜
	const wchar_t* m_baseTextureName;

	// ���ʉ�
	DirectX::SoundEffectInstance* m_selectSound;

	// �L�[�K�C�h�̃|�W�V����
	DirectX::SimpleMath::Vector2 m_position;

	// �A�j���[�V��������
	float m_time;

	// ���
	STATE m_state;

	// �A�j���[�V�����p�����[�^
	nlohmann::json m_popAnimation;

};

#endif // MENU_MANAGER_DEFINED