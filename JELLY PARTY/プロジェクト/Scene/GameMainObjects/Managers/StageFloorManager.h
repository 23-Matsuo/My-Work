//--------------------------------------------------------------------------------------
// File: StageFloorManager.h
//
// ステージの床管理
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------
#pragma once

#ifndef STAGE_FLOOR_MANAGER_DEFINED
#define STAGE_FLOOR_MANAGER_DEFINED

#include "DeviceResources.h"
#include "ImaseLib/ObjCollision.h"
#include "MyLib/GameObject.h"
#include "Scene/GameMainObjects/Objects/Floor.h"
#include "Scene/GameMainObjects/Objects/Stage.h"

class StageFloorManager
{
public:

	// 床オブジェクトの数
	static const int NUM;

	static const DirectX::SimpleMath::Vector3 FLOOR_RADUIS;

	static const DirectX::SimpleMath::Vector3 POSITION;

public:
	
	// コンストラクタ
	StageFloorManager(DX::DeviceResources* pDR);

	// デストラクタ
	~StageFloorManager();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection,
		std::function<void __cdecl()> function
	);

	// 終了処理
	void Finalize();

public:
	
	// モデルの当たり判定を取得
	std::vector<Imase::ObjCollision> GetObjCollisions() { return m_objCollisions; }

private:
	// 床作成
	std::unique_ptr<Floor> CreateFloor(
		DirectX::Model* model,
		DirectX::SimpleMath::Vector3 position,
		DirectX::SimpleMath::Matrix rotation,
		const float& scale
	);

	// 当たり判定作成
	Imase::ObjCollision CreateCollider(
		const wchar_t* path,
		DirectX::SimpleMath::Vector3 position,
		DirectX::SimpleMath::Matrix rotation,
		const float& scale
	);

	// デバイスリソースへのポインタ
	DX::DeviceResources* m_pDR;

	// 衝突判定の表示オブジェクトへのポインタ
	std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// 衝突判定のオブジェクト
	std::vector<Imase::ObjCollision> m_objCollisions;

	// 床のオブジェクト
	std::unique_ptr<Floor> m_floor;
	// ステージのオブジェクト
	std::unique_ptr<Stage> m_stage;

	DirectX::SimpleMath::Vector3 m_position;
};

#endif // STAGE_FLOOR_MANAGER_DEFINED