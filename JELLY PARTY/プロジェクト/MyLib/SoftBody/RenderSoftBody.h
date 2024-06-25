//--------------------------------------------------------------------------------------
// File: RenderSoftBody.cpp
//
// ソフトボディの描画クラス
//
// Last Update: 2023.10.20
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef RENDER_SOFTBODY_DEFINED
#define RENDER_SOFTBODY_DEFINED

#include <DeviceResources.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <vector>

class RenderSoftBody
{
public:

	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	lightPosition;
	};

	struct ShadowConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
	};
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

public:

	// コンストラクタ
	RenderSoftBody();

	// デストラクタ
	~RenderSoftBody();

	// 初期化
	void Create(ID3D11ShaderResourceView* texture);

	// 更新
	void Update(const std::vector<DirectX::SimpleMath::Vector3>& verticesPos);

	// 描画
	void Render();

	// 影描画
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

	// 頂点バッファを作成
	void CreateBuffer(const std::vector<DirectX::VertexPositionColorTexture> vertices);

	// インデックス番号を計算する
	void CalculateIndex(const int& XNUM, const int& YNUM);

	// クローン
	void Clone(RenderSoftBody* other, const std::vector<DirectX::VertexPositionColorTexture> vertices)
	{
		other->SetIndices(m_indices);
		other->SetTexture(m_texture);
		other->SetInputLayout(m_inputLayout.Get());
		other->SetShaders(
			m_pixelShader.Get(), m_vertexShader.Get(),
			m_pixelShadowShader.Get(), m_vertexShadowShader.Get());
		other->CreateCloneBuffer(vertices);
	}

public: // セッター

	// インデックスを設定
	void SetIndices(const std::vector<uint16_t>& indices) { m_indices = indices; }
	// テクスチャを設定
	void SetTexture(ID3D11ShaderResourceView* texture) { m_texture = texture; }
	// 色を設定
	void SetColor(const DirectX::SimpleMath::Color& color);
	// インプットレイアウトを設定
	void SetInputLayout(ID3D11InputLayout* inputLayout) { m_inputLayout = inputLayout; }
	// シェーダーを設定
	void SetShaders(
		ID3D11PixelShader* pixel, ID3D11VertexShader* vertex, 
		ID3D11PixelShader* pixelSM, ID3D11VertexShader* vertexSM
	);
	// バッファを設定
	void CreateCloneBuffer(const std::vector<DirectX::VertexPositionColorTexture> vertices);


public: // ゲッター

	// インデックスを取得
	const std::vector<uint16_t>& GetIndices() { return m_indices; }

private:

	// 描画設定
	void RenderSetting(ID3D11DeviceContext1* context);

	// 影の描画設定
	void RenderShadowSetting(ID3D11DeviceContext1* context);

	// シェーダー作成
	void CreateShader();

private:
	DX::DeviceResources* m_pDR;

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// 定数バッファ(シャドウマップ用)
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_sCBuffer;

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_VBuffer;

	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_IBuffer;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// テクスチャハンドル
	ID3D11ShaderResourceView* m_texture;

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// 頂点シェーダ（シャドウマップ用）
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShadowShader;

	// ピクセルシェーダ（シャドウマップ用）
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShadowShader;

	// 頂点
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;

	// 三角ポリゴンのインデックス
	std::vector<uint16_t> m_indices;
};

#endif	//RENDER_SOFTBODY_DEFINED
