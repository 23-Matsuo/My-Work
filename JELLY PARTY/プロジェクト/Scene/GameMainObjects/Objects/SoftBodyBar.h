//--------------------------------------------------------------------------------------
// File: SoftBodyBar.h
//
// 回転するバー
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef SOFTBODY_BAR_DEFINED
#define SOFTBODY_BAR_DEFINED

#include "CenterSoftBody.h"
#include "Scene/Effects/HitParticle.h"

class SoftBodyBar
{
public:
	enum class COLORS
	{
		MAGENTA,
		CYAN,
		YELLOW
	};

	// バーの数
	static const int BAR_NUM = 2;

public:

	// コンストラクタ
	SoftBodyBar(const std::vector<DirectX::SimpleMath::Vector3>& jointPoints);

	// デストラクタ
	~SoftBodyBar();

	// 更新
	void Update(
		DirectX::SimpleMath::Matrix rotation,
		SoftBodyCylinder* pCylinder,
		const float& anglarVelocity
	);

	// 描画
	void Render();

	// 影描画
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

public:

	// カラー情報取得
	DirectX::SimpleMath::Color GetColor(COLORS color);

	// ソフトボディ取得
	SoftBodyBox* GetSoftBodies(const int& index) { return m_box[index].get(); }

private:

	// バーの状態を設定
	void SetBarPointsState();

	// 頂点を繋げる
	void JointPoints();

private:

	// 繋げる頂点
	std::vector<DirectX::SimpleMath::Vector3> m_jointPoints;
	// ボックスへのポインタ
	std::unique_ptr<SoftBodyBox> m_box[BAR_NUM];

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// 衝突判定の表示オブジェクトへのポインタ
	std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// 当たったエフェクト
	std::unique_ptr<HitParticle> m_hitParticle;

	// パラメータ
	nlohmann::json m_parameters;

	// 頂点の数
	int m_pointXNum;
	int m_pointYNum;
	int m_pointZNum;
};

#endif // SOFTBODY_BAR_DEFINED
