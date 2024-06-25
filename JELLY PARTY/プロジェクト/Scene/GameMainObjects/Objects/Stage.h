//--------------------------------------------------------------------------------------
// File: Stage.h
//
// ステージ上のオブジェクト管理クラス
//
// Date: 2023.7.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef STAGE_DEFINED
#define STAGE_DEFINED

class Stage
{
public:

	// コンストラクタ
	Stage(
		DX::DeviceResources* pDR,
		DirectX::Model* pModel,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Matrix& rotation,
		const float& scale
	);
	// デストラクタ
	~Stage() 
	{
		Finalize();
	};
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
	DirectX::Model* GetModel() { return m_kitchenModel; }

	// オブジェクトの状態を設定
	void SetActive(bool isActive) { m_isActive = isActive; }

private:

	// デバイスリソースへのポインタ
	DX::DeviceResources* m_pDR;
	// モデル
	DirectX::Model* m_kitchenModel;
	// ポジション
	DirectX::SimpleMath::Vector3 m_position;
	// 回転行列
	DirectX::SimpleMath::Matrix m_rotation;
	// 拡縮
	float m_scale;
	// 状態
	bool m_isActive;

};

#endif // STAGE_DEFINED