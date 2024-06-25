//--------------------------------------------------------------------------------------
// File: SoftBodyCube.h
//
// 落ちてくるゼリー
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "SoftBodyCube.h"

#include "MyLib/ObjectCollision.h"
#include "Scene/GameManager.h"
#include <random>
#include "MyLib/SoundUtility.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
SoftBodyCube::SoftBodyCube()
	:
	m_cubeIndex(0)
	, m_appeared(false)
{
	auto deviceResources = Graphics::GetInstance()->GetDeviceResources();
	auto device = deviceResources->GetD3DDevice();
	auto context = deviceResources->GetD3DDeviceContext();

	// テクスチャを設定
	m_texture = Resources::GetInstance()->GetTexture(L"Jelly").texture;
	// 衝突判定の表示オブジェクトの作成
	m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);

	// パラメータを読み込む
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/JellyCubeParameter");

	// スポーンのインターバル
	m_spawnTime = m_parameters["SPAWN_INTERVAL"];

	// キューブの数
	m_cubeMaxNum = m_parameters["CUBE_MAX_NUM"];
	// キューブの数
	m_cubeNum = m_parameters["CUBE_NUM"];

	CreateCube();

	for (int i = 0; i < m_cubeMaxNum - m_cubeNum; i++)
	{
		DupricateCube();
	}

	// 念のためキャパをサイズに合わせる
	m_boxes.shrink_to_fit();

	m_getParticle = std::make_unique<GetParticle>();
	m_getParticle->Create(deviceResources);

	m_hitSound = Resources::GetInstance()->GetSound(L"HitJelly");
}

//------------------------------------------
// デストラクタ
//------------------------------------------
SoftBodyCube::~SoftBodyCube()
{
	m_boxes.clear();
}

//------------------------------------------
// キューブの作成
//------------------------------------------
void SoftBodyCube::CreateCube()
{
	// スポーン範囲
	const float SPAWN_RANGE = static_cast<float>(m_parameters["SPAWN_RANGE"]);
	// スポーンの高さ
	const float SPAWN_HEIGHT = static_cast<float>(m_parameters["SPAWN_HEIGHT"]);
	// スケール分布の範囲
	const float SCALE_DIST_MIN = static_cast<float>(m_parameters["SCALE_DIST_MIN"]);
	const float SCALE_DIST_MAX = static_cast<float>(m_parameters["SCALE_DIST_MAX"]);
	// 色分布の範囲
	const float COLOR_DIST_MIN = static_cast<float>(m_parameters["COLOR_DIST_MIN"]);
	const float COLOR_DIST_MAX = static_cast<float>(m_parameters["COLOR_DIST_MAX"]);

	// 1つのキューブの頂点の数
	const int POINT_NUM = m_parameters["POINT_NUM"];

	// ソフトボディを作成
	std::random_device seed;
	std::mt19937 engine(seed());
	// ランダムな位置
	std::uniform_real_distribution<float> positionDist(-SPAWN_RANGE, SPAWN_RANGE);
	// ランダムな回転
	std::uniform_real_distribution<float> radianDist(-DirectX::XM_PI, DirectX::XM_PI);
	std::uniform_int_distribution<>		  rotationDist(0, 1);
	// ランダムな色
	std::uniform_real_distribution<float> colorDist(COLOR_DIST_MIN, COLOR_DIST_MAX);
	// ランダムなスケール
	std::uniform_real_distribution<float> scaleDist(SCALE_DIST_MIN, SCALE_DIST_MAX);

	// 回転軸をランダムに決める
	int pivot = rotationDist(engine);
	DirectX::SimpleMath::Matrix rotation;
	if (pivot == 0)
	{
		rotation = DirectX::SimpleMath::Matrix::CreateRotationX(radianDist(engine));
	}
	else
	{
		rotation = DirectX::SimpleMath::Matrix::CreateRotationX(radianDist(engine));
	}

	// ランダムスケール
	float scale = scaleDist(engine);

	// ランダム位置
	DirectX::SimpleMath::Vector3 randomPos = { positionDist(engine),positionDist(engine) + SPAWN_HEIGHT ,positionDist(engine) };

	// ソフトボディを作成
	m_boxes.push_back(
		SoftBodyFactory::CreateBox(
			POINT_NUM, POINT_NUM, POINT_NUM,
			randomPos,
			{},
			DirectX::SimpleMath::Vector3(scale, scale, scale),
			DirectX::SimpleMath::Color(colorDist(engine), colorDist(engine), colorDist(engine), 1.0f),
			m_texture.Get(),
			PhysicalPoint::STATE::UNFIXED
		)
	);

	DirectX::BoundingSphere sphere(m_boxes[m_boxes.size() - 1]->GetPosition(), scale / 2.0f);
	m_collisions.push_back(sphere);
}


