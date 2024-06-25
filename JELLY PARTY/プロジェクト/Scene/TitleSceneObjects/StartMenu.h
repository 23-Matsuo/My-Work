//--------------------------------------------------------------------------------------
// File: StartMenu.h
//
// �^�C�g���̃��j���[
//
// Date: 2023/07/26
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef START_MENU_DEFINED
#define START_MENU_DEFINED

#include "MyLib/UserInterface.h"
#include "MyLib/MenuManager.h"

class StartMenu
{
public:

	// ���
	enum STATE
	{
		IDLE,
		POPOUT,
		POPIN,
	};

	// �I�������{�^��
	enum SelectedButton
	{
		PLAY,
		QUIT,
	};

public:
	// �R���X�g���N�^
	StartMenu(int width, int height);
	// �f�X�g���N�^
	~StartMenu();
	// �X�V
	void Update();
	// �`��
	void Render();

	// �|�b�v�A�j���[�V����
	void Pop();

	// ��Ԑݒ�
	void SetState(const STATE& state) { m_state = state; }
	// ��Ԃ��擾
	const STATE& GetState() { return m_state; }

	// ���j���[�}�l�[�W���[���擾
	MenuManager* GetMenuManager() { return m_menuManager.get(); }

private:
	
	// ���j���[�}�l�[�W���ւ̃|�C���^
	std::unique_ptr<MenuManager> m_menuManager;

	// ���j���[�̏��
	STATE m_state;
};

#endif // START_MENU_DEFINED
