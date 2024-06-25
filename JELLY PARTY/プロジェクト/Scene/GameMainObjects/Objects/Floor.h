//--------------------------------------------------------------------------------------
// File: Floor.h
//
// ステージの床
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------
#pragma once

#ifndef FLOOR_DEFINED
#define FLOOR_DEFINED

#include "DeviceResources.h"

#include "MyLib/GameObject.h"

class Floor
{
public:
	
	// コンストラクタ
	Floor(
		DX::DeviceResources* pDR,
		DirectX::Model* pModel,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Matrix& rotation,
		const float& scale
	);

	// デストラクタ
	~Floor();

	// 初期化
	void Initialize();

	// 更新
	void Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position);

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
	// IDを取得
	int GetObjectID() { return 0; }

	// 位置を取得
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	// モデルを取得
	DirectX::Model* GetModel() { return m_floorModel; }

	// オブジェクトの状態を設定
	void SetActive(bool isActive) { m_isActive = isActive; }

private:

	// デバイスリソースへのポインタ
	DX::DeviceResources* m_pDR;
	// モデル
	DirectX::Model* m_floorModel;
	// ポジション
	DirectX::SimpleMath::Vector3 m_position;
	// 回転行列
	DirectX::SimpleMath::Matrix m_rotation;
	// 拡縮
	float m_scale;
	// 状態
	bool m_isActive;

};

#endif // FLOOR_DEFINED