//--------------------------------------------------------------------------------------
// File: CenterSoftBody.h
//
// 回転するバーの中心
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef CENTER_SOFTBODY_DEFINED
#define CENTER_SOFTBODY_DEFINED

#include "MyLib/SoftBody/SoftBodyFactory.h"
#include "Scene/Effects/HitParticle.h"

class CenterSoftBody
{
public:

public:
	// コンストラクタ
	CenterSoftBody();

	// デストラクタ
	~CenterSoftBody();

	// 更新
	void Update(DirectX::SimpleMath::Matrix rotation);

	// 描画
	void Render();

	// 影描画
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

public:

	SoftBodyCylinder::Points* GetPoints() { return m_cylinder->GetPoints(); }
	// 頂点を繋げる
	std::vector<DirectX::SimpleMath::Vector3> GetJointPoints() const { return m_jointPoints; }

	DirectX::BoundingSphere GetCollision() const { return m_collision[0]; }

	SoftBodyCylinder* GetCylinder() { return m_cylinder.get(); }
private:

	// ジョイント作成
	void CreateJoint();

private:
	
	// ジョイントする位置
	std::vector<DirectX::SimpleMath::Vector3> m_jointPoints;

	// シリンダーへのポインタ
	std::unique_ptr<SoftBodyCylinder> m_cylinder;

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// 衝突判定オブジェクト
	DirectX::BoundingSphere m_collision[2];

	// 衝突判定の表示オブジェクトへのポインタ
	std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// パラメータ
	nlohmann::json m_parameters;

	// 頂点の数
	int m_pointXNum;
	int m_pointYNum;

	// 当たったエフェクト
	std::unique_ptr<HitParticle> m_hitParticle;
};

#endif // CENTER_SOFTBODY_DEFINED
