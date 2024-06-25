//--------------------------------------------------------------------------------------
// File: SoftBodyCylinder.h
//
// 円柱ソフトボディクラス
//
// 
// Last Update: 2023.11.14
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef SOFTBODY_CYLINDER_DEFINED
#define SOFTBODY_CYLINDER_DEFINED

#include "MyLib/PhysicalObject.h"
#include "Spring.h"
#include "RenderSoftBody.h"

#include "ImaseLib/ObjCollision.h"
#include <array>

class SoftBodyCylinder : public PhysicalObject
{
public:

	// エイリアス宣言
	using Points = std::vector<PhysicalPoint>;

public:
	enum class STATE
	{
		FIXED,
		UNFIXED
	};

	// 点のX軸方向の数
	int POINT_X_NUM = 8;
	// 点のY軸方向の
	int POINT_Y_NUM = 2;

	enum Face
	{
		BOTTOM,
		TOP,
		SIDE,

		FACENUM
	};

public:

	// コンストラクタ
	SoftBodyCylinder(
		const int& xNum, const int& yNum,
		const float& radius,
		const float& height,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotation,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Color& color,
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED
	);

	// デストラクタ
	~SoftBodyCylinder() {};

	// 初期化
	void Intialize(ID3D11ShaderResourceView* texture);

	// 更新
	void Update(const DX::StepTimer& timer) override;

	// 回転
	void Rotate(DirectX::SimpleMath::Matrix rotation);

	// 描画
	void Render();

	// 影描画
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

public:

	// 形を作る
	void CreateFigure();

public:	// ゲッター

	// 頂点取得
	Points* GetPoints()									{ return &m_points; }
	// 回転取得
	const DirectX::SimpleMath::Quaternion GetRotation() { return m_rotation; }
	// 当たっているかどうか取得
	const bool GetIsHit() 								{ return m_isHit; }

	const int GetXNum()									{ return POINT_X_NUM; }
	const int GetYNum()									{ return POINT_Y_NUM; }

	const DirectX::SimpleMath::Color GetColor()			{ return m_color; }
private:

	void UpdateRenderPosition();

	// ボディの状態を確認
	void CheckBodyState();

	// 点と点の間のばねを作る
	void CreateArms();

	// 箱の面を作成
	void CreateFaces();

	void CreateSideFaces();
	void CreateTBFaces();

	// 重複を削除
	void DeleteDuplication(const Face& face);
	// UV座標の計算
	void CalculateUV(const int& face);

	// UV座標の計算
	void CalculateIndex(const int& face);


private:

	// 頂点のコンテナ
	Points m_points;

	// ばねのコンテナ
	std::vector<Spring> m_springs;

	// 面を構成する頂点番号を格納する
	std::array<std::vector<unsigned int>, Face::FACENUM> m_pointsIndices;

	// 面を構成する頂点番号の重複を消したものを格納する
	std::array<std::vector<unsigned int>, Face::FACENUM> m_faceIndices;

	// ゼリーの面描画用クラスへのポインタ * 6
	std::array<std::unique_ptr<RenderSoftBody>, Face::FACENUM> m_renders;
private:

	// 状態
	PhysicalPoint::STATE m_state;

	// ソフトボディのポジション
	DirectX::SimpleMath::Vector3 m_position;

	// 初期ポジション
	DirectX::SimpleMath::Vector3 m_initPosition;

	// 回転角
	DirectX::SimpleMath::Quaternion m_rotation;

	// 大きさ
	DirectX::SimpleMath::Vector3  m_scale;

	// 半径
	float m_radius;
	// 高さ
	float m_height;

	// 色
	DirectX::SimpleMath::Color m_color;

	// 当たった
	bool m_isHit;

	// UVのオフセット
	DirectX::SimpleMath::Vector3 m_uvOffset;
};

#endif	// SOFTBODY_CYLINDER_DEFINED