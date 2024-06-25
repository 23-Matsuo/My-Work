//--------------------------------------------------------------------------------------
// File: CenterSoftBody.h
//
// 回転する中心のオブジェクト
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "CenterSoftBody.h"

#include "MyLib/ObjectCollision.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
CenterSoftBody::CenterSoftBody()
{
	auto deviceResources = Graphics::GetInstance()->GetDeviceResources();
	auto device = deviceResources->GetD3DDevice();
	auto context = deviceResources->GetD3DDeviceContext();
	// 衝突判定の表示オブジェクトの作成
	m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);

	// テクスチャを設定
	m_texture = Resources::GetInstance()->GetTexture(L"Jelly").texture;

	// パラメータを読み込む
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/JellyCenterParameter");

	// 頂点の数データ
	for (const auto& num : m_parameters["POINT_NUM"])
	{
		m_pointXNum = num[0];
		m_pointYNum = num[1];
	}
	// 位置データ
	DirectX::SimpleMath::Vector3 initPos;
	for (const auto& position : m_parameters["INITIAL_POSITION"])
	{
		initPos = { position[0], position[1], position[2] };
	}
	// 半径
	const float RADIUS = static_cast<float>(m_parameters["RADIUS"]);
	// 高さ
	const float HEIGHT = static_cast<float>(m_parameters["HEIGHT"]);

	// シリンダーを作成
	m_cylinder = SoftBodyFactory::CreateCylinder(
		m_pointXNum, m_pointYNum,
		RADIUS,
		HEIGHT,
		initPos,
		{},
		DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
		DirectX::SimpleMath::Color(0.9f, 0.25f, 0.25f, 1.0f),
		m_texture.Get()
	);

	m_collision[0] = DirectX::BoundingSphere(DirectX::SimpleMath::Vector3(0.0f, HEIGHT - 0.5f, 0.0f), RADIUS - 0.2f);
	m_collision[1] = DirectX::BoundingSphere(DirectX::SimpleMath::Vector3(0.0f, HEIGHT + 0.8f, 0.0f), RADIUS - 0.2f);

	// ジョイント作成
	CreateJoint();

	m_hitParticle = std::make_unique<HitParticle>();
	m_hitParticle->Create(deviceResources);
}

//------------------------------------------
// デストラクタ
//------------------------------------------
CenterSoftBody::~CenterSoftBody()
{
	m_cylinder.reset();
}

//------------------------------------------
// 更新
//------------------------------------------
void CenterSoftBody::Update(DirectX::SimpleMath::Matrix rotation)
{
	auto timer = Graphics::GetInstance()->GetStepTimer();

	// 回す
	m_cylinder->Rotate(rotation);

	// ソフトボディの更新
	m_cylinder->Update(*timer);

	for (int i = 0; i < 2; i++)
	{
		DirectX::SimpleMath::Vector3 hitPosition;
		if (ObjectCollision::GetInstance()->CatToShaft(m_collision[i], m_cylinder.get(), &hitPosition))
		{
			// 当たった時のエフェクト生成
			m_hitParticle->CreateHitParticle(hitPosition, m_cylinder.get()->GetColor());
			m_hitParticle->SetState(HitParticle::State::PLAY);
			break;
		}
	}
	// エフェクト更新
	m_hitParticle->Update(*timer);
}

//------------------------------------------
// 描画
//------------------------------------------
void CenterSoftBody::Render()
{
	// ソフトボディテクスチャ描画
	m_cylinder->Render();

	// パーティクルをビルボードさせて描画
	m_hitParticle->CreateBillboard(DirectX::SimpleMath::Vector3::Up);
	m_hitParticle->Render();
}

//------------------------------------------
// 影描画
//------------------------------------------
void CenterSoftBody::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection)
{
	// ソフトボディ影描画
	m_cylinder->ShadowRender(lightView, lightProjection);

}

//------------------------------------------
// ジョイント作成
//------------------------------------------
void CenterSoftBody::CreateJoint()
{
	//-----バーと繋げる位置を設定-----//
	int PointXNum = m_cylinder->GetXNum();
	int PointYNum = m_cylinder->GetYNum();
	SoftBodyCylinder::Points& points = *m_cylinder->GetPoints();

	// 左下
	m_jointPoints.push_back(points[0].GetPosition());
	points[0].SetState(PhysicalPoint::STATE::FIXED);
	// 右下
	m_jointPoints.push_back(points[PointXNum - 1].GetPosition());
	points[PointXNum - 1].SetState(PhysicalPoint::STATE::FIXED);
	// 左上
	m_jointPoints.push_back(points[PointXNum].GetPosition());
	points[PointXNum].SetState(PhysicalPoint::STATE::FIXED);
	// 右上
	m_jointPoints.push_back(points[PointXNum * PointYNum - 1].GetPosition());
	points[PointXNum * PointYNum - 1].SetState(PhysicalPoint::STATE::FIXED);

	// 左下
	m_jointPoints.push_back(points[PointXNum / 2].GetPosition());
	points[PointXNum / 2].SetState(PhysicalPoint::STATE::FIXED);
	// 右下
	m_jointPoints.push_back(points[PointXNum / 2 + 1].GetPosition());
	points[PointXNum / 2 + 1].SetState(PhysicalPoint::STATE::FIXED); 

	// 左上
	m_jointPoints.push_back(points[PointXNum + PointXNum / 2].GetPosition());
	points[PointXNum + PointXNum / 2].SetState(PhysicalPoint::STATE::FIXED);

	// 右上
	m_jointPoints.push_back(points[PointXNum + PointXNum / 2 + 1].GetPosition());
	points[PointXNum + PointXNum / 2 + 1].SetState(PhysicalPoint::STATE::FIXED);

}