//------------------------------------------
// 更新
//------------------------------------------
void SoftBodyCube::Update(const int& spendingTime, CenterSoftBody* pCylinder)
{
	auto timer = Graphics::GetInstance()->GetStepTimer();

	m_spendingTime = spendingTime;

	// キューブを出現
	AppearCubes();

	for (int i = 0; i < m_cubeNum; i++)
	{
		// 更新
		CubesUpdate(i);

		// モデルと頂点の当たり判定
		ObjectCollision::GetInstance()->PointsToModel(m_boxes[i].get(), 0.01f);

		for (int j = 0; j < m_boxes.size(); j++)
		{
			if (i == j) continue;
			if (m_cubeNum == 1) continue;
			// ゼリーキューブ同士の当たり判定
			ObjectCollision::GetInstance()->CubeToOtherCube(
				m_boxes[i].get(), m_collisions[i],
				m_boxes[j].get(), m_collisions[j]
			);
		}

		// 球とゼリーキューブの当たり判定
		if (ObjectCollision::GetInstance()->CatToCube(m_collisions[i]))
		{
			// 当たっていたら作成
			m_getParticle->CreateGetParticle(m_boxes[i]->GetPosition(), m_boxes[i]->GetColor());
			// 再生
			m_getParticle->SetState(GetParticle::State::PLAY);

			// ヒット音を鳴らす
			PlaySound(m_hitSound, false);

			// パラメータ初期化
			ResetParameters(i);
		}

		// リセット位置
		const float RESET_HEIGHT = 30;

		// 現在の位置
		DirectX::SimpleMath::Vector3 position = m_boxes[i]->GetPosition();
		// 初期位置
		DirectX::SimpleMath::Vector3 initialPosition = m_boxes[i]->GetInitialPosition();

		// 現在の位置が初期位置 - リセット位置より低くなったら初期化
		if (position.y <= initialPosition.y - RESET_HEIGHT)
		{
			ResetParameters(i);
		}

		// 中心のゼリーとキューブの衝突判定
		ObjectCollision::GetInstance()->ShaftToCube(pCylinder, m_boxes[i].get(), m_collisions[i]);
	}

	// 取得パーティクルの更新
	m_getParticle->Update(*timer);
}

//------------------------------------------
// 描画
//------------------------------------------
void SoftBodyCube::Render()
{
	if (m_boxes.size() == 0) return;

	for (int i = 0; i < m_cubeNum; i++)
	{
		// ソフトボディテクスチャ描画
		m_boxes[i]->Render();
	}

	// パーティクル描画
	m_getParticle->CreateBillboard(DirectX::SimpleMath::Vector3::Up);
	m_getParticle->Render();
}


//------------------------------------------
// 影描画
//------------------------------------------
void SoftBodyCube::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	for (int i = 0; i < m_cubeNum; i++)
	{
		// ソフトボディ影描画
		m_boxes[i]->ShadowRender(lightView, lightProjection);
	}
}

//------------------------------------------
// 頂点の状態を設定
//------------------------------------------
void SoftBodyCube::CubesUpdate(const int& index)
{
	auto timer = Graphics::GetInstance()->GetStepTimer();

	// ソフトボディの更新
	m_boxes[index]->Update(*timer);

	// 頂点取得
	SoftBodyBox::Points points = *m_boxes[index]->GetPoints();

	DirectX::SimpleMath::Vector3 p1 = points[0].GetPosition();
	DirectX::SimpleMath::Vector3 p2 = points[points.size() - 1].GetPosition();

	// 中心点を設定
	DirectX::SimpleMath::Vector3 center = (p1 + p2) / 2;
	m_collisions[index].Center = center;
}

