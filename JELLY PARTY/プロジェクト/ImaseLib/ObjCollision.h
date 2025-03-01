//--------------------------------------------------------------------------------------
// File: ObjCollision.h
//
// Obj形式のメッシュをコリジョンにするクラス
//
// Date: 2018.7.11
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "Mylib/Collision.h"
#include "ImaseLib/DisplayCollision.h"

namespace Imase
{
	// Obj形式のメッシュをコリジョンにするクラス
	class ObjCollision
	{
	private:

		// 位置
		DirectX::SimpleMath::Vector3 m_position;

		// 回転
		DirectX::SimpleMath::Quaternion m_rotation;
		// 大きさ
		float m_scale;

	private:

		// 頂点情報
		std::vector<DirectX::SimpleMath::Vector3> m_vertexes;

		// インデックス情報
		std::vector<uint32_t> m_indexes;

		// コリジョン用三角形データ
		std::vector<Collision::Triangle> m_triangles;

		// コリジョン用三角形データの追加関数
		void AddTriangle(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, DirectX::SimpleMath::Vector3 c);

	public:

		// コンストラクタ
		ObjCollision(
			const wchar_t* fileName
			, const DirectX::SimpleMath::Vector3& position
			, const DirectX::SimpleMath::Quaternion& rotate
			, const float& scale
		);

		// コリジョン情報の更新
		void UpdateBoundingInfo(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& rotate);

		// 線分との交差判定関数
		bool IntersectLineSegment(
			DirectX::SimpleMath::Vector3 a,
			DirectX::SimpleMath::Vector3 b,
			DirectX::SimpleMath::Vector3* hitPosition,
			DirectX::SimpleMath::Vector3* normal = nullptr
		);

		// 衝突判定の表示に登録する関数
		void AddDisplayCollision(Imase::DisplayCollision* displayCollision, DirectX::FXMVECTOR lineColor = DirectX::XMVECTORF32{ 0.0f, 0.0f, 0.0f, 0.0f }) const;

	};
}

