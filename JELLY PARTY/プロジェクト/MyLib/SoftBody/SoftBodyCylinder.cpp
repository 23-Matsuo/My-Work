//--------------------------------------------------------------------------------------
// File: SoftBodyCylinder.cpp
//
// 円柱ソフトボディクラス
//
// Last Update: 2023.11.14
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "SoftBodyCylinder.h"

const float SCALE = 1.0f;

//------------------------------------------
// コンストラクタ
//------------------------------------------
SoftBodyCylinder::SoftBodyCylinder(
	const int& xNum, const int& yNum,
	const float& radius,
	const float& height,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotation,
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Color& color,
	const PhysicalPoint::STATE& state
)
	:
	POINT_Y_NUM(yNum)
	, m_radius(radius)
	, m_height(height)
	, m_position(position)
	, m_initPosition(m_position)
	, m_rotation(rotation)
	, m_scale(scale)
	, m_color(color)
	, m_state(state)
	, m_isHit(false)
{
	// 8の倍数
	POINT_X_NUM *= xNum;


	// uvのオフセットを計算
	m_uvOffset.x = 1.0f / (POINT_X_NUM  / 4);
	m_uvOffset.y = 1.0f / (POINT_Y_NUM - 1);
	//m_uvOffset.z = 1.0f / (POINT_X_NUM / 2 + 1);
}

//------------------------------------------
// 初期化
//------------------------------------------
void SoftBodyCylinder::Intialize(ID3D11ShaderResourceView* texture)
{
	// 面の数
	for (auto& render : m_renders)
	{
		// レンダーの作成
		render = std::make_unique<RenderSoftBody>();
		render->Create(texture);
	}

	for (int i = 0; i < FACENUM; i++)
	{
		std::vector<DirectX::VertexPositionColorTexture> vertices;

		// トップとサイドのみ計算する
		if (!(i % 2))
		{
			CalculateUV(i);
			CalculateIndex(i);
		}

		for (int j = 0; j < m_faceIndices[i].size(); j++)
		{
			// 頂点を作成
			DirectX::VertexPositionColorTexture vertex = {
				m_points[m_faceIndices[i][j]].GetPosition(),
				m_color,
				m_points[m_faceIndices[i][j]].GetUV()
			};
			vertices.push_back(vertex);
		}
		// バッファを作成
		m_renders[i]->CreateBuffer(vertices);
	}
}

//------------------------------------------
// 更新
//------------------------------------------
void SoftBodyCylinder::Update(const DX::StepTimer& timer)
{
	// 頂点の更新
	for (PhysicalPoint& point : m_points)
	{
		point.Update(timer);
	}

	UpdateRenderPosition();

	// ばねの更新
	for (Spring& spring : m_springs)
	{
		spring.CalculateForce();
	}

	int num = static_cast<int>(m_points.size() - 1);
	// 中心点の位置
	m_position = (m_points[0].GetPosition() + m_points[num].GetPosition()) / 2;

}

//------------------------------------------
// 回転
//------------------------------------------
void SoftBodyCylinder::Rotate(DirectX::SimpleMath::Matrix rotation)
{
	for (PhysicalPoint& point : m_points)
	{
		point.SetPosition(DirectX::SimpleMath::Vector3::Transform(point.GetPosition(), rotation));
	}
}

//------------------------------------------
// 描画
//------------------------------------------
void SoftBodyCylinder::Render()
{
	for (auto& render : m_renders)
	{
		render->Render();
	}
}

//------------------------------------------
// 影描画
//------------------------------------------
void SoftBodyCylinder::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	// 面を描画
	for (auto& render : m_renders)
	{
		render->ShadowRender(lightView, lightProjection);
	}
}



//------------------------------------------
// 描画位置を更新
//------------------------------------------
void SoftBodyCylinder::UpdateRenderPosition()
{
	// 面の位置
	for (int i = 0; i < m_faceIndices.size(); i++)
	{
		std::vector<DirectX::SimpleMath::Vector3> verticesPos;
		// 頂点のインデックス
		for (int j = 0; j < m_faceIndices[i].size(); j++)
		{
			DirectX::SimpleMath::Vector3 position = m_points[m_faceIndices[i][j]].GetPosition();
			verticesPos.push_back(position);
		}
		m_renders[i]->Update(verticesPos);

	}
}

