//--------------------------------------------------------------------------------------
// File: Arm.h
//
// ボールの腕クラス
//
//
// Last Update: 2023.12.08
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------


#include "pch.h"
#include "Arm.h"

#include "CatSphere.h"

const DirectX::SimpleMath::Vector3 Arm::SCALE = { 1.5f, 1.5f, 1.5f };

inline float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

//------------------------------------------
// コンストラクタ
//------------------------------------------
Arm::Arm(CatSphere* parent)
	:
	m_parent(parent)
	, m_state(ArmState::IDLE)
	, m_armLAngle(0.0f)
	, m_armRAngle(0.0f)
	, m_armLClapAngle(DirectX::XMConvertToRadians(0.0f))
	, m_armRClapAngle(DirectX::XMConvertToRadians(20.0f))
	, m_clapLDirection(1)
	, m_clapRDirection(-1)
{
	m_armModel = Resources::GetInstance()->GetModel(L"Arm");
}

//------------------------------------------
// デストラクタ
//------------------------------------------
Arm::~Arm()
{
}

//------------------------------------------
// 初期化
//------------------------------------------
void Arm::Initialize(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& rotation)
{
	m_armLPosition = position;
	m_armLPosition.y += m_parent->GetRadius();

	m_armRPosition = position;
	m_armRPosition.y += m_parent->GetRadius();
	m_rotation = rotation;

	m_armLAngle = 0.0f;
	m_armRAngle = 0.0f;
}

//------------------------------------------
// 更新
//------------------------------------------
void Arm::Update(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& rotation)
{
	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	m_armLPosition = position;
	m_armLPosition.y += m_parent->GetRadius();
	m_armRPosition = position;
	m_armRPosition.y += m_parent->GetRadius();
	m_rotation = rotation;

	float angle = m_parent->GetVelocity().Length() * 500000;
	
	m_armLAngle += DirectX::XMConvertToRadians(angle) * elapsedTime;

	m_armRAngle += DirectX::XMConvertToRadians(angle) * elapsedTime;
}

//------------------------------------------
// 描画
//------------------------------------------
void Arm::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection,
	std::function<void __cdecl()> function,
	const DirectX::SimpleMath::Vector3& scale
)
{
	UNREFERENCED_PARAMETER(scale);

	DirectX::SimpleMath::Matrix world;

	// スケール行列
	DirectX::SimpleMath::Matrix scaleMat
		= DirectX::SimpleMath::Matrix::CreateScale(SCALE);
	// 左腕の移動行列
	DirectX::SimpleMath::Matrix armLTranslationMat
		= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-1.3f, m_parent->GetRadius() / 1.5f, 0.0f));
	DirectX::SimpleMath::Matrix parentPosLMatrix
		= DirectX::SimpleMath::Matrix::CreateTranslation(m_armLPosition);
	// 右腕の移動行列
	DirectX::SimpleMath::Matrix armRTranslationMat 
		= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(1.3f, -m_parent->GetRadius() / 1.5f, 0.0f));
	DirectX::SimpleMath::Matrix parentPosRMatrix
		= DirectX::SimpleMath::Matrix::CreateTranslation(m_armRPosition);

	// 親の回転行列
	DirectX::SimpleMath::Matrix parentRotation
		= DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation);


	// 移動時に腕を回転させる為の行列計算

	DirectX::SimpleMath::Vector3 forward = m_parent->GetForward();
	DirectX::SimpleMath::Vector3 horizontal = { forward.z, 0.0f, forward.x };

	DirectX::SimpleMath::Matrix rotateL;
	DirectX::SimpleMath::Matrix rotateR;
	// 胴体の向きから横軸中心の回転行列
	rotateL	= DirectX::SimpleMath::Matrix::CreateFromAxisAngle(horizontal, m_armLAngle);
	rotateR = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(horizontal, m_armRAngle);

	if (m_state == ArmState::CLAP)
	{
		DirectX::SimpleMath::Vector3 vertical = { 0.0f, 1.0f, 0.0f };
		rotateL *= DirectX::SimpleMath::Matrix::CreateFromAxisAngle(vertical, m_armLClapAngle);
		rotateR *= DirectX::SimpleMath::Matrix::CreateFromAxisAngle(vertical, m_armRClapAngle);
	}

	world = scaleMat * armLTranslationMat * parentRotation * rotateL * parentPosLMatrix;
	m_armModel->Draw(context, *states, world, view, projection, false, function);

	world = scaleMat * armRTranslationMat * parentRotation * rotateR * parentPosRMatrix;
	m_armModel->Draw(context, *states, world, view, projection, false, function);
}

//------------------------------------------
// 終了処理
//------------------------------------------
void Arm::Finalize()
{
}

//------------------------------------------
// リザルト時の更新
//------------------------------------------
void Arm::ResultUpdate()
{
	switch (m_state)
	{
	case Arm::ArmState::IDLE:
		break;
	case Arm::ArmState::LOOKUP:
		m_armLAngle = lerp(m_armLAngle, DirectX::XMConvertToRadians(0.0f), 0.05f);
		m_armRAngle = lerp(m_armRAngle, DirectX::XMConvertToRadians(180.0f), 0.05f);
		break;
	case Arm::ArmState::CLAP:

		m_armLAngle = lerp(m_armLAngle, DirectX::XMConvertToRadians(20.0f), 0.05f);
		m_armRAngle = lerp(m_armRAngle, DirectX::XMConvertToRadians(160.0f), 0.05f);

		Clap();
		break;
	case Arm::ArmState::LOOKCAMERA:

		m_armLAngle = lerp(m_armLAngle, DirectX::XMConvertToRadians(180.0f), 0.05f);
		m_armRAngle = lerp(m_armRAngle, DirectX::XMConvertToRadians(0.0f), 0.05f);

		break;
	default:
		break;
	}
}

void Arm::Clap()
{
	float clapSpeed = DirectX::XMConvertToRadians(10.0f);

	// 左腕の拍手の向きを計算
	if (m_armLClapAngle <= DirectX::XMConvertToRadians(-70.0f))
	{
		m_clapLDirection *= -1;
	}
	else if (m_armLClapAngle >= DirectX::XMConvertToRadians(0.0f))
	{
		m_clapLDirection *= -1;
	}
	// 角度を計算
	m_armLClapAngle += clapSpeed * m_clapLDirection;

	// 右腕の拍手の向きを計算
	if (m_armRClapAngle >= DirectX::XMConvertToRadians(90.0f))
	{
		m_clapRDirection *= -1;
	}
	else if (m_armRClapAngle <= DirectX::XMConvertToRadians(20.0f))
	{
		m_clapRDirection *= -1;
	}
	// 角度を計算
	m_armRClapAngle += clapSpeed * m_clapRDirection;

}