//------------------------------------------
// バーとゼリーキューブの当たり判定
//------------------------------------------
void SoftBodyCube::HitBar(const int& index, SoftBodyBar* pBar, const float& speed)
{
	const float FORCE_RATIO = 25.0f;
	for (int i = 0; i < pBar->BAR_NUM; i++)
	{
		// 当たった頂点からゼリーキューブまでの向き
		DirectX::SimpleMath::Vector3 direction;
		// バーとゼリーキューブの衝突判定
		if (ObjectCollision::GetInstance()->BarToCubeBody(
			m_collisions[index], pBar->GetSoftBodies(i), &direction)
			)
		{
			// 力の向きを計算
			DirectX::SimpleMath::Vector3 newForce =
				-direction * speed * FORCE_RATIO;
			newForce.y /= FORCE_RATIO;

			// 頂点全てに力を設定
			SoftBodyBox::Points& points = *m_boxes[index]->GetPoints();
			for (auto& point : points)
			{
				point.UpdateVector(newForce);
			}
		}
	}
}


//------------------------------------------
// ゼリーキューブを出現させる
//------------------------------------------
void SoftBodyCube::AppearCubes()
{
	if (m_spendingTime == 0 || m_spendingTime % m_spawnTime != 0) return;
	if (m_cubeMaxNum == m_cubeNum) return;

	m_cubeNum++;
}


//------------------------------------------
// キューブを複製する
//------------------------------------------
void SoftBodyCube::DupricateCube()
{
	std::unique_ptr<SoftBodyBox> box;
	box = std::make_unique<SoftBodyBox>();
	// クローンを生成
	m_boxes[0]->Clone(box.get());

	const float SPAWN_RANGE = static_cast<float>(m_parameters["SPAWN_RANGE"]);
	const float SPAWN_HEIGHT = static_cast<float>(m_parameters["SPAWN_HEIGHT"]);

	const float COLOR_DIST_MIN = static_cast<float>(m_parameters["COLOR_DIST_MIN"]);
	const float COLOR_DIST_MAX = static_cast<float>(m_parameters["COLOR_DIST_MAX"]);

	// ソフトボディを作成
	std::random_device seed;
	std::mt19937 engine(seed());
	// ランダムな位置
	std::uniform_real_distribution<float> positionDist(-SPAWN_RANGE, SPAWN_RANGE);
	DirectX::SimpleMath::Vector3 randomPos(positionDist(engine), positionDist(engine) + SPAWN_HEIGHT, positionDist(engine));

	// ランダムな色
	std::uniform_real_distribution<float> colorDist(COLOR_DIST_MIN, COLOR_DIST_MAX);
	DirectX::SimpleMath::Color newColor(colorDist(engine), colorDist(engine), colorDist(engine), 1.0f);

	box->ResetParameters(randomPos);
	box->SetColor(newColor);

	m_boxes.push_back(std::move(box));

	// 衝突判定を作成
	DirectX::BoundingSphere sphere(m_boxes[m_boxes.size() - 1]->GetPosition(), m_boxes[0]->GetScale().x / 1.8f);
	m_collisions.push_back(sphere);

}

//------------------------------------------
// パラメータを初期化
//------------------------------------------
void SoftBodyCube::ResetParameters(const int& index)
{
	const float SPAWN_RANGE = static_cast<float>(m_parameters["SPAWN_RANGE"]);
	const float SPAWN_HEIGHT = static_cast<float>(m_parameters["SPAWN_HEIGHT"]);

	const float COLOR_DIST_MIN = static_cast<float>(m_parameters["COLOR_DIST_MIN"]);
	const float COLOR_DIST_MAX = static_cast<float>(m_parameters["COLOR_DIST_MAX"]);

	// ソフトボディを作成
	std::random_device seed;
	std::mt19937 engine(seed());
	// ランダムな位置
	std::uniform_real_distribution<float> positionDist(-SPAWN_RANGE, SPAWN_RANGE);
	DirectX::SimpleMath::Vector3 randomPos(positionDist(engine), positionDist(engine) + SPAWN_HEIGHT, positionDist(engine));

	// ランダムな色
	std::uniform_real_distribution<float> colorDist(COLOR_DIST_MIN, COLOR_DIST_MAX);
	DirectX::SimpleMath::Color newColor(colorDist(engine), colorDist(engine), colorDist(engine), 1.0f);

	m_boxes[index]->ResetParameters(randomPos);

	m_boxes[index]->SetColor(newColor);
}

