//--------------------------------------------------------------------------------------
// File: PushSpace\.h
//
// タイトル背景クラス
//
// Date: 2023/12/02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef PUSH_SPACE_DIFINED
#define PUSH_SPACE_DIFINED

class PushSpace
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
	PushSpace(const float& width, const float& height);

	// デストラクタ
	~PushSpace();

	// 更新
	void Update(const float& elapsedTime);

	// 描画
	void Render(); 

	// ポップアニメーション
	void Pop(const float& elapsedTime);

	// 状態設定
	void SetState(const STATE& state) { m_state = state; }
	// 大きさ設定
	void SetScale(const DirectX::SimpleMath::Vector2& scale) { m_scale = scale; }

	// 状態を取得
	const STATE& GetState() { return m_state; }

private:

	// ポップアウト
	void PopOut();
	// ポップイン
	void PopIn();

private:

	// 画像の状態
	STATE m_state;

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

	// 位置
	DirectX::SimpleMath::Vector2 m_position;

	// 画像のスケール
	DirectX::SimpleMath::Vector2 m_scale;

	// アニメーション時間
	float m_time;

	// アニメーションパラメータ
	nlohmann::json m_popAnimation;

	// 画像の初期の大きさ
	DirectX::SimpleMath::Vector2 m_defaultScale;

};

#endif // PUSH_SPACE_DIFINED