//------------------------------------------
// ソフトボディを形成（3次元）
//------------------------------------------
void SoftBodyCylinder::CreateFigure()
{

	// コンテナ初期化
	m_points.clear();

	int ID = 1;
	
	for (int j = 0; j < POINT_Y_NUM; j++)
	{
		for (int i = 0; i < POINT_X_NUM; ++i)
		{
			float angle = DirectX::XM_2PI * static_cast<float>(i) / static_cast<float>(POINT_X_NUM);
			float x = m_radius * cosf(angle);
			float y = m_radius * sinf(angle);

			// Vector3に変換
			DirectX::SimpleMath::Vector3 pos = { x, m_position.y + m_height * j ,y };
			// 拡縮
			pos *= SCALE;
			// 回転
			pos = DirectX::SimpleMath::Vector3::Transform(pos, m_rotation);
			// 移動
			pos += m_position;
			// 点のポジション設定
			PhysicalPoint p(pos, ID);
			// 頂点のコライダーのサイズを設定
			p.SetColliderSize(0.2f);
			// vector配列に格納
			m_points.push_back(p);

			ID++;
		}
	}
	// 余分なキャパ削除
	m_points.shrink_to_fit();
	// ソフトボディの状態を見る
	CheckBodyState();
	// 右回りに面を作成
	CreateFaces();
}

//------------------------------------------
// 固定するかどうか
//------------------------------------------
void SoftBodyCylinder::CheckBodyState()
{
	for (int i = 0; i < POINT_X_NUM; i++)
	{
		m_points[i].SetState(PhysicalPoint::STATE::BAR);
		m_points[i + POINT_X_NUM].SetState(PhysicalPoint::STATE::BAR);
	}
}

//------------------------------------------
// 点と点をつなぐばねを作成（3次元）
//------------------------------------------
void SoftBodyCylinder::CreateArms()
{
	// コンテナ初期化
	m_springs.clear();

	// 円柱の円周部分を接続
	for (int i = 0; i < m_pointsIndices[SIDE].size() - 3; i += 4)
	{
		Spring s0(&m_points[m_pointsIndices[SIDE][i]], &m_points[m_pointsIndices[SIDE][i + 1]]);
		Spring s1(&m_points[m_pointsIndices[SIDE][i + 1]], &m_points[m_pointsIndices[SIDE][i + 2]]);
		Spring s2(&m_points[m_pointsIndices[SIDE][i + 2]], &m_points[m_pointsIndices[SIDE][i + 3]]);
		Spring s3(&m_points[m_pointsIndices[SIDE][i + 3]], &m_points[m_pointsIndices[SIDE][i]]);
		m_springs.push_back(s0);
		m_springs.push_back(s1);
		m_springs.push_back(s2);
		m_springs.push_back(s3);
	}
	// 奥の方と接続
	for (int i = 0; i < POINT_X_NUM / 2; i++)
	{
		Spring s0(&m_points[i], &m_points[i + POINT_X_NUM / 2]);
		int next = i + POINT_X_NUM;
		Spring s1(&m_points[next], &m_points[next + (POINT_X_NUM / 2)]);

		m_springs.push_back(s0);
		m_springs.push_back(s1);
	}

	// 余分なキャパ削除
	m_springs.shrink_to_fit();
}

//------------------------------------------
// 面を作成
//------------------------------------------
void SoftBodyCylinder::CreateFaces()
{

	// 面作成
	CreateTBFaces();
	CreateSideFaces();

	// ばねを作成
	CreateArms();

	for (auto& pointIndices : m_pointsIndices)
	{
		pointIndices.clear();
	}

}

