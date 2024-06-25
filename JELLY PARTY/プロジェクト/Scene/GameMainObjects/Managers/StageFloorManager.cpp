//--------------------------------------------------------------------------------------
// File: StageFloorManager.cpp
//
// ステージの床
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "StageFloorManager.h"

#include "MyLib/ObjectCollision.h"

// 床オブジェクトのポジション
const DirectX::SimpleMath::Vector3 StageFloorManager::POSITION = { 0.0f, -0.5f, 0.0f };

const float SCALE = 35.0f;

//------------------------------------------
// コンストラクタ
//------------------------------------------
StageFloorManager::StageFloorManager(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	auto device = m_pDR->GetD3DDevice();
	auto context = m_pDR->GetD3DDeviceContext();

	// 回転行列を作成
	DirectX::SimpleMath::Matrix rotation =
		DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0.0f));

	// ステージの床モデルを取得
	DirectX::Model* dishModel = Resources::GetInstance()->GetModel(L"Floor");

	// ステージのモデルを取得
	DirectX::Model* kitchenModel = Resources::GetInstance()->GetModel(L"Kitchen");

	// 床の位置
	DirectX::SimpleMath::Vector3 position = POSITION;
	// 床の作成
	m_floor = std::make_unique<Floor>(m_pDR, dishModel, position, rotation, SCALE);

	// 床の作成
	m_stage = std::make_unique<Stage>(m_pDR, kitchenModel, position, rotation, SCALE);

	// 衝突判定の表示オブジェクトの作成
	m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);
	// モデルのコリジョン作成
	const wchar_t* path = L"Resources/CollisionMeshes/DishStage.obj";
	// 当たり判定の作成
	m_objCollisions.push_back(CreateCollider(path, DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), rotation, 1.0f));
	// モデルの当たり判定を設定
	ObjectCollision::GetInstance()->SetModelCollision(&m_objCollisions[0]);
}

//------------------------------------------
// デストラクタ
//------------------------------------------
StageFloorManager::~StageFloorManager()
{
}

//------------------------------------------
// 初期化
//------------------------------------------
void StageFloorManager::Initialize()
{

}

//------------------------------------------
// 更新
//------------------------------------------
void StageFloorManager::Update()
{
	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	m_floor->Update(elapsedTime, {0.0f, 0.0f ,0.0f});

	m_stage->Update(elapsedTime, {0.0f, 0.0f ,0.0f});
}

//------------------------------------------
// 描画
//------------------------------------------
void StageFloorManager::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection,
	std::function<void __cdecl()> function
)
{
	m_floor->Render(context, states, view, projection, function);

	m_stage->Render(context, states, view, projection, function);

	//// 衝突判定の表示
	//m_objCollisions[0].AddDisplayCollision(m_displayCollision.get());
	//m_displayCollision->DrawCollision(context, states, view, projection);

}


//------------------------------------------
// 終了処理
//------------------------------------------
void StageFloorManager::Finalize()
{

}

//------------------------------------------
// 床オブジェクト作成
//------------------------------------------
std::unique_ptr<Floor> StageFloorManager::CreateFloor(
	DirectX::Model* model, 
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Matrix rotation,
	const float& scale
)
{
	std::unique_ptr<Floor> floor;
	// 床のインスタンス生成
	floor.reset(new Floor(m_pDR, model, position, rotation, scale));

	return floor;
}


//------------------------------------------
// 床の衝突判定作成
//------------------------------------------
Imase::ObjCollision StageFloorManager::CreateCollider(
	const wchar_t* path,
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Matrix rotation,
	const float& scale
)
{
	// 回転行列をクォータニオンに変換
	DirectX::SimpleMath::Quaternion rotate = 
		DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(rotation);
	// 当たり判定オブジェクトのインスタンス生成
	Imase::ObjCollision collision(path, position, rotate, scale);

	return collision;
}
