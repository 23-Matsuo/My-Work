//--------------------------------------------------------------------------------------
// File: Sphere.cpp
//
// プレイヤークラス
//
// Last Update: 2023.10.31
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "CatSphere.h"
#include "MyLib/Resources.h"
#include "MyLib/Graphics.h"
#include "MyLib/ObjectCollision.h"
#include "MyLib/SoundUtility.h"

using namespace DirectX;


//------------------------------------------
// コンストラクタ
//------------------------------------------
CatSphere::CatSphere()
	:
	m_collider(SimpleMath::Vector3::Zero, 0.0f)
	, m_isHit(false)
	, m_isDead(false)
	, m_landParticlePlayed(false)
	, m_jumpForce(0.0f)
	, m_gravityAcceleration{}
	, m_model(nullptr)
	, m_animated(false)
	, m_timer(0.0f)
	, m_resultState(ResultState::IDLE)
	, m_running(this)
	, m_jumping(this)
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	// 当たり判定表示用
	m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);

	// モデル設定
	m_model = Resources::GetInstance()->GetModel(L"Sphere");

	// 衝突判定にポインタを渡す
	ObjectCollision::GetInstance()->SetCatBody(this);

	// パーティクル作成
	m_landParticle = std::make_unique<LandParticle>();
	m_landParticle->Create(Graphics::GetInstance()->GetDeviceResources());
	m_jumpParticle = std::make_unique<JumpParticle>();
	m_jumpParticle->Create(Graphics::GetInstance()->GetDeviceResources());
	m_runParticle = std::make_unique<RunParticle>();
	m_runParticle->Create(Graphics::GetInstance()->GetDeviceResources());

	// プレイヤーのパラメータを読み込む
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/PlayerParameter");

	m_radius = static_cast<float>(m_parameters["RADIUS"]);
	m_collider.Radius = m_radius;

	// 腕を作成する
	m_arm = std::make_unique<Arm>(this);

	// 現在の状態をランニングにする
	m_currentState = GetRunning();
}


//------------------------------------------
// デストラクタ
//------------------------------------------
CatSphere::~CatSphere()
{
}

//------------------------------------------
// 初期化
//------------------------------------------
void CatSphere::Intialize()
{
	m_isHit = false;
	// 重力加速度を計算
	m_gravityAcceleration = static_cast<float>(m_parameters["GRAVITY"]) * static_cast<float>(m_parameters["MASS"]);

	// 位置データ
	DirectX::SimpleMath::Vector3 initPos;
	for (const auto& pos : m_parameters["START_POSITION"])
	{
		initPos = { pos[0], pos[1], pos[2] };
	}
	// 元のスケールデータ
	for (const auto& scale : m_parameters["DEFAULT_SCALE"])
	{
		m_defaultScale = { scale[0], scale[1], scale[2] };
	}
	m_scale = m_defaultScale;

	// 動きの速さ
	m_moveSpeed = static_cast<float>(m_parameters["MOVE_SPEED"]);
	// ジャンプ力
	m_jumpForce = static_cast<float>(m_parameters["JUMP_FORCE"]);
	// 摩擦
	m_fricition = static_cast<float>(m_parameters["FRICTION"]);

	// スタート位置の設定
	SetPosition(initPos);

	// ボールの移動方向の初期化
	m_forward = SimpleMath::Vector3(0.0f, 0.0f, 1.0f);

	// 移動速度初期化
	m_velocity = {};
	//回転初期化
	m_rotation = SimpleMath::Quaternion::CreateFromRotationMatrix(
		SimpleMath::Matrix::CreateLookAt(GetPosition(), GetPosition() + m_forward, SimpleMath::Vector3::Up));
	m_totalForce = {};

	// 腕初期化
	m_arm->Initialize(GetPosition(), m_rotation);
}

//------------------------------------------
// 更新
//------------------------------------------
void CatSphere::Update(const DX::StepTimer& timer)
{
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());

	// キー操作
	KeyInput(elapsedTime);

	// 現在の状態を更新
	m_currentState->Update(elapsedTime);

	// 動き
	Movement(elapsedTime);

	// 腕の更新
	m_arm->Update(GetPosition(), m_rotation);


	// 当たり判定オブジェクトの位置を設定
	m_collider.Center = GetPosition();
	m_collider.Center.y += m_radius;

	// エフェクト更新
	m_landParticle->Update(timer);
	m_jumpParticle->Update(timer);

	m_runParticle->Update(timer);

}