//------------------------------------------
// 側面を作成
//------------------------------------------
void SoftBodyCylinder::CreateSideFaces()
{
	for (int i = 0; i < POINT_X_NUM; i++)
	{
		int a[4];
		if (i == POINT_X_NUM - 1)
		{
			a[0] = POINT_X_NUM - 1; a[1] = 0;
			a[2] = POINT_X_NUM * 2 - 1; a[3] = POINT_X_NUM;

			m_pointsIndices[SIDE].push_back(a[3]); m_pointsIndices[SIDE].push_back(a[1]);
			m_pointsIndices[SIDE].push_back(a[2]); m_pointsIndices[SIDE].push_back(a[0]);

			break;
		}
		// 左下の頂点				// 右下の頂点
		a[0] = i;					a[1] = i + 1;
		// 左上の頂点				// 右上の頂点
		a[2] = i + POINT_X_NUM;		a[3] = i + POINT_X_NUM + 1;

		m_pointsIndices[SIDE].push_back(a[3]); m_pointsIndices[SIDE].push_back(a[1]);
		m_pointsIndices[SIDE].push_back(a[2]); m_pointsIndices[SIDE].push_back(a[0]);

	}

	DeleteDuplication(SIDE);

}

//------------------------------------------
// 上と底面を作成
//------------------------------------------
void SoftBodyCylinder::CreateTBFaces()
{
	// 端の面は別で作成
	int a[4];
	// 右上の頂点			// 右下の頂点
	a[0] = 0;				a[1] = 1;
	// 左上の頂点			// 右上の頂点
	a[2] = POINT_X_NUM - 1;	a[3] = 2;
	// 底の面を作成
	m_pointsIndices[BOTTOM].push_back(a[0]); m_pointsIndices[BOTTOM].push_back(a[3]);
	m_pointsIndices[BOTTOM].push_back(a[1]); m_pointsIndices[BOTTOM].push_back(a[2]);
	// 上の面を作成
	int next = POINT_X_NUM;
	m_pointsIndices[TOP].push_back(a[3] + next); m_pointsIndices[TOP].push_back(a[2] + next);
	m_pointsIndices[TOP].push_back(a[0] + next); m_pointsIndices[TOP].push_back(a[1] + next);

	int j = 2;
	for (int i = POINT_X_NUM - 1; i > POINT_X_NUM / 2 + 1; i--)
	{
		// 左下の頂点	// 右下の頂点
		a[0] = i;		a[2] = i - 1;
		// 左上の頂点	// 右上の頂点
		a[1] = j;	a[3] = j + 1;
		// 底の面を作成
		m_pointsIndices[BOTTOM].push_back(a[0]); m_pointsIndices[BOTTOM].push_back(a[3]);
		m_pointsIndices[BOTTOM].push_back(a[1]); m_pointsIndices[BOTTOM].push_back(a[2]);
		// 上の面を作成
		m_pointsIndices[TOP].push_back(a[3] + next); m_pointsIndices[TOP].push_back(a[2] + next);
		m_pointsIndices[TOP].push_back(a[0] + next); m_pointsIndices[TOP].push_back(a[1] + next);
		
		j++;
	}

	DeleteDuplication(BOTTOM);
	DeleteDuplication(TOP);
}



//------------------------------------------
// 重複を削除
//------------------------------------------
void SoftBodyCylinder::DeleteDuplication(const Face& face)
{
	// コピー
	m_faceIndices[face] = m_pointsIndices[face];
	// 昇順にソート
	std::sort(m_faceIndices[face].begin(), m_faceIndices[face].end());
	// 重複を削除
	m_faceIndices[face].erase(
		std::unique(
			m_faceIndices[face].begin(),
			m_faceIndices[face].end()
		),
		m_faceIndices[face].end()
	);

	m_faceIndices[face].shrink_to_fit();
}

