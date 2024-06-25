//--------------------------------------------------------------------------------------
// File: ResultConfetti.cpp
//
// パーティクルクラス
//
// Last Update: 2023.10.02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "ResultConfetti.h"

#include "MyLib/BinaryFile.h"
#include <algorithm>
#include <random>

//-----------------------------------
// インプットレイアウト
//-----------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC> ResultConfetti::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

//-----------------------------------
// コンストラクタ
//-----------------------------------
ResultConfetti::ResultConfetti()
	: m_pDR(nullptr)
	, m_timer(0.0f)
{
}

//-----------------------------------
// デストラクタ
//-----------------------------------
ResultConfetti::~ResultConfetti()
{
}

//-----------------------------------
// 生成関数
//-----------------------------------
void ResultConfetti::Create(DX::DeviceResources* pDR)
{	
	m_pDR = pDR;

	//シェーダーの作成
	CreateShader();
}

//-----------------------------------
// 紙吹雪作成
//-----------------------------------
void ResultConfetti::CreateConfetti(const float& elapsedTime)
{
	// 3.0秒ごとに生成
	m_timer += elapsedTime;
	if (m_timer >= 0.1f)
	{
		std::random_device seed;
		std::default_random_engine engine(seed());
		std::uniform_real_distribution<float> positionDist(-5.0f, 5.0f);
		std::uniform_real_distribution<float> velocityDist(0.5f, 2.0f);
		std::uniform_real_distribution<float> rotateDist(-5.0f, 5.0f);
		std::uniform_real_distribution<float> scaleDist(0.05f, 0.2f);
		std::uniform_int_distribution<> colorDist(0, 6);

		// ランダム速度
		DirectX::SimpleMath::Vector3 velocity(0.0f, velocityDist(engine), 0.0f);
		// ランダム回転
		DirectX::SimpleMath::Vector3 rotation(rotateDist(engine), rotateDist(engine), rotateDist(engine));
		// ランダムスケール
		//DirectX::SimpleMath::Vector3 scale(scaleDist(engine), scaleDist(engine), scaleDist(engine));
		DirectX::SimpleMath::Vector3 scale(0.05f, 0.05f, 0.05f);
		

		float x = static_cast<float>(positionDist(engine));
		float z = static_cast<float>(positionDist(engine));

		// 紙インスタンス作成
		ConfettiUtility confetti(
			10.0f,												// 生存時間
			DirectX::SimpleMath::Vector3(x, 8.0f, z + 5.0f),	// 基準座標
			velocity,											// 速度
			DirectX::SimpleMath::Vector3::Zero,					// 加速度
			rotation,											// 回転
			scale,												// スケール			
			GetColor(colorDist(engine))							// カラー
		);

		m_confettiUtility.push_back(confetti);
		m_timer = 0.0f;
	}
}

//-----------------------------------
// 更新
//-----------------------------------
void ResultConfetti::Update(const float& elapsedTime)
{
	//timerを渡してm_effectの更新処理を行う
	for (std::list<ConfettiUtility>::iterator ite = m_confettiUtility.begin(); ite != m_confettiUtility.end(); ite++)
	{
		// 子クラスからfalseで消す
		if (!(ite)->Update(elapsedTime))
		{
			ite = m_confettiUtility.erase(ite);
			if (ite == m_confettiUtility.end())
			{
				break;
			}
		}
	}
}


//-----------------------------------
// Shader作成
//-----------------------------------
void ResultConfetti::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ConfettiVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ConfettiGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ConfettiPS.cso");
	
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
// 描画
//------------------------------------------
void ResultConfetti::Render()
{
	auto context = m_pDR->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();
	DirectX::SimpleMath::Matrix view = Graphics::GetInstance()->GetViewMatrix();
	DirectX::SimpleMath::Matrix projection = Graphics::GetInstance()->GetProjectionMatrix();

	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	m_confettiUtility.sort(
		[&](ConfettiUtility lhs, ConfettiUtility  rhs)
			{
				//カメラ正面の距離でソート
				return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
			});
	//登録されている頂点をリセット
	m_vertices.clear();
	for (auto& cU : m_confettiUtility)
	{
		if (cameraDir.Dot(cU.GetPosition() - m_cameraPosition) < 0.0f) {
			//内積がマイナスの場合はカメラの後ろなので表示する必要なし
			continue;
		}

		// 回転
		DirectX::SimpleMath::Vector3 rotataion = cU.GetRotation();

		// 位置
		DirectX::VertexPositionColorTexture vPCT;
		vPCT.position = cU.GetPosition();

		// 色情報
		DirectX::SimpleMath::Color color = cU.GetColor();

		// アルファ値はいらないので回転のxを渡す
		color.w = rotataion.x;
	
		// 色情報をセット
		vPCT.color = color;

		// UV座標はいらないので回転角のyzを渡す
		vPCT.textureCoordinate = { rotataion.y, rotataion.z };
		m_vertices.push_back(vPCT);
	}
		
	//表示する点がない場合は描画を終わる
	if (m_vertices.empty())
	{
		return;
	}

	DirectX::SimpleMath::Matrix world;

	//world = m_rotation[0];

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = projection.Transpose();
	cbuff.matWorld = world.Transpose();
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

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

	// カリングはなし
	context->RSSetState(states->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = Resources::GetInstance()->GetTexture(L"Smoke").texture;

	//ピクセルシェーダにテクスチャを登録する。
	context->PSSetShaderResources(0, 1, texture.GetAddressOf());

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	batch->Begin();
	for (int i = 0; i < m_vertices.size(); i++)
	{
		batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[i], 1);
	}
	batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

//-----------------------------------
// ビルボード作成関数
//-----------------------------------
void ResultConfetti::CreateBillboard(
	DirectX::SimpleMath::Vector3 up
)
{
	DirectX::SimpleMath::Vector3 target = Graphics::GetInstance()->GetCameraTarget();
	DirectX::SimpleMath::Vector3 eye = Graphics::GetInstance()->GetCameraEye();

	m_billboard =
		//SimpleMath::Matrix::CreateBillboard(target, eye , up);
		DirectX::SimpleMath::Matrix::CreateBillboard(DirectX::SimpleMath::Vector3::Zero,eye - target, up);

	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::Identity;
	rot._11 = -1;
	rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;
	m_billboard = rot * m_billboard;
}


//-----------------------------------
// 色を取得
//-----------------------------------
const DirectX::SimpleMath::Color ResultConfetti::GetColor(int rnd)
{
	DirectX::SimpleMath::Color color;

	switch (rnd)
	{
	case 0:	// 赤
		color = DirectX::Colors::Red;
		break;
	case 1:	// 緑
		color = DirectX::Colors::Green;
		break;
	case 2:	// 青
		color = DirectX::Colors::Blue;
		break;
	case 3:	// 黄色
		color = DirectX::Colors::Yellow;
		break;
	case 4:	// 紫
		color = DirectX::Colors::Purple;
		break;
	case 5:	// オレンジ
		color = DirectX::Colors::Orange;
		break;
	case 6:	// シアン
		color = DirectX::Colors::Cyan;
		break;
	default:
		break;
	}

	return color;
}
