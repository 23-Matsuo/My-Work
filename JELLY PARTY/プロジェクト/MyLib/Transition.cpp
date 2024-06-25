//--------------------------------------------------------------------------------------
// File: Transition.cpp
//
// トランジションクラス
//
// Last Update: 2023/10/12
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Transition.h"
#include "Resources.h"
#include "Graphics.h"
#include "MyLib/BinaryFile.h"

//------------------------------------------
// インプットレイアウト
//------------------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC> Transition::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const float Transition::TRANS_SPEED = 2.0f;

//------------------------------------------
// コンストラクタ
//------------------------------------------
Transition::Transition()
	:
	m_vertex{}
	, m_state(STATE::SLIDEOUT)
	, m_slideRatio(DirectX::SimpleMath::Vector4::One)
{
}

//------------------------------------------
// デストラクタ
//------------------------------------------
Transition::~Transition()
{

}

//------------------------------------------
// 初期化
//------------------------------------------
void Transition::Initialize()
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/TransitionVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/TransitionGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/TransitionPS.cso");
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
// 更新
//------------------------------------------
void Transition::Update()
{
	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	
	m_slideRatio.Clamp(DirectX::SimpleMath::Vector4::Zero, DirectX::SimpleMath::Vector4::One);


	// 状態に応じて更新
	switch (m_state)
	{
	case STATE::IDLE:
		break;
	case STATE::SLIDEIN:
		SlideIn(TRANS_SPEED, elapsedTime);
		break;
	case STATE::SLIDEOUT:
		SlideOut(TRANS_SPEED, elapsedTime);
		break;
	}
}

//------------------------------------------
// 描画
//------------------------------------------
void Transition::Render()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();
	//auto spbatch = Resources::GetInstance()->GetSpriteBatch();

	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_vertex =
	{
		DirectX::SimpleMath::Vector3::One,
		DirectX::SimpleMath::Vector4::One,
		DirectX::SimpleMath::Vector2::One
	};


	//シェーダーに渡す追加のバッファを作成
	ConstBuffer cbuff;
	cbuff.slidePos = m_slideRatio;

	//半透明描画指定
	ID3D11BlendState* blendstate = states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// カリングは左周り
	context->RSSetState(states->CullNone());

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	batch->Begin();
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertex, 1);
	batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void Transition::Restart()
{
	m_slideRatio = DirectX::SimpleMath::Vector4::One;
	m_state = STATE::SLIDEOUT;
}

//------------------------------------------
// スライドイン
//------------------------------------------
void Transition::SlideIn(const float time, float elapsedTime)
{
	m_slideRatio.y += time * elapsedTime;
	// 経過時間が移動にかかる時間を超えているか
	if (m_slideRatio.y >= 1.0f)
	{
		// ステートを待ち状態に
		m_state = STATE::IDLE;
	}
}

//------------------------------------------
// スライドアウト
//------------------------------------------
void Transition::SlideOut(const float time, float elapsedTime)
{
	m_slideRatio.y -= time * elapsedTime;

	// 経過時間が移動にかかる時間を超えているか
	if (m_slideRatio.y <= 0.0f)
	{
		// ステートを待ち状態
		m_state = STATE::IDLE;
	}
}

