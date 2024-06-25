//--------------------------------------------------------------------------------------
// File: SoftBodyCorn.h
//
// ソフトボディを作成するクラス
//
// Date: 2023.5.17
// LastUpdate Date: 2023/08/28 
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "SoftBodyCorn.h"

SoftBodyCorn::SoftBodyCorn()
	:
	m_position{}
	, m_pObjCollision(nullptr)
	, m_pSphere(nullptr)
	, m_rotation{}
	//, m_scale(1.0)
{
}

void SoftBodyCorn::Intialize(DirectX::SimpleMath::Vector3 startPos)
{
	// 初期位置を設定
	m_position = startPos;
}

void SoftBodyCorn::Update(const DX::StepTimer& timer)
{
	// バウンディングボックスの直径
	DirectX::SimpleMath::Vector3 max = m_points[0].GetPosition();
	DirectX::SimpleMath::Vector3 min = m_points[0].GetPosition();
	for (PhysicalPoint& point : m_points)
	{
		// 頂点の更新
		point.Update(timer);
		//// 頂点と球の当たり判定
		//CollisionDitection(point);

		// ボックスの半径を更新
		DirectX::SimpleMath::Vector3 pos = point.GetPosition();
		// 半径の最大値を設定
		if (max.x < pos.x) max.x = pos.x;
		if (max.y < pos.y) max.y = pos.y;
		if (max.z < pos.z) max.z = pos.z;
		// 半径の最小値を設定
		if (min.x > pos.x) min.x = pos.x;
		if (min.y > pos.y) min.y = pos.y;
		if (min.z > pos.z) min.z = pos.z;
	}

	// ばねの更新
	for (Spring &spring : m_springs)
	{
		spring.CalculateForce();
	}

	// 面との当たり判定
	for (SBTriangle &face : m_faces)
	{
		face.Update();
		if (face.CollisionDitection(m_pSphere))
		{
			break;
		}
	}
	//// バウンディングボックスの中心位置を更新
	//int num = POINT_X_NUM * POINT_Y_NUM * POINT_Z_NUM - 1;
	//m_sbBox->Center = (m_points[0].GetPosition() + m_points[num].GetPosition()) / 2;
	//// 半径更新
	//m_sbBox->Extents = (max - min) / 2;
	//// 球の中心点がボックスの中だったら
	//if (m_sbBox->Contains(m_pSphere->GetPosition()))
	//{
	//	// ソフトボディの中か調べる
	//	if (IsPointInsideBox())
	//	{
	//		ClosestFace();
	//	}
	//}
}

/// <summary>
/// 描画
/// </summary>
/// <param name="batch"></param>
void SoftBodyCorn::Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* batch)
{
	//// 線の描画
	//for (auto &spring : m_springs)
	//{
	//	spring.Render(batch);
	//}
	//for (auto &face : m_faces)
	//{
	//	face.DebugRender(batch);
	//}
	//DX::Draw(batch, *m_sbBox.get(), DirectX::Colors::White);
	// 描画開始
	batch->Begin();

	// 頂点情報(板ポリゴンの４頂点の座標情報）
	std::vector<DirectX::VertexPositionColorTexture> vertex;
	// 描画開始頂点の要素番号
	int num = 0;
	// 面の数分ループ
	for (int i = 0; i < m_faces.size(); i += 1)
	{
		// 面の頂点位置をVPCTに変換
		for (int j = 0; j < m_faces[i].GetPointsPosition().size(); j++)
		{
			vertex.push_back(DirectX::VertexPositionColorTexture(
				m_faces[i].GetPointsPosition()[j],
				DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.5f),
				DirectX::SimpleMath::Vector2(0.5f, 0.5f)));
		}
		// 描画
		batch->DrawTriangle(vertex[num], vertex[num + 1], vertex[num + 2]);
		// 描画した頂点の数分プラス
		num += 3;
	}
	// 描画終了
	batch->End();

}

