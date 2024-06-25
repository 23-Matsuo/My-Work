//--------------------------------------------------------------------------------------
// File: UserInterface.cpp
//
// ユーザーインターフェースクラス
//
// Last Update: 2023.10.30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "UserInterface.h"

#include "BinaryFile.h"
#include "Resources.h"
#include "Graphics.h"
using namespace DirectX;

//------------------------------------------
// インプットレイアウト
//------------------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC> UserInterface::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3)+ sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const DirectX::SimpleMath::Vector2 UserInterface::DEFAULT_SCALE = { 0.5f,0.5f };

//------------------------------------------
// コンストラクタ
//------------------------------------------
UserInterface::UserInterface()
	: 
	m_pDR(nullptr)
	, m_textureHeight(0)
	, m_textureWidth(0)
	, m_texture(nullptr)
	, m_scale(SimpleMath::Vector2::One)
	, m_position(SimpleMath::Vector2::Zero)
	, m_color(DirectX::Colors::White)
	, m_anchor(ANCHOR::TOP_LEFT)
	, m_renderRatio(1.0f)
{
	RECT windowSize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_windowHeight = windowSize.bottom;
	m_windowWidth = windowSize.right;
}

//------------------------------------------
// デストラクタ
//------------------------------------------
UserInterface::~UserInterface()
{
}

//------------------------------------------
// テクスチャ読み込み
//------------------------------------------
void UserInterface::LoadTexture(const wchar_t* name)
{
	// テクスチャ取得
	Resources::TextureTable texture = Resources::GetInstance()->GetTexture(name);
	// テクスチャ情報設定
	m_texture = texture.texture;
	m_textureWidth = texture.textureWidth / 2;
	m_textureHeight = texture.textureHeight / 2;

}

//------------------------------------------
// 生成関数
//------------------------------------------
void UserInterface::Create(
	const wchar_t* name
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, ANCHOR anchor)
{
	m_position = position;
	m_baseScale = DEFAULT_SCALE;
	m_scale = scale;
	m_anchor = anchor;

	//シェーダーの作成
	CreateShader();

	//画像の読み込み
	LoadTexture(name);
}

//------------------------------------------
// Shader作成
//------------------------------------------
void UserInterface::CreateShader()
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/UIVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/UIGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/UIPS.cso");

	//インプットレイアウトの作成
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

//------------------------------------------
// 描画関数
//------------------------------------------
void UserInterface::Render()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();
	RECT windowSize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

		// 頂点情報
		// Position.xy	:アンカー座標(ピクセル指定:1280 ×720)
		// Position.z	:アンカータイプ(0～8)の整数で指定
		// Color.xy　	:色
		// Tex.xy		:ウィンドウサイズ（バッファも同じ。こちらは未使用）

	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(
		SimpleMath::Vector3(m_position.x, m_position.y, static_cast<float>(m_anchor))
		, m_color
		, SimpleMath::Vector2(m_renderRatio, 0))
	};

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.windowSize = SimpleMath::Vector4(static_cast<float>(windowSize.right), static_cast<float>(windowSize.bottom), 1, 1);
	cbuff.scale = m_scale;
	cbuff.textureSize = SimpleMath::Vector2(static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight));

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// カリングは左周り
	context->RSSetState(states->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	batch->Begin();
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}