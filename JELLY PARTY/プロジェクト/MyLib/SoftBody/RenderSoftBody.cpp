//--------------------------------------------------------------------------------------
// File: RenderSoftBody.cpp
//
// ソフトボディの描画クラス
//
// Last Update: 2023.10.20
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "RenderSoftBody.h"
#include "../BinaryFile.h"

//------------------------------------------
// インプットレイアウト
//------------------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC> RenderSoftBody::INPUT_LAYOUT =
{
	// セマンティック名, 番号, フォーマット, スロット, オフセットバイト値, スロットクラス, インスタンスの数
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

//------------------------------------------
// コンストラクタ
//------------------------------------------
RenderSoftBody::RenderSoftBody()
	:
	m_pDR(nullptr)
	, m_texture(nullptr)
	, m_IBuffer(nullptr)
	, m_indices{}
	, m_vertices{}
{
}

//------------------------------------------
// デストラクタ
//------------------------------------------
RenderSoftBody::~RenderSoftBody()
{
	m_pDR = nullptr;
	m_texture = nullptr;
	m_IBuffer = nullptr;
	m_indices.clear();
	m_vertices.clear();
}

//------------------------------------------
// 初期化
//------------------------------------------
void RenderSoftBody::Create(ID3D11ShaderResourceView* texture)
{
	m_pDR = Graphics::GetInstance()->GetDeviceResources();

	CreateShader();

	m_texture = texture;
}

//------------------------------------------
// 更新
//------------------------------------------
void RenderSoftBody::Update(const std::vector<DirectX::SimpleMath::Vector3>& verticesPos)
{
	for (int i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].position = verticesPos[i];
	}
}

//------------------------------------------
// 面1枚を描画
//------------------------------------------
void RenderSoftBody::Render()
{
	auto context = m_pDR->GetD3DDeviceContext();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();
	
	DirectX::SimpleMath::Matrix world;
	DirectX::SimpleMath::Matrix view = Graphics::GetInstance()->GetViewMatrix();
	DirectX::SimpleMath::Matrix projection = Graphics::GetInstance()->GetProjectionMatrix();

	//シェーダーに渡すバッファを作成
	ConstBuffer cbuff;
	cbuff.matWorld = world.Transpose();
	cbuff.matView = view.Transpose();
	cbuff.matProj = projection.Transpose();
	cbuff.lightPosition = DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, nullptr, &cbuff, 0, 0);

	RenderSetting(context);

	batch->Begin();
	
	batch->DrawIndexed(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		m_indices.data(), m_indices.size(),			// インデックス情報
		m_vertices.data(), m_vertices.size()		// 頂点情報
	);
	
	batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}


//------------------------------------------
// 描画の設定
//------------------------------------------
void RenderSoftBody::RenderSetting(ID3D11DeviceContext1* context)
{
	auto states = Graphics::GetInstance()->GetCommonStates();

	//---------------------------
	// 定数バッファを渡す
	//---------------------------
	ID3D11Buffer* cb[] = { m_sCBuffer.Get(), m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 2, cb);
	context->PSSetConstantBuffers(0, 2, cb);

	//---------------------------
	// 頂点バッファを渡す
	//---------------------------
	UINT stride = sizeof(DirectX::VertexPositionColorTexture);
	UINT offset = 0;
	ID3D11Buffer* vb[1] = { m_VBuffer.Get() };
	context->IASetVertexBuffers(0, 1, vb, &stride, &offset);

	//---------------------------
	// インデックスバッファを渡す
	//---------------------------
	context->IASetIndexBuffer(m_IBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = states->AlphaBlend();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに読み取りを参照する
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// カリングは左周り
	// にしたいけど変更に時間が掛かるからカリングなし
	context->RSSetState(states->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, &m_texture);

}

//------------------------------------------
// 面1枚の影描画
//------------------------------------------
void RenderSoftBody::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	auto context = m_pDR->GetD3DDeviceContext();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();

	DirectX::SimpleMath::Matrix world;
	//シェーダーに渡すバッファを作成
	ShadowConstBuffer scbuff;
	scbuff.matWorld = world.Transpose();
	scbuff.matView = lightView.Transpose();
	scbuff.matProj = lightProjection.Transpose();

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_sCBuffer.Get(), 0, nullptr, &scbuff, 0, 0);

	RenderShadowSetting(context);

	batch->Begin();

	batch->DrawIndexed(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		m_indices.data(), m_indices.size(),			// インデックス情報
		m_vertices.data(), m_vertices.size()		// 頂点情報
	);

	batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