//------------------------------------------
// 描画
//------------------------------------------
void CatSphere::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection,
	std::function<void __cdecl()> function
)
{
	//// 当たり判定を描画
	//m_displayCollision->AddBoundingVolume(m_boundingSphere);
	//m_displayCollision->DrawCollision(context, states, view, projection);

	// 拡縮行列作成
	SimpleMath::Matrix scaleMatrix = DirectX::SimpleMath::Matrix::CreateScale(m_scale);

	// 半径分ずらす移動行列
	SimpleMath::Matrix radiusMatrix = SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.0f, -GetRadius(), 0.0f));

	// 回転行列を計算
	SimpleMath::Matrix rotMatrix = SimpleMath::Matrix::CreateFromQuaternion(m_rotation);

	// 移動行列を計算
	SimpleMath::Matrix transMatrix = SimpleMath::Matrix::CreateTranslation(
		GetPosition() + SimpleMath::Vector3(0.0f, GetRadius(), 0.0f));

	// ワールド行列を計算
	SimpleMath::Matrix world;

	world *= scaleMatrix * radiusMatrix * rotMatrix * transMatrix;

	// ボディモデル描画
	m_model->Draw(context, *states, world, view, projection, false, function);

	// 腕描画
	m_arm->Render(context, states, view, projection, function, m_scale);


}

//------------------------------------------
// エフェクト描画
//------------------------------------------
void CatSphere::RenderEffect()
{
	if (m_resultState != ResultState::IDLE) return;

	// エフェクト描画
	m_runParticle->CreateBillboard(
		DirectX::SimpleMath::Vector3::Up);
	m_runParticle->Render();

	m_landParticle->CreateBillboard(
		DirectX::SimpleMath::Vector3::Up);
	m_landParticle->Render();

	m_jumpParticle->CreateBillboard(
		DirectX::SimpleMath::Vector3::Up);
	m_jumpParticle->Render();

}

//------------------------------------------
// 終了処理
//------------------------------------------
void CatSphere::Finalize()
{
	
}


//------------------------------------------
// 移動
//------------------------------------------
void CatSphere::Movement(float elapsedTime)
{
	// 摩擦処理
	m_velocity *= m_fricition;
	// 速度制限
	m_velocity.Clamp(-SimpleMath::Vector3::One / 8, SimpleMath::Vector3::One / 8);

	// 速度を加算
	m_totalForce += m_velocity;

	// 摩擦処理
	m_totalForce.x *= m_fricition - 0.01f;
	m_totalForce.z *= m_fricition - 0.01f;

	// 重力とジャンプを加算
	m_totalForce.y -= m_gravityAcceleration;

	// ボール移動
	SetPosition(GetPosition() + m_totalForce);

	// 衝突判定チェック
	if (ObjectCollision::GetInstance()->CatToModel() && m_currentState == GetRunning())
	{
		// yの力を0
		m_totalForce.y = 0.0f;
		// 重力加速度初期化
		m_gravityAcceleration = {};
		// 当たっている
		m_isHit = true;
	}
	else
	{
		// スケールを徐々に元の大きさに
		m_scale = SimpleMath::Vector3::Lerp(m_scale, m_defaultScale, 0.2f);
		// 重力加速度を計算
		m_gravityAcceleration = static_cast<float>(m_parameters["GRAVITY"]) * static_cast<float>(m_parameters["MASS"]);

		// 当たっていない
		m_isHit = false;
		// 着地のアニメーションをしていない
		m_animated = false;
		// 着地のパーティクルを出していない
		m_landParticlePlayed = false;
	}

	if (m_isHit)
	{
		if (!m_landParticlePlayed)
		{
			m_landParticle->CreateLandParticle(GetPosition());
			m_landParticle->SetState(LandParticle::State::PLAY);
			m_landParticlePlayed = true;
		}

		// 着地アニメーション再生
		if (!m_animated)
		{
			m_timer += elapsedTime;
			LandingAnimation();
		}

		if (m_totalForce.Length() > 0.05f)
		{
			m_runParticle->CreateRunParticle(elapsedTime, GetPosition());
			m_runParticle->SetState(RunParticle::State::PLAY);
		}
	}


	// ボール回転
	m_rotation = SimpleMath::Quaternion::CreateFromRotationMatrix(
		SimpleMath::Matrix::CreateLookAt(GetPosition(), GetPosition() + m_forward, SimpleMath::Vector3::Up));

	if (GetPosition().y < DEADLINE)
	{
		m_isDead = true;
	}
}

