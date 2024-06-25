//--------------------------------------------------------------------------------------
// File: Floor.cpp
//
// ステージの床
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Floor.h"


//------------------------------------------
// コンストラクタ
//------------------------------------------
Floor::Floor(
	DX::DeviceResources* pDR,
	DirectX::Model* pModel,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Matrix& rotation,
	const float& scale
)
	:
	m_pDR(pDR)
	, m_floorModel(pModel)
	, m_position(position)
	, m_rotation(rotation)
	, m_scale(scale)
	, m_isActive(true)
{

}

//------------------------------------------
// デストラクタ
//------------------------------------------
Floor::~Floor()
{

}

//------------------------------------------
// 初期化
//------------------------------------------
void Floor::Initialize()
{
}

//------------------------------------------
// 更新
//------------------------------------------
void Floor::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position)
{
	UNREFERENCED_PARAMETER(position);
	UNREFERENCED_PARAMETER(elapsedTime);
}

//------------------------------------------
// 描画
//------------------------------------------
void Floor::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection,
	std::function<void __cdecl()> function
)
{

	// ワールド行列
	DirectX::SimpleMath::Matrix world;

	DirectX::SimpleMath::Matrix translationMat;
	DirectX::SimpleMath::Matrix rotationMat;
	DirectX::SimpleMath::Matrix scaleMat;

	// 拡縮行列作成
	scaleMat = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	// 回転行列作成
	rotationMat = m_rotation;
	// 移動行列作成
	translationMat = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	// ワールド行列作成
	world = scaleMat * rotationMat * translationMat;
	// 足場モデル描画
	m_floorModel->Draw(context, *states, world, view, projection, false, function);
}


//------------------------------------------
// 終了処理
//------------------------------------------
void Floor::Finalize()
{

}
