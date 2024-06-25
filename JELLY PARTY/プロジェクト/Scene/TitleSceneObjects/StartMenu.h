//--------------------------------------------------------------------------------------
// File: StartMenu.h
//
// タイトルのメニュー
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

	// 状態
	enum STATE
	{
		IDLE,
		POPOUT,
		POPIN,
	};

	// 選択したボタン
	enum SelectedButton
	{
		PLAY,
		QUIT,
	};

public:
	// コンストラクタ
	StartMenu(int width, int height);
	// デストラクタ
	~StartMenu();
	// 更新
	void Update();
	// 描画
	void Render();

	// ポップアニメーション
	void Pop();

	// 状態設定
	void SetState(const STATE& state) { m_state = state; }
	// 状態を取得
	const STATE& GetState() { return m_state; }

	// メニューマネージャーを取得
	MenuManager* GetMenuManager() { return m_menuManager.get(); }

private:
	
	// メニューマネージャへのポインタ
	std::unique_ptr<MenuManager> m_menuManager;

	// メニューの状態
	STATE m_state;
};

#endif // START_MENU_DEFINED