//------------------------------------------
// キー入力
//------------------------------------------
void CatSphere::KeyInput(float elapsedTime)
{
	auto keyState = Graphics::GetInstance()->GetKeyboardStateTracker();
	auto key = keyState->GetLastState();

	// ジャンプ中かどうか
	if (m_currentState != GetJumping() && m_isHit)
	{
		// ジャンプ
		if (keyState->pressed.Space)
		{
			m_currentState = GetJumping();
			// パーティクル作成
			m_jumpParticle->CreateJumpParticle(GetPosition());
			m_jumpParticle->SetState(JumpParticle::State::PLAY);
			// アニメーションタイマーを初期化
			m_timer = 0.0f;

		}
	}
}

//------------------------------------------
// 力を更新
//------------------------------------------
void CatSphere::UpdateForce(DirectX::SimpleMath::Vector3 force)
{
	m_totalForce = force;
}

//------------------------------------------
// リザルト時の処理
//------------------------------------------
void CatSphere::ResultUpdate()
{
	switch (m_resultState)
	{
	case CatSphere::ResultState::LOOKUP:
	{
		// 上を見上げる回転
		SimpleMath::Quaternion lookUpQuaternion =
			SimpleMath::Quaternion::CreateFromRotationMatrix(
				SimpleMath::Matrix::CreateRotationX(XMConvertToRadians(45.0f)) *
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180.0f))
				);

		// 徐々に上を見上げる
		m_rotation = SimpleMath::Quaternion::Lerp(
			m_rotation,
			lookUpQuaternion,
			0.1f
		);

		// 見上げる
		m_arm->SetArmState(Arm::ArmState::LOOKUP);

		break;
	}
	case CatSphere::ResultState::CLAP:
	{
		// 正面を見る
		SimpleMath::Quaternion resultQuaternion =
			SimpleMath::Quaternion::CreateFromRotationMatrix(
				SimpleMath::Matrix::CreateRotationX(XMConvertToRadians(20.0f)) *
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180.0f))
			);

		//回転初期化
		m_rotation = SimpleMath::Quaternion::Lerp(
			m_rotation,
			resultQuaternion,
			0.1f
		);

		m_arm->SetArmState(Arm::ArmState::CLAP);
		break;
	}
	case CatSphere::ResultState::RESULT:
	{
		// 正面を見る
		SimpleMath::Quaternion resultQuaternion =
			SimpleMath::Quaternion::CreateFromRotationMatrix(
				SimpleMath::Matrix::CreateRotationX(XMConvertToRadians(20.0f)) *
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180.0f))
			);

		//回転初期化
		m_rotation = SimpleMath::Quaternion::Lerp(
			m_rotation,
			resultQuaternion,
			0.1f
		);

		// 正面を見る
		m_arm->SetArmState(Arm::ArmState::LOOKCAMERA);

		break;
	}
	default:
		break;
	}
	
	// 腕更新
	m_arm->ResultUpdate();
}

//------------------------------------------
// 着地アニメーション
//------------------------------------------
void CatSphere::LandingAnimation()
{
	float scale = 0.0f;

	if (m_timer > 0.8f)
	{
		m_timer = 0.0f;
		m_scale = m_defaultScale;
		m_animated = true;
	}
	else if (m_timer > 0.2f)
	{
		scale = 0.05f;
		m_scale = SimpleMath::Vector3::Lerp(m_scale, m_defaultScale, 0.2f);
	}
	else if (m_timer > 0.0f)
	{
		scale = 0.07f;
		m_scale += DirectX::SimpleMath::Vector3(scale, -scale, scale);
	}

	// 大きさを制限する
	m_scale.Clamp({ m_defaultScale.x ,0.2f, m_defaultScale.z }, {1.2f, m_defaultScale.y, 1.2f});
}