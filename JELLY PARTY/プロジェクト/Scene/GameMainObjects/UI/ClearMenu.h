//--------------------------------------------------------------------------------------
// File: ClearMenu.h
//
// �N���A���̃��j���[
//
// Date: 2023/07/29
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef CLEAR_MENU_DEFINED
#define CLEAR_MENU_DEFINED

#include "MyLib/UserInterface.h"
#include "MyLib/MenuManager.h"
#include <vector>

class ClearMenu
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
		RESTART,
		MENU,
	};

public:

	// �R���X�g���N�^
	ClearMenu(int width, int height);

	// �f�X�g���N�^
	~ClearMenu();

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

#endif // CLEAR_MENU_DEFINED