//--------------------------------------------------------------------------------------
// File: MenuManager.h
//
// メニュー管理
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
	// コンストラクタ
	MenuManager(
		const wchar_t* baseTextureName,
		DirectX::SimpleMath::Vector2 position
	);
	// デストラクタ
	~MenuManager();
	// 初期化
	void Initialize(int width, int height);
	// 更新
	void Update();
	// 描画
	void Render();

	// メニューを追加
	void Add(const wchar_t* name
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, ANCHOR anchor);

	// 選択中のメニュー番号を取得
	unsigned int GetMenuIndex() { return m_menuIndex; }

public:

	// ポップアニメーション
	void Pop();

	// 状態設定
	void SetState(const STATE& state) { m_state = state; }
	// 状態を取得
	const STATE& GetState() { return m_state; }



private:

	// ポップイン
	void PopIn();
	// ポップアウト
	void PopOut();

private:

	// 選択中のメニュー番号
	unsigned int m_menuIndex;
	// UI
	std::vector<std::unique_ptr<UserInterface>> m_userInterface;
	// ベース
	std::vector<std::unique_ptr<UserInterface>> m_base;

	// ベース画像
	const wchar_t* m_baseTextureName;

	// 効果音
	DirectX::SoundEffectInstance* m_selectSound;

	// キーガイドのポジション
	DirectX::SimpleMath::Vector2 m_position;

	// アニメーション時間
	float m_time;

	// 状態
	STATE m_state;

	// アニメーションパラメータ
	nlohmann::json m_popAnimation;

};

#endif // MENU_MANAGER_DEFINED