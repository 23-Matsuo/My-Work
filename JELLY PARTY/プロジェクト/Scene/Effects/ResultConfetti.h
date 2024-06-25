//--------------------------------------------------------------------------------------
// File: ResultConfetti.h
//
// リザルトの紙吹雪演出
//
// 
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef RESULT_CONFETTI_DEFINED
#define RESULT_CONFETTI_DEFINED

#include "StepTimer.h"
#include <vector>
#include <list>

#include "MyLib/ConfettiUtility.h"

class ResultConfetti
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

public:
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// コンストラクタ
	ResultConfetti();

	// デストラクタ
	~ResultConfetti();

	// 初期化
	void Create(DX::DeviceResources* pDR);

	// 更新
	void Update(const float& elapsedTime);

	// 描画
	void Render();

	// ビルボード作成
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 up);

	// 紙吹雪作成
	void CreateConfetti(const float& elapsedTime);

private:

	// シェーダー作成
	void CreateShader();

	// 色を取得
	const DirectX::SimpleMath::Color GetColor(int rnd);

private:

	DX::DeviceResources* m_pDR;

	float m_timer;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	m_geometryShader;

	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	DirectX::SimpleMath::Matrix m_billboard;

	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	std::list<ConfettiUtility> m_confettiUtility;

	DirectX::SimpleMath::Vector3 m_cameraPosition;
	DirectX::SimpleMath::Vector3 m_cameraTarget;

	// 回転
	std::vector<DirectX::SimpleMath::Matrix>  m_rotation;

};

#endif // RUN_PARTICLE_DEFINED