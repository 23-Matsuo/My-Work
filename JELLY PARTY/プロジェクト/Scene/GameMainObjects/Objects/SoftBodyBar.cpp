//--------------------------------------------------------------------------------------
// File: SoftBodyBar.h
//
// 回転するバー
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "SoftBodyBar.h"
#include "MyLib/ObjectCollision.h"
#include "MyLib/SoftBody/SoftBodyFactory.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
SoftBodyBar::SoftBodyBar(const std::vector<DirectX::SimpleMath::Vector3>& jointPoints)
{
	auto deviceResources = Graphics::GetInstance()->GetDeviceResources();
	auto device = deviceResources->GetD3DDevice();
	auto context = deviceResources->GetD3DDeviceContext();

	// テクスチャを設定
	m_texture = Resources::GetInstance()->GetTexture(L"Jelly").texture;
	// 衝突判定の表示オブジェクトの作成
	m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);

	// パラメータを読み込む
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/JellyBarParameter");

	// 頂点の数データ
	for (const auto& num : m_parameters["POINT_NUM"])
	{
		m_pointXNum = num[0];
		m_pointYNum = num[1];
		m_pointZNum = num[2];
	}
	// 位置データ
	DirectX::SimpleMath::Vector3 initPos;
	for (const auto& position : m_parameters["INITIAL_POSITION"])
	{
		initPos = { position[0], position[1], position[2] };
	}


	// 繋げる頂点の位置をメンバ変数に代入
	m_jointPoints = jointPoints;

	// ボックスの作成
	m_box[0] = SoftBodyFactory::CreateBox(
		m_pointXNum, m_pointYNum, m_pointZNum,
		initPos,
		{},
		DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
		GetColor(COLORS::MAGENTA),
		m_texture.Get(),
		PhysicalPoint::STATE::BAR
	);

	m_box[1] = std::make_unique<SoftBodyBox>();
	m_box[0]->Clone(m_box[1].get());

	// 180度回転させたものを反対側に作る
	DirectX::SimpleMath::Matrix rotation =
		DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XM_PI - DirectX::XMConvertToRadians(22.5f));
	m_box[1]->Rotate(rotation);


	// 頂点の状態を設定
	SetBarPointsState();

	//-----頂点をジョイントさせる-----//
	JointPoints();


	m_hitParticle = std::make_unique<HitParticle>();
	m_hitParticle->Create(deviceResources);

}

//------------------------------------------
// デストラクタ
//------------------------------------------
SoftBodyBar::~SoftBodyBar()
{
	m_box[0].reset();
	m_box[1].reset();
}

//------------------------------------------
// 更新
//------------------------------------------
void SoftBodyBar::Update(
	DirectX::SimpleMath::Matrix rotation,
	SoftBodyCylinder* pCylinder,
	const float& anglarVelocity
)
{
	auto timer = Graphics::GetInstance()->GetStepTimer();

	DirectX::SimpleMath::Vector3 hitPosition;

	for (int i = 0; i < BAR_NUM; i++)
	{
		// 回す
		m_box[i]->Rotate(rotation);

		// ソフトボディの更新
		m_box[i]->Update(*timer);

		// 衝突判定
		if (ObjectCollision::GetInstance()->CatToBarPoints(
			m_box[i].get(), pCylinder, anglarVelocity, &hitPosition))
		{
			// 当たった時のエフェクト生成
			m_hitParticle->CreateHitParticle(hitPosition, m_box[i]->GetColor());
			m_hitParticle->SetState(HitParticle::State::PLAY);
		}
	}

	m_hitParticle->Update(*timer);
}

//------------------------------------------
// 描画
//------------------------------------------
void SoftBodyBar::Render()
{

	for (int i = 0; i < BAR_NUM; i++)
	{
		// ソフトボディテクスチャ描画
		m_box[i]->Render();
	}

	// パーティクルをビルボードさせて描画
	m_hitParticle->CreateBillboard(DirectX::SimpleMath::Vector3::Up);
	m_hitParticle->Render();

}

//------------------------------------------
// 影描画
//------------------------------------------
void SoftBodyBar::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	for (int i = 0; i < BAR_NUM; i++)
	{
		// ソフトボディ影描画
		m_box[i]->ShadowRender(lightView, lightProjection);
	}

}

//------------------------------------------
// 色の取得
//------------------------------------------
DirectX::SimpleMath::Color SoftBodyBar::GetColor(COLORS color)
{
	DirectX::SimpleMath::Vector4 sbbColor;
	sbbColor.w = 1.0f;
	switch (color)
	{
	case COLORS::CYAN:
		sbbColor.x = 0.0f;
		sbbColor.y = 191.0f / (0.0f + 191.0f + 255.0f) * 1.5f;
		sbbColor.z = 255.0f / (0.0f + 191.0f + 255.0f) * 1.5f;
		break;
	case COLORS::MAGENTA:
		sbbColor.x = 255.0f / (255.0f + 20.0f + 147.0f) * 1.5f;
		sbbColor.y = 20.0f / (255.0f + 20.0f + 147.0f) * 1.5f;
		sbbColor.z = 137.0f / (255.0f + 20.0f + 147.0f) * 1.5f;
		break;
	case COLORS::YELLOW:
		sbbColor.x = 1.0f;
		sbbColor.y = 1.0f;
		sbbColor.z = 0.0f;
		break;
	default:
		break;
	}
	return sbbColor;
}

//------------------------------------------
// バーの頂点の状態を設定
//------------------------------------------
void SoftBodyBar::SetBarPointsState()
{
	for (int i = 0; i < BAR_NUM; i++)
	{
		SoftBodyBox::Points& points = *m_box[i]->GetPoints();

		for (PhysicalPoint& point : points)
		{
			// 全てバーの状態にする
			point.SetState(PhysicalPoint::STATE::BAR);
		}
	}
}

//------------------------------------------
// 中心のソフトボディと頂点繋げる
//------------------------------------------
void SoftBodyBar::JointPoints()
{
	// 右側の頂点の設定を行う
	SoftBodyBox::Points& pointsA = *m_box[0]->GetPoints();

	for (int i = 0; i < pointsA.size(); i += m_pointXNum)
	{
		pointsA[i].SetState(PhysicalPoint::STATE::FIXED);
	}
	// 左下
	pointsA[m_pointXNum * m_pointYNum].SetPosition(m_jointPoints[0]);
	// 右下
	pointsA[0].SetPosition(m_jointPoints[1]);
	// 左上
	pointsA[m_pointXNum * m_pointYNum * m_pointZNum - m_pointXNum].SetPosition(m_jointPoints[2]);
	// 右上
	pointsA[m_pointXNum * m_pointYNum - m_pointXNum].SetPosition(m_jointPoints[3]);

	// 左側の頂点の設定を行う
	SoftBodyBox::Points& pointsB = *m_box[1]->GetPoints();

	for (int i = 0; i < pointsB.size(); i += m_pointXNum)
	{
		pointsB[i].SetState(PhysicalPoint::STATE::FIXED);
	}
	// 左下
	pointsB[m_pointXNum * m_pointYNum].SetPosition(m_jointPoints[5]);
	// 右下
	pointsB[0].SetPosition(m_jointPoints[4]);
	// 左上
	pointsB[m_pointXNum * m_pointYNum * m_pointZNum - m_pointXNum].SetPosition(m_jointPoints[7]);
	// 右上
	pointsB[m_pointXNum * m_pointYNum - m_pointXNum].SetPosition(m_jointPoints[6]);

}

