//--------------------------------------------------------------------------------------
// File: SoftBodyCube.h
//
// 回転するバー
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef SOFTBODY_CUBE_DEFINED
#define SOFTBODY_CUBE_DEFINED

#include "MyLib/SoftBody/SoftBodyFactory.h"
#include "Scene/GameMainObjects/Objects/SoftBodyBar.h"
#include "Scene/GameMainObjects/Objects/CenterSoftBody.h"
#include "Scene/Effects/GetParticle.h"
#include "Scene/GameMainObjects/UI/Score.h"

class SoftBodyCube
{
public:

	// シーンの状態
	enum SceneState
	{
		MAIN,
		RESULT	
	};

public:

	// コンストラクタ
	SoftBodyCube();

	// デストラクタ
	~SoftBodyCube();

	// 初期化
	void CreateCube();

	// 更新
	void Update(const int& spendingTime, CenterSoftBody* pCylinder);

	// 描画
	void Render();

	// 影描画
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

public:

	// リザルト時の初期化
	void ResultInitialize(Score* pScore);

	// リザルト時の更新処理
	void ResultUpdate();

	// リザルト時の描画
	void ResultRender();

	// リザルト時の影描画
	void ResultShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

	// カウント終了したかどうか
	const bool& GetCountFinished() { return m_pScore->GetResultCountFinish(); }

private:

	// 壁の状態を確認
	void SetPointsState();

	// ゼリーキューブを出現
	void AppearCubes();

	// 更新
	void CubesUpdate(const int& index);

	void HitBar(const int& index, SoftBodyBar* pBar, const float& speed);

	// 指定された番号のオブジェクトのパラメータ初期化
	void ResetParameters(const int& index);
	
	// 複製する
	void DupricateCube();

private:

	// ゼリーキューブ
	std::vector<std::shared_ptr<SoftBodyBox>> m_boxes;

	// 出現時間(フレーム時間)
	int m_spawnTime;

	// 最大出現個数
	int m_cubeMaxNum;
	// 出現数
	int m_cubeNum;

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// 衝突判定の表示オブジェクトへのポインタ
	std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// 当たり判定オブジェクト
	std::vector<DirectX::BoundingSphere> m_collisions;

	// 取得音へのポインタ
	DirectX::SoundEffectInstance* m_hitSound;

	// 経過時間
	int m_spendingTime;

	// ゼリーキューブのインデックス番号
	int m_cubeIndex;

	// 出現した
	bool m_appeared;

	// 取得時のパーティクル
	std::unique_ptr<GetParticle> m_getParticle;

private:

	// リザルト時の出現時間
	int m_timer;

	// リザルト時のキャラが正面を向くまでの時間
	float m_interval;

	// スコア表示オブジェクトへのポインタ
	Score* m_pScore;

	// リセットの位置
	DirectX::SimpleMath::Vector3 m_resultResetPosition;

	// パラメータ
	nlohmann::json m_parameters;
};

#endif // SOFTBODY_CUBE_DEFINED
