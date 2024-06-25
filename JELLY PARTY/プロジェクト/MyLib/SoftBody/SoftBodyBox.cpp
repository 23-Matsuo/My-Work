//--------------------------------------------------------------------------------------
// File: SoftBodyBox.cpp
//
// 箱型ソフトボディクラス
//
// Last Update: 2023.02.05	
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "SoftBodyBox.h"

//------------------------------------------
// デフォルトコンストラクタ
//------------------------------------------
SoftBodyBox::SoftBodyBox()
	:
	m_colorChanged(false)
	, m_state(PhysicalPoint::STATE::FIXED)
{

}

//------------------------------------------
// コンストラクタ
//------------------------------------------
SoftBodyBox::SoftBodyBox(
	const int& xNum, const int& yNum, const int& zNum,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotation,
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Color& color,
	const PhysicalPoint::STATE& state)
	:
	POINT_X_NUM(xNum)
	, POINT_Y_NUM(yNum)
	, POINT_Z_NUM(zNum)
	, m_position(position)
	, m_initPosition(m_position)
	, m_rotation(rotation)
	, m_scale(scale)
	, m_color(color)
	, m_state(state)
	, m_colorChanged(false)
{
	// uvのオフセットを計算
	m_uvOffset.x = 1.0f / (POINT_X_NUM - 1);
	m_uvOffset.y = 1.0f / (POINT_Y_NUM - 1);
	m_uvOffset.z = 1.0f / (POINT_Z_NUM - 1);
}

//------------------------------------------
// 初期化
//------------------------------------------
void SoftBodyBox::Intialize(ID3D11ShaderResourceView* texture)
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
		// トップとレフトとフロントだけ計算する
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
			m_vertices[i].push_back(vertex);
		}
		// バッファを作成
		m_renders[i]->CreateBuffer(m_vertices[i]);
	}
}

//------------------------------------------
// 更新
//------------------------------------------
void SoftBodyBox::Update(const DX::StepTimer& timer)
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

	// 中心位置
	int num = static_cast<int>(m_points.size() - 1);
	m_position = (m_points[0].GetPosition() + m_points[num].GetPosition()) / 2;
	SetPosition(m_position);

	if (m_colorChanged)
	{
		for (int i = 0; i < m_renders.size(); i++)
		{
			m_renders[i]->SetColor(m_color);
		}
		m_colorChanged = false;
	}

}

//------------------------------------------
// 描画
//------------------------------------------
void SoftBodyBox::Render()
{
	// 面を描画
	for (auto& render : m_renders)
	{
		render->Render();
	}
}

