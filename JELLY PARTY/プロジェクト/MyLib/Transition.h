//--------------------------------------------------------------------------------------
// File: Transition.h
//
// トランジションクラス
//
// Date: 2023/07/30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef TRANSITION_DEFINED
#define TRANSITION_DEFINED

class Transition
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 slidePos;
	};

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
	// トランジション状態
	enum class STATE
	{
		IDLE,
		SLIDEIN,
		SLIDEOUT
	};

	// スライドの速度
	static const float TRANS_SPEED;

public:

	// コンストラクタ
	Transition();
	// デストラクタ
	~Transition();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Render();

	// 状態を設定
	void SetState(STATE state)	{ m_state = state; }

	// 状態を取得
	STATE GetState() const		{ return m_state; }

	void Restart();
private:

	// 画像のスライドイン
	void SlideIn(const float time, float elapsedTime);

	// 画像のスライドアウト
	void SlideOut(const float time, float elpasedTime);

private:
	// 状態
	STATE m_state;

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	DirectX::VertexPositionColorTexture m_vertex;

	DirectX::SimpleMath::Vector4 m_slideRatio;

};
#endif // TRANSITION_DEFINED
