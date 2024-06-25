//--------------------------------------------------------------------------------------
// File: GetParticle.cpp
//
// パーティクルクラス
//
// Last Update: 2023.10.02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "GetParticle.h"

#include "MyLib/BinaryFile.h"
#include <algorithm>
#include <random>

//-----------------------------------
// インプットレイアウト
//-----------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC> GetParticle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

//-----------------------------------
// コンストラクタ
//-----------------------------------
GetParticle::GetParticle()
	: m_pDR(nullptr)
	, m_state(State::IDLE)
{
}

//-----------------------------------
// デストラクタ
//-----------------------------------
GetParticle::~GetParticle()
{
}

//-----------------------------------
// 生成関数
//-----------------------------------
void GetParticle::Create(DX::DeviceResources* pDR)
{	
	m_pDR = pDR;

	//シェーダーの作成
	CreateShader();
}

//-----------------------------------
// パーティクル作成
//-----------------------------------
void GetParticle::CreateGetParticle(
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Color color
)
{
	const int PARTICLE_NUM = 8;
	const float RADIUS = 3.5f;


	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		float angle = i * (360.0f / PARTICLE_NUM);
		float x = RADIUS * std::cos(angle * DirectX::XM_PI / 180.0f);
		float z = RADIUS * std::sin(angle * DirectX::XM_PI / 180.0f);
		ParticleUtility pU(
			0.3f,															// 生存時間
			position,														// 基準座標
			DirectX::SimpleMath::Vector3(x * 2, 0.0f, z * 2),				// 速度
			DirectX::SimpleMath::Vector3(-x * 2, -10.0f, -z * 2),			// 加速度
			DirectX::SimpleMath::Vector3(0.5f, 0.5f, 1.0f),					// 初期スケール
			DirectX::SimpleMath::Vector3(0.1f, 0.1f, 1.0f),					// 最終スケール
			DirectX::SimpleMath::Color(color.x, color.y, color.z, 0.8f),	// 初期カラー
			DirectX::SimpleMath::Color(color.x, color.y, color.z, 0.0f)		// 最終カラー
		);

		m_particleUtility.push_front(pU);
	}
}

//-----------------------------------
// 更新
//-----------------------------------
void GetParticle::Update(const DX::StepTimer& timer)
{
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	if (m_state != State::PLAY) return;
	//timerを渡してm_effectの更新処理を行う
	for (std::list<ParticleUtility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)
	{
		// 子クラスからfalseで消す
		if (!(ite)->Update(elapsedTime))
		{
			m_particleUtility.clear();
			m_state = State::IDLE;
			break;
		}
	}
}


//-----------------------------------
// Shader作成
//-----------------------------------
void GetParticle::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");
	
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
void GetParticle::Render()
{
	if (m_state != State::PLAY) return;

	auto context = m_pDR->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();
	DirectX::SimpleMath::Matrix view = Graphics::GetInstance()->GetViewMatrix();
	DirectX::SimpleMath::Matrix projection = Graphics::GetInstance()->GetProjectionMatrix();

	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
			{
				//カメラ正面の距離でソート
				return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) >
					cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
			});
	//登録されている頂点をリセット
	m_vertices.clear();
	for (auto& pU : m_particleUtility)
	{
		if (cameraDir.Dot(pU.GetPosition() - m_cameraPosition) < 0.0f) {
			//内積がマイナスの場合はカメラの後ろなので表示する必要なし
			continue;
		}
		DirectX::VertexPositionColorTexture vPCT;
		vPCT.position = DirectX::XMFLOAT3(pU.GetPosition());
		vPCT.color = pU.GetNowColor();
		vPCT.textureCoordinate = DirectX::XMFLOAT2(pU.GetNowScale().x, pU.GetNowScale().y );
			
		m_vertices.push_back(vPCT);
	}
		
	//表示する点がない場合は描画を終わる
	if (m_vertices.empty())
	{
		return;
	}

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = projection.Transpose();
	cbuff.matWorld = m_billboard.Transpose();
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

	// カリングは左周り
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
 	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());
	batch->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

//-----------------------------------
/// ビルボード作成関数
//-----------------------------------
void GetParticle::CreateBillboard(
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