//------------------------------------------
// 頂点の状態を設定
//------------------------------------------
void SoftBodyCube::SetPointsState()
{
	for (int i = 0; i < m_cubeNum; i++)
	{
		SoftBodyBox::Points& points = *m_boxes[i]->GetPoints();

		for (auto& p : points)
		{
			p.SetState(PhysicalPoint::STATE::UNFIXED);
		}
	}
}

//------------------------------------------
// リザルト時の初期化
//------------------------------------------
void SoftBodyCube::ResultInitialize(Score* pScore)
{
	m_pScore = pScore;
	m_timer = 0;
	m_interval = 0.0f;

	m_resultResetPosition = DirectX::SimpleMath::Vector3(0.0f, -10.0f, -7.0f);

	// 今回のスコア
	const int NOW_SCORE = GameManager::GetInstance()->GetNowScore();

	// スコアの数が出現しているキューブの量より大きい
	if (m_boxes.size() < NOW_SCORE)
	{
		// スコアの数がキューブの最大数より大きい場合
		if (NOW_SCORE > m_cubeMaxNum)
		{
			// 最大数まで作成
			for (int i = m_boxes.size(); i < m_cubeMaxNum; i++)
			{
				DupricateCube();
			}
		}
		else
		{
			// スコアの数まで作成
			for (int i = m_boxes.size(); i < NOW_SCORE; i++)
			{
				DupricateCube();
			}
		}
	}

	for (int i = 0; i < m_boxes.size(); i++)
	{
		m_boxes[i]->ResultReset(m_resultResetPosition);
	}
}

//------------------------------------------
// リザルト時の更新
//------------------------------------------
void SoftBodyCube::ResultUpdate()
{
	auto timer = Graphics::GetInstance()->GetStepTimer();
	float elapsedTime = static_cast<float>(timer->GetElapsedSeconds());

	const int INTERVAL = 5;

	// 今回のスコア
	const int NOW_SCORE = GameManager::GetInstance()->GetNowScore();

	m_timer++;
	if (m_timer % INTERVAL == 0)
	{
		// カウントが取得した数と同じだったら
		if (m_pScore->GetScore() == NOW_SCORE)
		{
			m_interval += elapsedTime;
			// キャラが正面を向くまでの時間
			if (m_interval > 0.2f)
			{
				// カウント終了
				m_pScore->SetResultCountFinish(true);
			}
			return;
		}

		// カウントアップ
		m_pScore->CountUp();

		if (m_pScore->GetScore() >= NOW_SCORE)
		{
			// カウントが取得した数以上だったら取得した数に設定
			m_pScore->SetScore(NOW_SCORE);
		}
		else
		{
			// ヒット音を鳴らす
			PlaySound(m_hitSound, false);
		}
	}

	// カウント終了した
	if (!m_pScore->GetResultCountFinish()) return;

	// スコアがキューブの最大数より大きければリザルトで出すキューブの数を制限
	if (NOW_SCORE > m_cubeMaxNum)
	{
		m_cubeIndex = m_cubeMaxNum;
	}
	else
	{
		m_cubeIndex = NOW_SCORE;
	}

	for (int i = 0; i < m_cubeIndex; i++)
	{
		CubesUpdate(i);

		// モデルと頂点の当たり判定
		ObjectCollision::GetInstance()->PointsToModel(m_boxes[i].get(), 0.01f);

		for (int j = 0; j < m_cubeIndex; j++)
		{
			if (i == j) continue;
			// ゼリーキューブ同士の当たり判定
			ObjectCollision::GetInstance()->CubeToOtherCube(
				m_boxes[i].get(), m_collisions[i],
				m_boxes[j].get(), m_collisions[j]
			);
		}
	}
}

//------------------------------------------
// リザルト時の描画
//------------------------------------------
void SoftBodyCube::ResultRender()
{
	if (!m_pScore->GetResultCountFinish()) return;


	for (int i = 0; i < m_cubeIndex; i++)
	{
		// ソフトボディテクスチャ描画
		m_boxes[i]->Render();
	}
}


//------------------------------------------
// 影描画
//------------------------------------------
void SoftBodyCube::ResultShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	for (int i = 0; i < m_cubeIndex; i++)
	{
		// ソフトボディ影描画
		m_boxes[i]->ShadowRender(lightView, lightProjection);
	}
}