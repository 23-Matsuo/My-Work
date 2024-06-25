//--------------------------------------------------------------------------------------
// File: ShadowMap.cpp
//
// トランジションクラス
//
// Last Update: 2023/10/12
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "ShadowMap.h"
#include "ReadData.h"
#include "BinaryFile.h"
#include <iomanip>

//------------------------------------------
// コンストラクタ
//------------------------------------------
ShadowMap::ShadowMap()
	:
	m_pDR(nullptr)
{
}

//------------------------------------------
// デストラクタ
//------------------------------------------
ShadowMap::~ShadowMap()
{

}

//------------------------------------------
// 初期化
//------------------------------------------
void ShadowMap::Initialize()
{
	m_pDR = Graphics::GetInstance()->GetDeviceResources();

	auto device = m_pDR->GetD3DDevice();

	RECT rect = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE };

	// レンダーテクスチャの作成（シャドウマップ用）
	m_shadowMapRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
	m_shadowMapRT->SetDevice(device);
	m_shadowMapRT->SetWindow(rect);

	// デプスステンシルの作成（シャドウマップ用）
	m_shadowMapDS = std::make_unique<DepthStencil>(DXGI_FORMAT_D32_FLOAT);
	m_shadowMapDS->SetDevice(device);
	m_shadowMapDS->SetWindow(rect);

	BinaryFile vs_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_VS_Depth.cso");
	BinaryFile ps_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_PS_Depth.cso");
	BinaryFile VS = BinaryFile::LoadFile(L"Resources/Shaders/SM_VS.cso");
	BinaryFile PS = BinaryFile::LoadFile(L"Resources/Shaders/SM_PS.cso");

	// 頂点シェーダーの作成（シャドウマップ用）
	if (FAILED(device->CreateVertexShader(vs_depth.GetData(), vs_depth.GetSize(), NULL, m_VS_Depth.ReleaseAndGetAddressOf())))
	{
		// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダーの作成（シャドウマップ用）
	if (FAILED(device->CreatePixelShader(ps_depth.GetData(), ps_depth.GetSize(), NULL, m_PS_Depth.ReleaseAndGetAddressOf())))
	{
		// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
	// 頂点シェーダーの作成（シャドウマップ用）
	if (FAILED(device->CreateVertexShader(VS.GetData(), VS.GetSize(), NULL, m_VS.ReleaseAndGetAddressOf())))
	{
		// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダーの作成（シャドウマップ用）
	if (FAILED(device->CreatePixelShader(PS.GetData(), PS.GetSize(), NULL, m_PS.ReleaseAndGetAddressOf())))
	{
		// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// 定数バッファの作成
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer));	// 確保するサイズ（16の倍数で設定する）
	// GPU (読み取り専用) と CPU (書き込み専用) の両方からアクセスできるリソース
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 定数バッファとして扱う
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPUが内容を変更できるようにする
	DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf()));

	// サンプラーの作成
	D3D11_SAMPLER_DESC sampler_desc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
	sampler_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS;
	device->CreateSamplerState(&sampler_desc, m_shadowMapSampler.ReleaseAndGetAddressOf());

	// ライトの作成
	m_light = std::make_unique<Light>();

	// ライトの位置
	m_lightPosition = m_light->GetPosition();

	// ライトの回転
	m_lightRotate = m_light->GetRotation();
}

//------------------------------------------
// 更新
//------------------------------------------
void ShadowMap::Update()
{
}

//------------------------------------------
// 描画
//------------------------------------------
void ShadowMap::Render(
	CatSphere* pSphere,
	StageFloorManager* pStageFloorManager,
	StageSoftBodyManager* pSoftBodyManager
)
{
	auto context = m_pDR->GetD3DDeviceContext();

	auto states = Graphics::GetInstance()->GetCommonStates();

	// -------------------------------------------------------------------------- //
	// Pass1 シャドウマップの作成
	// -------------------------------------------------------------------------- //

	auto rtv = m_shadowMapRT->GetRenderTargetView();
	auto srv = m_shadowMapRT->GetShaderResourceView();
	auto dsv = m_shadowMapDS->GetDepthStencilView();

	// レンダーターゲットを変更（shadowMapRT）
	context->ClearRenderTargetView(rtv, DirectX::SimpleMath::Color(1.0f, 0.0f, 1.0f, 1.0f));
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->OMSetRenderTargets(1, &rtv, dsv);

	// ビューポートを設定
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
	context->RSSetViewports(1, &vp);

	m_view = Graphics::GetInstance()->GetViewMatrix();
	m_projection = Graphics::GetInstance()->GetProjectionMatrix();

	// ------------------------------------------------ //
	// ライト空間のビュー行列と射影行列を作成する
	// ------------------------------------------------ //

	// ライトのビュー行列を作成
	DirectX::SimpleMath::Matrix lightView = m_light->GetView();

	// ライトの射影行列を作成
	DirectX::SimpleMath::Matrix lightProjection = m_light->GetProjection();

	// -------------------------------------------------------------------------- //
	// 定数バッファを更新
	// -------------------------------------------------------------------------- //

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// GPUが定数バッファに対してアクセスを行わないようにする
	context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// 定数バッファを更新
	ConstantBuffer cb = {};
	DirectX::SimpleMath::Matrix lightViewProjection = lightView * lightProjection;
	cb.lightViewProj = DirectX::XMMatrixTranspose(lightViewProjection);
	cb.lightPosition = m_lightPosition;
	cb.lightAmbient = m_light->GetAmbient();

	*static_cast<ConstantBuffer*>(mappedResource.pData) = cb;

	// GPUが定数バッファに対してのアクセスを許可する
	context->Unmap(m_constantBuffer.Get(), 0);

	// ------------------------------------------------ //
	// 影になるモデルを描画する
	// ------------------------------------------------ //

	// ゼリーの影描画
	pSoftBodyManager->ShadowRender(lightView, lightProjection);

	// プレイヤーの描画
	pSphere->Render(context, states, lightView, lightProjection, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// -------------------------------------------------------------------------- //
	// レンダーターゲットとビューポートを元に戻す
	// -------------------------------------------------------------------------- //
	auto renderTarget = m_pDR->GetRenderTargetView();
	auto depthStencil = m_pDR->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	auto const viewport = m_pDR->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// ステージ床の描画
	pStageFloorManager->Render(context, states, m_view, m_projection, [&]()
		{
			// 定数バッファの設定
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 1, cbuf);

			// 作成したシャドウマップをリソースとして設定
			context->PSSetShaderResources(1, 1, &srv);

			// テクスチャサンプラーの設定
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// シェーダーの設定
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// プレイヤーの描画
	pSphere->Render(context, states, m_view, m_projection, [&]()
		{
		}
	);

	// ゼリー描画
	pSoftBodyManager->Render();

	// リソースの割り当てを解除する（shadowMapRT）
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, nullsrv);

	// 球のエフェクトを描画
	pSphere->RenderEffect();
}