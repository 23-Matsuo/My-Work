//--------------------------------------------------------------------------------------
// File: MainMenuText.h
//
// メニューの文字クラス
//
// Date: 2023/12/02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef MAIN_MENU_TEXT_DIFINED
#define MAIN_MENU_TEXT_DIFINED

class MainMenuText
{
public:

	static const DirectX::SimpleMath::Vector2 SCALE;

	enum STATE
	{
		IDLE,
		POPOUT,
		POPIN,
	};
public:
	// コンストラクタ
	MainMenuText(const float& width, const float& height);

	// デストラクタ
	~MainMenuText();

	// 更新
	bool Update(const float& elapsedTime);

	// 描画
	void Render(); 

	void SetPosition(const DirectX::SimpleMath::Vector2& position)	{ m_position = position; }
	void SetState(const STATE& state) { m_state = state; }

	const DirectX::SimpleMath::Vector2& GetPosition()				{ return m_position; }
	const STATE& GetState() { return m_state; }

private:

	// バネの動き
	void Damping(float* scale, float* velocity, float defaultScale);

private:

	// 状態
	STATE m_state;

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

	// 位置
	DirectX::SimpleMath::Vector2 m_position;

	// 加速度
	float m_acceleration;

	// 重力
	float m_gravity;

	// 画像の大きさ
	DirectX::SimpleMath::Vector2 m_scale;

	// バネの力
	DirectX::SimpleMath::Vector2 m_damping;

	// アニメーション時間
	float m_time;

	// 起動時アニメーションフラグ
	bool m_animated;

	float m_alpha;
};

#endif // MAIN_MENU_TEXT_DIFINED
