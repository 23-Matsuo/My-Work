//--------------------------------------------------------------------------------------
// File: SoftBodyCorn.h
//
// 共通オブジェクト継承用クラス
//
// Usage: SoftBodyCornに継承する
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#include "MyLib/PhysicalObject.h"
#include "SoftBodyFaces.h"
#include "Spring.h"
#include "ImaseLib/ObjCollision.h"

class SoftBodyCorn : public PhysicalObject
{
public:
	enum class STATE
	{
		FIXED,
		UNFIXED
	};

public:

	// コンストラクタ
	SoftBodyCorn();
	// デストラクタ
	~SoftBodyCorn() {};
	// 初期化
	void Intialize(DirectX::SimpleMath::Vector3 startPos);
	// 更新
	void Update(const DX::StepTimer& timer) override;
	// 描画
	void Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* batch);
	// 形を作る
	void CreateFigure();
	// 点と点の間のばねを作る
	void CreateArms();
	// 点と点の間の面を作る
	void CreateFaces();
	// 点との衝突判定を取る
	void CollisionDitection(PhysicalPoint& point);
	// 点とソフトボディの内外判定
	bool IsPointInsideBox();
	// 最近面の位置を見る
	void ClosestFace();
public:
	// 点のX軸方向の数
	int POINT_X_NUM = 2;
	// 点のZ軸方向の数
	int POINT_Z_NUM = 2;

public:
	std::vector<PhysicalPoint> GetPoints() { return m_points; }
	std::vector<Spring> GetSprings() { return m_springs; }
	std::vector<SBSquare> GetFaces() { return m_faces; }

public:
	void SetObjCollision(Imase::ObjCollision* pObj) { m_pObjCollision = pObj; }
	// 球を設定
	void SetSphere(CatSphere*sphere) { m_pSphere = sphere; }
	// 回転角を設定
	void SetRotation(DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }
	// 頂点の数を設定
	void SetPointNumber(const int x, const int z) 
	{
		POINT_X_NUM = x; POINT_Z_NUM = z;
	}
	// 頂点の位置を設定
	void SetPointsPosition(DirectX::SimpleMath::Vector3 position) 
	{
		for (int i = 0; i < m_points.size(); i++)
		{
			m_points[i].SetPosition(m_points[i].GetPosition() + position);
		}
	}
private:
	// 頂点のコンテナ
	std::vector<PhysicalPoint> m_points;
	// ばねのコンテナ
	std::vector<Spring> m_springs;
	// 面のコンテナ
	std::vector<SBSquare> m_faces;

	// ポジション
	DirectX::SimpleMath::Vector3 m_position;
	// 回転角
	DirectX::SimpleMath::Quaternion m_rotation;

	Imase::ObjCollision* m_pObjCollision;
	CatSphere* m_pSphere;
	
	std::unique_ptr<DirectX::BoundingBox> m_sbBox;

	std::vector<DirectX::SimpleMath::Vector3> m_closestPoints;
};