/// <summary>
/// ソフトボディを形成（3次元）
/// </summary>
void SoftBodyCorn::CreateFigure()
{
	const float SCALE = 1.0f;
	// コンテナ初期化
	m_points.clear();
	float height = 5.0f;
	// 点のインスタンス生成	
	DirectX::SimpleMath::Vector3 pos;

	for (int h = 0; h < POINT_Z_NUM; h++)
	{
		// 横の点の数分ループ
		for (int j = 0; j < POINT_X_NUM; j++)
		{

			// x座標計算(j - 点の数から1引いて÷２した数)
			float x = (j - static_cast<float>(POINT_X_NUM - 1) / 2.0f);
			// z座標計算
			float z = (h - static_cast<float>(POINT_Z_NUM - 1) / 2.0f);

			// 点のインスタンス生成	
			pos = DirectX::SimpleMath::Vector3(x, 0.0f, z);
			pos += m_position;
			PhysicalPoint p(pos * SCALE);
			// vector配列に格納
			m_points.push_back(p);
		}
	}
	pos = DirectX::SimpleMath::Vector3(0.0f, height, 0.0f);
	pos += m_position;
	PhysicalPoint topPoint(pos);

	m_points.push_back(topPoint);
	m_points.shrink_to_fit();
	// ばねを作成
	CreateArms();
	// 面を作成
	CreateFaces();
	// バウンディングボックスを作成
	m_sbBox = std::make_unique<DirectX::BoundingBox>(m_position, DirectX::SimpleMath::Vector3::Zero);
}

/// <summary>
/// 点と点をつなぐばねを作成（3次元）
/// </summary>
void SoftBodyCorn::CreateArms()
{
	m_springs.clear();
	// 斜め奥を少しだけ繋げる
	const static int diagonally[9][3] = {
		{1, 0, 0}, {1, 0, 0},{-1, 0, 0},
		{0, 0, 1}, {1, 0, 1},{0, 0, -1},
		{-1, 0, -1}, {1, 0, -1}
	};

	for (int h = 0; h < POINT_Z_NUM; h++)
	{
		for (int j = 0; j < POINT_X_NUM; j++)
		{
			// 点の右上右下左上左下に点があるか
			for (const auto& dia : diagonally)
			{
				// dia = -1～1の値
				int	x = j + dia[0];
				int z = h + dia[2];

				// もしaとbが点の数の範囲外じゃなかったら
				if (x >= 0 && x < POINT_X_NUM &&
					z >= 0 && z < POINT_Z_NUM)
				{
					// 参照する頂点のポインタの位置を計算
					int a = h * POINT_X_NUM + j;

					// 他の頂点のポインタの位置を計算
					int b = z * POINT_X_NUM + x;
					// ばねの作成
					Spring spring(&m_points[a], &m_points[b]);
					// ばねの情報を保存
					m_springs.push_back(spring);
				}
			}
		}
	}

	int last = POINT_X_NUM * POINT_Z_NUM;
	Spring spring1(&m_points[0], &m_points[last]);
	m_springs.push_back(spring1);
	Spring spring2(&m_points[1], &m_points[last]);
	m_springs.push_back(spring2);
	Spring spring3(&m_points[2], &m_points[last]);
	m_springs.push_back(spring3);
	Spring spring4(&m_points[3], &m_points[last]);
	m_springs.push_back(spring4);
	// 余分な配列を消す
	m_springs.shrink_to_fit();
}