//------------------------------------------
// 影描画の設定
//------------------------------------------
void RenderSoftBody::RenderShadowSetting(ID3D11DeviceContext1* context)
{
	auto states = Graphics::GetInstance()->GetCommonStates();

	//---------------------------
	// 定数バッファを渡す
	//---------------------------
	ID3D11Buffer* cb[1] = { m_sCBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//---------------------------
	// 頂点バッファを渡す
	//---------------------------
	UINT stride = sizeof(DirectX::VertexPositionColorTexture);
	UINT offset = 0;
	ID3D11Buffer* vb[1] = { m_VBuffer.Get() };
	context->IASetVertexBuffers(0, 1, vb, &stride, &offset);

	//---------------------------
	// インデックスバッファを渡す
	//---------------------------
	context->IASetIndexBuffer(m_IBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = states->AlphaBlend();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに読み取りを参照する
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// カリングは左周り
	// にしたいけど変更に時間が掛かるからカリングなし
	context->RSSetState(states->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_vertexShadowShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShadowShader.Get(), nullptr, 0);

	//ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, &m_texture);
}


//------------------------------------------
// Shader作成部分だけ分離した関数
//------------------------------------------
void RenderSoftBody::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/JellyVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/JellyPS.cso");
	BinaryFile SM_VSData = BinaryFile::LoadFile(L"Resources/Shaders/JellySM_VS.cso");
	BinaryFile SM_PSData = BinaryFile::LoadFile(L"Resources/Shaders/JellySM_PS.cso");

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

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(SM_VSData.GetData(), SM_VSData.GetSize(), NULL, m_vertexShadowShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(SM_PSData.GetData(), SM_PSData.GetSize(), NULL, m_pixelShadowShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.ByteWidth = sizeof(ConstBuffer);					// バッファのサイズ(頂点数)
	cbd.Usage = D3D11_USAGE_DEFAULT;						// 使用方法
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;				// バッファの種類
	cbd.CPUAccessFlags = 0;									// CPUアクセス設定
	device->CreateBuffer(&cbd, nullptr, &m_CBuffer);

	//シェーダーにデータを渡すためのコンスタントバッファ生成（シャドウマップ用）
	D3D11_BUFFER_DESC scbd;
	ZeroMemory(&scbd, sizeof(scbd));
	scbd.ByteWidth = sizeof(ShadowConstBuffer);					// バッファのサイズ(頂点数)
	scbd.Usage = D3D11_USAGE_DEFAULT;						// 使用方法
	scbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;				// バッファの種類
	scbd.CPUAccessFlags = 0;									// CPUアクセス設定
	device->CreateBuffer(&scbd, nullptr, &m_sCBuffer);
}

//------------------------------------------
// バッファを作成
//------------------------------------------
void RenderSoftBody::CreateBuffer(const std::vector<DirectX::VertexPositionColorTexture> vertices)
{
	auto device = m_pDR->GetD3DDevice();

	// 頂点バッファを作成
	D3D11_BUFFER_DESC vbd = {};
	// バッファサイズ分のメモリをすべて初期化
	//ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = static_cast<UINT>(sizeof(DirectX::VertexPositionColorTexture) * vertices.size());
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 初期データを設定
	D3D11_SUBRESOURCE_DATA vbdInitData = {};
	vbdInitData.pSysMem = vertices.data();
	// エラーハンドリング
	if (FAILED(device->CreateBuffer(&vbd, &vbdInitData, &m_VBuffer)))
	{
		MessageBox(0, L"CreatVertexBuffer failed.", NULL, MB_OK);
	}
	// 頂点情報をコピー
	m_vertices = vertices;


	// インデックスバッファを作成
	D3D11_BUFFER_DESC ibd = {};
	//ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = static_cast<UINT>(sizeof(uint16_t) * m_indices.size());
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 初期データを設定
	D3D11_SUBRESOURCE_DATA ibdInitData = {};
	ibdInitData.pSysMem = m_indices.data();
	// エラーハンドリング
	if (FAILED(device->CreateBuffer(&ibd, &ibdInitData, &m_IBuffer)))
	{
		MessageBox(0, L"CreatIndexBuffer failed.", NULL, MB_OK);
	}
}

//------------------------------------------
// インデックスを計算
//------------------------------------------
void RenderSoftBody::CalculateIndex(const int& XNUM, const int& YNUM)
{
	int total = XNUM * YNUM;
	// 現在の行数
	int row = 1;

	for (int i = 0; i < total - XNUM - 1; ++i)
	{
		// 四角形を構成する頂点インデックス
		uint16_t p[4];

		// iが端になったら
		// 次の行に更新
		if (i == (XNUM * row) - 1)
		{
			i++; row++;
		}
		
		// 頂点位置を計算
		p[0] = static_cast<uint16_t>(i);			p[1] = static_cast<uint16_t>(i + 1);
		p[2] = static_cast<uint16_t>(i + XNUM);		p[3] = static_cast<uint16_t>(i + XNUM + 1);

		// 0->1->2, 1->2->3の順
		m_indices.push_back(p[0]);
		m_indices.push_back(p[1]);
		m_indices.push_back(p[2]);

		m_indices.push_back(p[1]);
		m_indices.push_back(p[2]);
		m_indices.push_back(p[3]);
	}
}

//------------------------------------------
// 色を設定
//------------------------------------------
void RenderSoftBody::SetColor(const DirectX::SimpleMath::Color& color)
{
	for (int i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].color = color;
	}
}

//------------------------------------------
// シェーダーを設定
//------------------------------------------
void RenderSoftBody::SetShaders(
	ID3D11PixelShader* pixel, ID3D11VertexShader* vertex, 
	ID3D11PixelShader* pixelSM, ID3D11VertexShader* vertexSM
)
{
	m_pixelShader = pixel;
	m_vertexShader = vertex;
	m_pixelShadowShader = pixelSM;
	m_vertexShadowShader = vertexSM;
}

//------------------------------------------
// バッファを複製
//------------------------------------------
void RenderSoftBody::CreateCloneBuffer(const std::vector<DirectX::VertexPositionColorTexture> vertices)
{
	m_pDR = Graphics::GetInstance()->GetDeviceResources();
	auto device = m_pDR->GetD3DDevice();

	// 頂点バッファを作成
	D3D11_BUFFER_DESC vbd = {};
	// バッファサイズ分のメモリをすべて初期化
	//ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = static_cast<UINT>(sizeof(DirectX::VertexPositionColorTexture) * vertices.size());
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 初期データを設定
	D3D11_SUBRESOURCE_DATA vbdInitData = {};
	vbdInitData.pSysMem = vertices.data();
	// エラーハンドリング
	if (FAILED(device->CreateBuffer(&vbd, &vbdInitData, &m_VBuffer)))
	{
		MessageBox(0, L"CreatVertexBuffer failed.", NULL, MB_OK);
	}
	// 頂点情報をコピー
	m_vertices = vertices;

	// インデックスバッファを作成
	D3D11_BUFFER_DESC ibd = {};
	//ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = static_cast<UINT>(sizeof(uint16_t) * m_indices.size());
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 初期データを設定
	D3D11_SUBRESOURCE_DATA ibdInitData = {};
	ibdInitData.pSysMem = m_indices.data();
	// エラーハンドリング
	if (FAILED(device->CreateBuffer(&ibd, &ibdInitData, &m_IBuffer)))
	{
		MessageBox(0, L"CreatIndexBuffer failed.", NULL, MB_OK);
	}

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.ByteWidth = sizeof(ConstBuffer);					// バッファのサイズ(頂点数)
	cbd.Usage = D3D11_USAGE_DEFAULT;						// 使用方法
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;				// バッファの種類
	cbd.CPUAccessFlags = 0;									// CPUアクセス設定
	device->CreateBuffer(&cbd, nullptr, &m_CBuffer);

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC scbd;
	ZeroMemory(&scbd, sizeof(scbd));
	scbd.ByteWidth = sizeof(ShadowConstBuffer);					// バッファのサイズ(頂点数)
	scbd.Usage = D3D11_USAGE_DEFAULT;							// 使用方法
	scbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;				// バッファの種類
	scbd.CPUAccessFlags = 0;									// CPUアクセス設定
	device->CreateBuffer(&scbd, nullptr, &m_sCBuffer);

}
