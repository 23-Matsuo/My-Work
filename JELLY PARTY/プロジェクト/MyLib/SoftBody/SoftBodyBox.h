//--------------------------------------------------------------------------------------
// File: SoftBodyBox.h
//
// ソフトボディの管理クラス
//
// 
// Last Update: 2023.10.21
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef SOFTBODY_BOX_DEFINED
#define SOFTBODY_BOX_DEFINED

#include "MyLib/PhysicalObject.h"
#include "Spring.h"
#include "RenderSoftBody.h"

#include "ImaseLib/ObjCollision.h"
#include <array>

class SoftBodyBox : public PhysicalObject
{
public:

	// エイリアス宣言
	using Points = std::vector<PhysicalPoint>;

public:
	enum class STATE
	{
		FIXED,
		UNFIXED,
		CORNERFIXED,
	};

	// 点のX軸方向の数
	int POINT_X_NUM = 3;
	// 点のY軸方向の
	int POINT_Y_NUM = 3;
	// 点のZ軸方向の数
	int POINT_Z_NUM = 3;

	enum Face
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		FRONT,
		BACK,

		FACENUM
	};

public:

	// コンストラクタ
	SoftBodyBox();

	// コンストラクタ
	SoftBodyBox(
		const int& xNum,
		const int& yNum,
		const int& zNum,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotation,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Color& color,
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED
	);

	// デストラクタ
	~SoftBodyBox() {};

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

	// クローン
	void Clone(SoftBodyBox* other)
	{
		other->SetPointsNum(POINT_X_NUM, POINT_Y_NUM, POINT_Z_NUM);
		other->SetState(m_state);
		other->SetPosition(m_position);
		other->SetRotation(m_rotation);
		other->SetPoints(m_points);
		other->CreateArms();
		other->SetVertices(m_vertices);
		other->SetColor(m_color);
		other->SetRenders(m_renders);
		other->SetFaceIndices(m_faceIndices);
	}

public:

	// 形を作る
	void CreateFigure();

	// モデルとの衝突判定
	void CheckHitModel2Point(Imase::ObjCollision* pObj);

	// 位置を初期化
	void ResetParameters(DirectX::SimpleMath::Vector3 position);

	// リザルトの初期化
	void ResultReset(const DirectX::SimpleMath::Vector3& position);

public:

	// 頂点を設定
	void SetPointsNum(const int& x, const int& y, const int& z);
	// 頂点を設定
	void SetPoints(Points points) { m_points = points; }
	// ばねを設定
	void SetSprings(std::vector<Spring> springs) { m_springs = springs; }
	// 回転を設定
	void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) { m_rotation = rotation; }
	// 色を設定
	void SetColor(const DirectX::SimpleMath::Color& color);
	// インデックスを設定
	void SetFaceIndices(const std::array<std::vector<unsigned int>, Face::FACENUM>& faceIndices);
	// 頂点を設定
	void SetVertices(const std::array<std::vector<DirectX::VertexPositionColorTexture>, Face::FACENUM>& vertices);
	// 頂点の位置を設定
	void SetPointsPosition();
	// 状態を設定
	void SetState(const PhysicalPoint::STATE& state) { m_state = state; }
	// 描画の設定
	void SetRenders(const std::array<std::unique_ptr<RenderSoftBody>, Face::FACENUM>& renders);

public:	// ゲッター

	// 頂点取得
	Points* GetPoints()											{ return& m_points; }
	// 回転取得
	const DirectX::SimpleMath::Quaternion& GetRotation()		{ return m_rotation; }
	// 当たっているかどうか取得
	const DirectX::SimpleMath::Color& GetColor()				{ return m_color; }

	const DirectX::SimpleMath::Vector3& GetInitialPosition()	{ return m_initPosition; }

	// スケール取得
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }

private:

	void UpdateRenderPosition();

	// 点と点の間のばねを作る
	void CreateArms();

	// 箱の面を作成
	void CreateFaces();

	void CreateFBFaces();
	void CreateTBFaces();
	void CreateLRFaces();

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
	
	// 描画する頂点
	std::array<std::vector<DirectX::VertexPositionColorTexture>, Face::FACENUM> m_vertices;

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


	// 色
	DirectX::SimpleMath::Color m_color;


	// UVのオフセット
	DirectX::SimpleMath::Vector3 m_uvOffset;

	bool m_colorChanged;
};

#endif	// SOFTBODY_BOX_DEFINED