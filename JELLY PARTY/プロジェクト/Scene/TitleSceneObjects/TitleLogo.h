//--------------------------------------------------------------------------------------
// File: TitleLogo.h
//
// タイトルロゴクラス
//
// Date: 2023/12/02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef TITLE_LOGO_DIFINED
#define TITLE_LOGO_DIFINED

class TitleLogo
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
	TitleLogo(const float& width, const float& height);

	// デストラクタ
	~TitleLogo();

	void Initialize();

	// 更新
	bool Update(const float& elapsedTime);

	// 描画
	void Render(); 

	void Pop(const float& elapsedTime);



	void SetPosition(const DirectX::SimpleMath::Vector2& position)	{ m_position = position; }
	void SetScale(const DirectX::SimpleMath::Vector2& scale)	{ m_scale = scale; }
	void SetState(const STATE& state) { m_state = state; }

	const DirectX::SimpleMath::Vector2& GetPosition()				{ return m_position; }
	const STATE& GetState() { return m_state; }

private:

	void PopOut();
	void PopIn();

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

	// アニメーションパラメータ
	nlohmann::json m_popAnimation;

	// 画像の初期の大きさ
	DirectX::SimpleMath::Vector2 m_defaultScale;
};

#endif // TITLE_LOGO_DIFINED
