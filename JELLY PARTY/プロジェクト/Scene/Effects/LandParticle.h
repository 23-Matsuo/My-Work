//--------------------------------------------------------------------------------------
// File: LandParticle.h
//
// 着地パーティクルクラス
//
// 
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef LAND_PARTICLE_DEFINED
#define LAND_PARTICLE_DEFINED

#include "StepTimer.h"
#include <vector>
#include <list>

#include "MyLib/ParticleUtility.h"

class LandParticle
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

	enum class State
	{
		IDLE,
		PLAY
	};

public:
	// インプットレイアウト
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// コンストラクタ
	LandParticle();

	// デストラクタ
	~LandParticle();

	// 初期化
	void Create(DX::DeviceResources* pDR);

	// 更新
	void Update(const DX::StepTimer& timer);

	// 描画
	void Render();

	// ビルボード作成
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 up);

	void SetState(State state) { m_state = state; }
	State GetState() { return m_state; }

	// パーティクル作成
	void CreateLandParticle(DirectX::SimpleMath::Vector3 position);

private:

	void CreateShader();

private:

	DX::DeviceResources* m_pDR;

	State m_state;

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
	std::list<ParticleUtility> m_particleUtility;

	DirectX::SimpleMath::Vector3 m_cameraPosition;
	DirectX::SimpleMath::Vector3 m_cameraTarget;

};

#endif // LAND_PARTICLE_DEFINED