//------------------------------------------
// UV計算
//------------------------------------------
void SoftBodyCylinder::CalculateUV(const int& face)
{

	int index = 0;

	DirectX::SimpleMath::Vector2 sideUV[4] =
	{
		{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}
	};
	int indexOffset;
	switch (face)
	{
	case BOTTOM:

		// 円の形になっているか四隅は事前に設定する
		m_points[m_faceIndices[TOP][0]].SetUV(sideUV[0]);
		m_points[m_faceIndices[TOP][POINT_X_NUM / 4]].SetUV(sideUV[1]);
		m_points[m_faceIndices[TOP][POINT_X_NUM / 2]].SetUV(sideUV[3]);
		m_points[m_faceIndices[TOP][POINT_X_NUM / 4 * 3]].SetUV(sideUV[2]);

		m_points[m_faceIndices[BOTTOM][0]].SetUV(sideUV[0]);
		m_points[m_faceIndices[BOTTOM][POINT_X_NUM / 4]].SetUV(sideUV[1]);
		m_points[m_faceIndices[BOTTOM][POINT_X_NUM / 2]].SetUV(sideUV[3]);
		m_points[m_faceIndices[BOTTOM][POINT_X_NUM / 4 * 3]].SetUV(sideUV[2]);

		// UV座標を合わせるための配列オフセット
		indexOffset = POINT_X_NUM / 4 - 2;

		// 1～頂点の4分の1の数
		for (int i = 1; i < POINT_X_NUM / 4; i++)
		{
			float y = i * m_uvOffset.x;

			DirectX::SimpleMath::Vector2 uv[2] =
			{
				{0.0f, y}, {1.0f, y}
			};
			m_points[m_faceIndices[TOP][i]].SetUV(uv[0]);
			m_points[m_faceIndices[BOTTOM][i]].SetUV(uv[0]);
			int next = i + POINT_X_NUM / 2 + indexOffset;
			m_points[m_faceIndices[TOP][next]].SetUV(uv[1]);
			m_points[m_faceIndices[BOTTOM][next]].SetUV(uv[1]);

			indexOffset -= 2;
		}
		indexOffset = POINT_X_NUM / 4 - 2;
		
		index = POINT_X_NUM / 4 + 1;
		// 1～頂点数の4分の1 + 1
		for (int i = 1; i < POINT_X_NUM / 4; i++)
		{
			float x = i * m_uvOffset.x;

			DirectX::SimpleMath::Vector2 uv[2] =
			{
				{ x, 0.0f}, { x, 1.0f}
			};
			m_points[m_faceIndices[TOP][index]].SetUV(uv[1]);
			m_points[m_faceIndices[BOTTOM][index]].SetUV(uv[1]);
			int next = index + POINT_X_NUM / 2 + indexOffset;
			m_points[m_faceIndices[TOP][next]].SetUV(uv[0]);
			m_points[m_faceIndices[BOTTOM][next]].SetUV(uv[0]);
			index++;
			indexOffset -= 2;
		}
		break;
	case SIDE:
		for (int i = 0; i < POINT_Y_NUM; i++)
		{
			float y = i * m_uvOffset.y;

			for (int j = 0; j < POINT_X_NUM; j++)
			{
				DirectX::SimpleMath::Vector2 uv = { j * m_uvOffset.x, y };
				m_points[m_faceIndices[SIDE][index]].SetUV(uv);
				index++;
			}
		}
		break;
	default:
		break;
	}
}

//------------------------------------------
// インデックス計算
//------------------------------------------
void SoftBodyCylinder::CalculateIndex(const int& face)
{
	// 頂点のインデックス番号を計算
	std::vector<uint16_t> indices;
	uint16_t p[4];
	switch (face)
	{
	case BOTTOM:
		p[0] = 0;										p[1] = 1;
		p[2] = static_cast<uint16_t>(POINT_X_NUM - 1);	p[3] = 2;
		indices.push_back(p[0]);
		indices.push_back(p[1]);
		indices.push_back(p[2]);

		indices.push_back(p[1]);
		indices.push_back(p[2]);
		indices.push_back(p[3]);

		for (int i = 2; i < POINT_X_NUM / 2; i++)
		{
			p[0] = static_cast<uint16_t>(i);		p[1] = static_cast<uint16_t>(POINT_X_NUM - i + 1);
			p[2] = static_cast<uint16_t>(i + 1);	p[3] = static_cast<uint16_t>(POINT_X_NUM - i);

			indices.push_back(p[0]);
			indices.push_back(p[1]);
			indices.push_back(p[2]);

			indices.push_back(p[1]);
			indices.push_back(p[2]);
			indices.push_back(p[3]);
		}

		m_renders[TOP]->SetIndices(indices);
		m_renders[BOTTOM]->SetIndices(m_renders[TOP]->GetIndices());
		break;
	case SIDE:
		m_renders[SIDE]->CalculateIndex(POINT_X_NUM, POINT_Y_NUM);
		break;
	default:
		break;
	}

}
