//--------------------------------------------------------------------------------------
// File: ShadowMap.h
//
// トランジションクラス
//
// Date: 2023/07/30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef SHADOWMAP_DEFINED
#define SHADOWMAP_DEFINED

#include "RenderTexture.h"
#include "DepthStencil.h"
#include "PostProcess.h"

#include "Scene/GameMainObjects/Objects/CatBall/CatSphere.h"
#include "Scene/GameMainObjects/Managers/StageFloorManager.h"
#include "Scene/GameMainObjects/Managers/StageSoftBodyManager.h"
#include "Scene/GameMainObjects/Light.h"

class ShadowMap
{
public:

	// コンストラクタ
	ShadowMap();
	// デストラクタ
	~ShadowMap();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Render(
		CatSphere* pSphere,
		StageFloorManager* pStageFloorManager,
		StageSoftBodyManager* pSoftBodyManager
	);

private:

	// デバイスリソース
	DX::DeviceResources* m_pDR;

	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

private:

	// シャドウマップのサイズ
	static const int SHADOWMAP_SIZE = 2048;

	// シャドウマップ用（レンダーテクスチャ）
	std::unique_ptr<DX::RenderTexture> m_shadowMapRT;

	// シャドウマップ用（デプスステンシル）
	std::unique_ptr<DepthStencil> m_shadowMapDS;

	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS_Depth;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Depth;

	// ライトの位置
	DirectX::SimpleMath::Vector3 m_lightPosition;

	// ライトの回転
	DirectX::SimpleMath::Quaternion m_lightRotate;

private:

	// 定数バッファの構造体
	struct ConstantBuffer
	{
		DirectX::XMMATRIX lightViewProj;	// ライトの投影空間へ座標変換する行列
		DirectX::XMVECTOR lightPosition;	// ライトの位置
		DirectX::XMVECTOR lightAmbient;		// ライトの環境光
	};

	// 定数バッファへのポインタ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// 頂点シェーダー
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;

	// サンプラー(シャドウマップ参照用)
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_shadowMapSampler;

	// ライト
	std::unique_ptr<Light> m_light;

};
#endif // SHADOWMAP_DEFINED