//------------------------------------------
// 影描画
//------------------------------------------
void SoftBodyBox::ShadowRender(
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
void SoftBodyBox::UpdateRenderPosition()
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
/// ソフトボディを形成（3次元）
//------------------------------------------
void SoftBodyBox::CreateFigure()
{
	// コンテナ初期化
	m_points.clear();

	int ID = 1;
	// Z軸の点の数分ループ
	for (int h = 0; h < POINT_Z_NUM; h++)
	{
		// Y軸の点の数分ループ
		for (int i = 0; i < POINT_Y_NUM; i++)
		{
			// X軸の点の数分ループ
			for (int j = 0; j < POINT_X_NUM; j++)
			{
				// 座標計算(j - 点の数から1引いて÷2した数) 
				// 点の数が偶数なら.5から始まる、奇数なら整数
				float x = (j - static_cast<float>(POINT_X_NUM - 1) / 2.0f);
				float y = (i - static_cast<float>(POINT_Y_NUM - 1) / 2.0f);
				float z = (h - static_cast<float>(POINT_Z_NUM - 1) / 2.0f);

				// Vector3に変換
				DirectX::SimpleMath::Vector3 pos = { x,y,z };
				// 拡縮
				pos *= m_scale;
				// 回転
				pos = DirectX::SimpleMath::Vector3::Transform(pos, m_rotation);
				// 移動
				pos += m_position;
				// 点のポジション設定
				PhysicalPoint p(pos, ID);
				// vector配列に格納
				m_points.push_back(p);

				ID++;
			}
		}
	}
	// 余分なキャパ削除
	m_points.shrink_to_fit();
	// ソフトボディの状態を見る
	//CheckBodyState();
	// ばねを作成
	CreateArms();
	// 右回りに面を作成
	CreateFaces();
}

//------------------------------------------
// 点と点をつなぐばねを作成（3次元）
//------------------------------------------
void SoftBodyBox::CreateArms()
{
	// 右,右上、上、左上、奥・・・に点があるか

	//// 斜め奥を繋げない
	//const static int diagonally[7][3] = { 
	//	{1, 0, 0}, {1, -1, 0}, {0, -1, 0}, {-1, -1, 0},
	//	{0, 0, 1}, {0, -1, 1}, {0, -1, -1},
	//};

	//// 斜め奥を少しだけ繋げる
	//const static int diagonally[11][3] = { 
	//	{1, 0, 0}, {1, -1, 0}, {0, -1, 0}, 
	//	{-1, -1, 0}, {0, 0, 1}, {1, 0, 1},
	//	{0, -1, 1}, {0, -1, -1}, {-1, 0, -1},
	//  {1, 0, -1}
	//};

	// 全方向を繋げる
	const static int diagonally[14][3] = {
		{1, 0, 0}, {1, -1, 0}, {0, -1, 0},			// 右, 右下, 下
		{-1, -1, 0}, {0, 0, 1}, {1, 0, 1},			// 左下, 奥, 右奥
		{1, -1, 1}, {0, -1, 1}, {0, -1, -1},		// 右下奥, 下奥, 下手前
		{-1, -1, -1} , {-1, 0, -1},	{1, 0, -1},		// 左下手前, 左手前, 右手前
		{1, -1, -1}, {1, 1, -1}						// 右下手前, 右上手前
	};

	// コンテナ初期化
	m_springs.clear();

	for (int h = 0; h < POINT_Z_NUM; h++)
	{
		for (int i = 0; i < POINT_Y_NUM; i++)
		{
			for (int j = 0; j < POINT_X_NUM; j++)
			{
				// 点の右上右下左上左下に点があるか
				for (const auto& dia : diagonally)
				{
					// dia = -1～1の値
					int	x = j + dia[0];
					int y = i + dia[1];
					int z = h + dia[2];

					// もしaとbが点の数の範囲外じゃなかったら
					if (x >= 0 && x < POINT_X_NUM &&
						y >= 0 && y < POINT_Y_NUM &&
						z >= 0 && z < POINT_Z_NUM)
					{
						// 参照する頂点のポインタの位置を計算
						int a = h * POINT_X_NUM * POINT_Y_NUM + i * POINT_X_NUM + j;

						// 他の頂点のポインタの位置を計算
						int b = z * POINT_X_NUM * POINT_Y_NUM + y * POINT_X_NUM + x;
						// ばねの作成
						Spring spring(&m_points[a], &m_points[b]);
						// ばねの情報を保存
						m_springs.push_back(spring);
					}
				}
			}
		}
	}
	// 余分なキャパ削除
	m_springs.shrink_to_fit();
}

//------------------------------------------
// 面を作成
//------------------------------------------
void SoftBodyBox::CreateFaces()
{

	// 面作成
	CreateTBFaces();
	CreateLRFaces();
	CreateFBFaces();

	for (auto& pointIndices : m_pointsIndices)
	{
		pointIndices.clear();
	}

}

//------------------------------------------
// 前と後ろ面を作成
//------------------------------------------
void SoftBodyBox::CreateFBFaces()
{
	int j = 0, row = 1;
	// 頂点の合計数
	int totalPointNum = static_cast<int>(m_points.size());

	// 条件① xNum * yNumから1列分減らした数の頂点から描画する
	//（例：xの頂点数が３でyも３の時、iが0～1,3～4の時に四角形ポリゴンを描画する)
	for (int i = 0; i < POINT_X_NUM * POINT_Y_NUM - POINT_X_NUM - 1; i++)
	{
		// 条件② i が xNum - 1 * 行数 より 大きくなったら 行数を更新
		if (i > POINT_X_NUM * row - 1)
		{
			row++; j++;
		}
		// i が ０の時は無視
		if (i != 0)
		{
			// 条件③ i が xNum - 1 + xNum * j がiだったら端っこなのでiを更新する
			if (i == (POINT_X_NUM - 1 + POINT_X_NUM * j))
			{
				i++;
			}
		}
		int a[4];
		// 左下の頂点           // 右下の頂点
		a[0] = i;				a[2] = i + 1;
		// 左上の頂点			// 右上の頂点
		a[1] = i + POINT_X_NUM;	a[3] = i + POINT_X_NUM + 1;
		// 奥の面を作成
		m_pointsIndices[Face::BACK].push_back(a[0]); m_pointsIndices[Face::BACK].push_back(a[2]);
		m_pointsIndices[Face::BACK].push_back(a[3]); m_pointsIndices[Face::BACK].push_back(a[1]);

		int next = totalPointNum - (POINT_X_NUM * POINT_Y_NUM);

		// 手前の面を作成
		m_pointsIndices[Face::FRONT].push_back(a[3] + next); m_pointsIndices[Face::FRONT].push_back(a[2] + next);
		m_pointsIndices[Face::FRONT].push_back(a[0] + next); m_pointsIndices[Face::FRONT].push_back(a[1] + next);
	}

	DeleteDuplication(BACK);
	DeleteDuplication(FRONT);

}

//------------------------------------------
// 上と底面を作成
//------------------------------------------
void SoftBodyBox::CreateTBFaces()
{
	int j = 0, row = 0;
	int totalPointNum = static_cast<int>(m_points.size());

	// 条件① xNum * yNumから1列分減らした数の頂点から描画する
	//（例：xの頂点数が３でyも３の時、iが0～1,3～4の時に四角形ポリゴンを描画する)
	for (int i = 0; i < totalPointNum - POINT_Y_NUM; i++)
	{
		// 条件② i が xNum - 1 * 行数 より 大きくなったら 行数を更新
		if (i > POINT_X_NUM + (POINT_X_NUM * POINT_Y_NUM) * row - 1)
		{
			row++;	j += POINT_Y_NUM;
		}
		// i が ０の時は無視
		if (i != 0)
		{
			// 条件③ i が xNum - 1 + xNum * j がiだったら端っこなのでi更新する
			if (i == (POINT_X_NUM - 1 + POINT_X_NUM * j))
			{
				i += (POINT_X_NUM * POINT_Y_NUM) - (POINT_X_NUM - 1);
			}
		}
		int a[4];
		// 左下の頂点						// 右下の頂点
		a[0] = i;								a[1] = i + 1;
		// 左上の頂点						// 右上の頂点
		a[2] = i + POINT_X_NUM * POINT_Y_NUM;	a[3] = i + POINT_X_NUM * POINT_Y_NUM + 1;
		// 底の面を作成
		m_pointsIndices[BOTTOM].push_back(a[3]); m_pointsIndices[BOTTOM].push_back(a[2]);
		m_pointsIndices[BOTTOM].push_back(a[0]); m_pointsIndices[BOTTOM].push_back(a[1]);
		// 上の面を作成
		int next = (POINT_X_NUM * POINT_Y_NUM) - POINT_X_NUM;
		// 上の面を作成
		m_pointsIndices[TOP].push_back(a[0] + next); m_pointsIndices[TOP].push_back(a[1] + next);
		m_pointsIndices[TOP].push_back(a[3] + next); m_pointsIndices[TOP].push_back(a[2] + next);

		// 最後の頂点にいったら抜ける
		if (a[3] == totalPointNum - POINT_X_NUM * POINT_Y_NUM + POINT_X_NUM - 1)
		{
			break;
		}
	}

	DeleteDuplication(BOTTOM);
	DeleteDuplication(TOP);
}

//------------------------------------------
// 側面を作成
//------------------------------------------
void SoftBodyBox::CreateLRFaces()
{
	int j = 0, row = 1;
	int totalPointNum = static_cast<int>(m_points.size());

	// 条件① xNum * yNumから1列分減らした数の頂点から描画する
	//（例：xの頂点数が３でyも３の時、iが0～1,3～4の時に四角形ポリゴンを描画する)
	for (int i = 0; i < totalPointNum - POINT_X_NUM; i += POINT_X_NUM)
	{
		// 条件② i が xNum - 1 * 行数 より 大きくなったら 行数を更新
		if (i > (POINT_X_NUM * POINT_Y_NUM) * row - 1)
		{
			row++; j += POINT_Y_NUM;
		}
		// i が ０の時は無視
		if (i != 0)
		{
			// 条件③ i が xNum - 1 + xNum * j がiだったら端っこなのでiを更新する
			if (i == (POINT_X_NUM * POINT_Y_NUM - POINT_X_NUM + POINT_X_NUM * j))
			{
				i += POINT_X_NUM;
			}
		}
		int a[4];
		// 左下の頂点							// 右下の頂点
		a[0] = i;								a[1] = i + POINT_X_NUM;
		// 左上の頂点							// 右上の頂点
		a[2] = i + POINT_X_NUM * POINT_Y_NUM;	a[3] = i + POINT_X_NUM * POINT_Y_NUM + POINT_X_NUM;
		// 左の面を作成
		m_pointsIndices[Face::LEFT].push_back(a[3]); m_pointsIndices[Face::LEFT].push_back(a[2]);
		m_pointsIndices[Face::LEFT].push_back(a[0]); m_pointsIndices[Face::LEFT].push_back(a[1]);
		// 右の面を作成
		int next = POINT_X_NUM - 1;
		m_pointsIndices[Face::RIGHT].push_back(a[0] + next); m_pointsIndices[Face::RIGHT].push_back(a[1] + next);
		m_pointsIndices[Face::RIGHT].push_back(a[3] + next); m_pointsIndices[Face::RIGHT].push_back(a[2] + next);

		// 最後の頂点にいったら抜ける
		if (a[3] == totalPointNum - POINT_X_NUM)
		{
			break;
		}
	}

	DeleteDuplication(LEFT);
	DeleteDuplication(RIGHT);
}

//------------------------------------------
// 重複を削除
//------------------------------------------
void SoftBodyBox::DeleteDuplication(const Face& face)
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
// 回転
//------------------------------------------
void SoftBodyBox::Rotate(
	DirectX::SimpleMath::Matrix rotation)
{
	for (int i = 0; i < m_points.size(); i++)
	{
		//// 回転前の位置を記憶
		//m_points[i].SetPrevPosition(GetPosition());

		// 位置を回転
		m_points[i].SetPosition(
			DirectX::SimpleMath::Vector3::Transform(m_points[i].GetPosition(), rotation));
	}
	UpdateRenderPosition();

}

//------------------------------------------
// モデルとの衝突判定
//------------------------------------------
void SoftBodyBox::CheckHitModel2Point(Imase::ObjCollision* pObj)
{
	DirectX::SimpleMath::Vector3 line = { 0.0f, -1.0f,0.0f };
	// 反発係数
	float restitution = 0.8f;
	for (PhysicalPoint& p : m_points)
	{
		line.x = p.GetPosition().x;
		line.z = p.GetPosition().z;
		DirectX::SimpleMath::Vector3 hitPosition;
		DirectX::SimpleMath::Vector3 normal;
		// 線分と床の交差判定を行う
		if (pObj->IntersectLineSegment(line, p.GetPosition(), &hitPosition, &normal))
		{
			// 反射ベクトルを計算
			DirectX::SimpleMath::Vector3 force = p.GetVector();
			force = -force * restitution;

			// 力更新
			p.UpdateVector(force);

			// 位置を設定
			p.SetPosition(hitPosition);
		}
	}
}


//------------------------------------------
// UV計算
//------------------------------------------
void SoftBodyBox::CalculateUV(const int& face)
{
	int index = 0;

	switch (face)
	{
	case TOP:
		for (int i = 0; i < POINT_Z_NUM; i++)
		{
			float y = i * m_uvOffset.z;

			for (int j = 0; j < POINT_X_NUM; j++)
			{
				DirectX::SimpleMath::Vector2 uv = { j * m_uvOffset.x, y };
				m_points[m_faceIndices[TOP][index]].SetUV(uv);
				m_points[m_faceIndices[BOTTOM][index]].SetUV(uv);
				index++;
			}
		}
		break;
	case LEFT:
		for (int i = 0; i < POINT_Z_NUM; i++)
		{
			float y = i * m_uvOffset.z;

			for (int j = 0; j < POINT_Y_NUM; j++)
			{
				DirectX::SimpleMath::Vector2 uv = { j * m_uvOffset.y, y };
				m_points[m_faceIndices[LEFT][index]].SetUV(uv);
				m_points[m_faceIndices[RIGHT][index]].SetUV(uv);
				index++;
			}
		}
		break;
	case FRONT:
		for (int i = 0; i < POINT_Y_NUM; i++)
		{
			float y = i * m_uvOffset.y;
			for (int j = 0; j < POINT_X_NUM; j++)
			{
				DirectX::SimpleMath::Vector2 uv = { j * m_uvOffset.x, y };
				m_points[m_faceIndices[FRONT][index]].SetUV(uv);
				m_points[m_faceIndices[BACK][index]].SetUV(uv);
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
void SoftBodyBox::CalculateIndex(const int& face)
{
	// 頂点のインデックス番号を計算
	switch (face)
	{
	case TOP:
		m_renders[TOP]->CalculateIndex(POINT_X_NUM, POINT_Z_NUM);
		m_renders[BOTTOM]->SetIndices(m_renders[TOP]->GetIndices());
		break;
	case LEFT:
		m_renders[LEFT]->CalculateIndex(POINT_Y_NUM, POINT_Z_NUM);
		m_renders[RIGHT]->SetIndices(m_renders[LEFT]->GetIndices());
		break;
	case FRONT:
		m_renders[FRONT]->CalculateIndex(POINT_X_NUM, POINT_Y_NUM);
		m_renders[BACK]->SetIndices(m_renders[FRONT]->GetIndices());
		break;
	default:
		break;
	}
}


//------------------------------------------
// パラメータを初期化
//------------------------------------------
void SoftBodyBox::ResetParameters(DirectX::SimpleMath::Vector3 position)
{
	for (auto& p : m_points)
	{
		p.ParameterReset(position);
	}
}

//------------------------------------------
// 頂点数を設定
//------------------------------------------
void SoftBodyBox::SetPointsNum(const int& x, const int& y, const int& z)
{
	POINT_X_NUM = x;
	POINT_Y_NUM = y;
	POINT_Z_NUM = z;
}

//------------------------------------------
// 描画オブジェクトの設定
//------------------------------------------
void SoftBodyBox::SetRenders(const std::array<std::unique_ptr<RenderSoftBody>, Face::FACENUM>& renders)
{

	for (int i = 0; i < FACENUM; i++)
	{
		// レンダーの作成
		m_renders[i] = std::make_unique<RenderSoftBody>();

		renders[i]->Clone(m_renders[i].get(), m_vertices[i]);
	}
}

//------------------------------------------
// 色を設定
//------------------------------------------
void SoftBodyBox::SetColor(const DirectX::SimpleMath::Color& color)
{
	// 色が変わった
	m_colorChanged = true;
	m_color = color;
}

//------------------------------------------
// インデックスを設定
//------------------------------------------
void SoftBodyBox::SetFaceIndices(const std::array<std::vector<unsigned int>, Face::FACENUM>& faceIndices)
{
	for (int i = 0; i < Face::FACENUM; i++)
	{
		m_faceIndices[i] = faceIndices[i];
	}
}

//------------------------------------------
// 頂点を設定
//------------------------------------------
void SoftBodyBox::SetVertices(const std::array<std::vector<DirectX::VertexPositionColorTexture>, Face::FACENUM>& vertices)
{
	for (int i = 0; i < Face::FACENUM; i++)
	{
		m_vertices = vertices;
	}
}

//------------------------------------------
// 頂点の位置を設定
//------------------------------------------
void SoftBodyBox::SetPointsPosition()
{
	for (int i = 0; i < m_points.size(); i++)
	{
		DirectX::SimpleMath::Vector3 temp = m_points[i].GetPosition();
		m_points[i].SetPosition(m_points[i].GetPosition() + m_points[i].GetInitPosition());
	}
}

//------------------------------------------
// リザルトの初期化
//------------------------------------------
void SoftBodyBox::ResultReset(const DirectX::SimpleMath::Vector3& position)
{
	for (auto& p : m_points)
	{
		p.ResultReset(position);
	}
}