/// <summary>
/// 面を作成
/// </summary>
void SoftBodyCorn::CreateFaces()
{
	m_faces.clear();
	// 前後上下左右の面を格納するコンテナ
	std::vector<SBTriangle> sideTriangles;
	std::vector<SBTriangle> bottomTriangles;

	int p0 = 0;
	int p1 = POINT_X_NUM - 1;
	int p2 = POINT_X_NUM * POINT_Z_NUM - POINT_X_NUM;
	int p3 = POINT_X_NUM * POINT_Z_NUM - 1;
	SBTriangle sideTriangle2(&m_points[p2], &m_points[p3], &m_points[m_points.size() - 1]);
	sideTriangles.push_back(sideTriangle2);
	SBTriangle sideTriangle0(&m_points[p0], &m_points[p1], &m_points[m_points.size() - 1]);
	sideTriangles.push_back(sideTriangle0);
	SBTriangle sideTriangle1(&m_points[p0], &m_points[p2], &m_points[m_points.size() - 1]);
	sideTriangles.push_back(sideTriangle1);
	SBTriangle sideTriangle3(&m_points[p1], &m_points[p3], &m_points[m_points.size() - 1]);
	sideTriangles.push_back(sideTriangle3);

	SBTriangle bottomTriangle1(&m_points[p0], &m_points[p1], &m_points[p2]);
	sideTriangles.push_back(bottomTriangle1);
	SBTriangle bottomTriangle2(&m_points[p1], &m_points[p2], &m_points[p3]);
	sideTriangles.push_back(bottomTriangle2);

	std::copy(sideTriangles.begin(), sideTriangles.end(), std::back_inserter(m_faces));
	std::copy(bottomTriangles.begin(), bottomTriangles.end(), std::back_inserter(m_faces));

	// 余分なキャパを削除
	m_faces.shrink_to_fit();
}

void SoftBodyCorn::CollisionDitection(PhysicalPoint& point)
{

	// 頂点の位置を取得
	DirectX::SimpleMath::Vector3 pointPos = point.GetPosition();
	// 球の位置を取得
	DirectX::SimpleMath::Vector3 spherePos = m_pSphere->GetPosition();
	// 点と点の距離
	DirectX:: SimpleMath::Vector3 distance = point.GetPosition() - m_pSphere->GetPosition();
	// 距離の内積を求める
	float distSq = distance.Dot(distance);
	// 半径の合計
	float radisuSum = m_pSphere->RADIUS + point.GetRadius();
	// 距離と半径を比較
	if (distSq <= radisuSum * radisuSum)
	{
		// 半径の合計のほうが大きければ当たっている
		// 当たった点に力を加える
		point.AddForce(m_pSphere->GetTotalForce() * 20);
		// ボール反射
		m_pSphere->Reflect(m_rotation);
	}
}

/// <summary>
/// 点がソフトボディの中か調べる
/// </summary>
/// <returns>交差した数の余り(1で中, 0で外)</returns>
bool SoftBodyCorn::IsPointInsideBox()
{
	// 最近接点の
	m_closestPoints.clear();
	// 光線
	DirectX::SimpleMath::Ray ray = {
		m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(0.0f,  1.0f, 0.0f)
		//{m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f)},
		//{m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(0.0f,  0.0f, 1.0f)},
		//{m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f)},
		//{m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(1.0f,  0.0f, 0.0f)},
		//{m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f)},
	};
	// 交差数
	int intersectCount = 0;
	for (auto& face : m_faces)
	{
		// 光線と面の交差判定
		if (face.RayIntersectsQuad(ray, m_pSphere))
		{
			// 交差していたらカウント
			intersectCount++;
			// 最近接点を格納
			m_closestPoints.push_back(face.GetClosestPoint());
		}
	}
	return intersectCount % 2;
}

/// <summary>
/// 面上の一番近い点を計算
/// </summary>
void SoftBodyCorn::ClosestFace()
{
	float distance = 100.0f;
	distance;
	int num = 0;
	num;

	// 光線
	DirectX::SimpleMath::Ray ray = {
		m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(0.0f,  1.0f, 0.0f)
	};
	for (auto& face : m_faces)
	{
		face.CalculateForce(ray, m_pSphere);
	}

	//for (int i = 0; i < m_closestPoints.size(); i++)
	//{
	//	// 距離を求める
	//	float tmp = DirectX::SimpleMath::Vector3::DistanceSquared(m_pSphere->GetPosition(), m_closestPoints[i]);
	//	// 距離が小さい方を設定する
	//	if (distance > tmp)
	//	{
	//		// 距離を設定
	//		distance = tmp;
	//		// 番号を設定
	//		num = i;
	//	}
	//}
	//
	//m_pSphere->SetPosition(m_closestPoints[num]);

}