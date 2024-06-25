//--------------------------------------------------------------------------------------
// File: ClearMenu.h
//
// クリア時のメニュー
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
		RESTART,
		MENU,
	};

public:

	// コンストラクタ
	ClearMenu(int width, int height);

	// デストラクタ
	~ClearMenu();

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

#endif // CLEAR_MENU_DEFINED