//--------------------------------------------------------------------------------------
// File: Arm.h
//
// �{�[���̘r�N���X
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
// �R���X�g���N�^
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
// �f�X�g���N�^
//------------------------------------------
Arm::~Arm()
{
}

//------------------------------------------
// ������
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
// �X�V
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
// �`��
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

	// �X�P�[���s��
	DirectX::SimpleMath::Matrix scaleMat
		= DirectX::SimpleMath::Matrix::CreateScale(SCALE);
	// ���r�̈ړ��s��
	DirectX::SimpleMath::Matrix armLTranslationMat
		= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-1.3f, m_parent->GetRadius() / 1.5f, 0.0f));
	DirectX::SimpleMath::Matrix parentPosLMatrix
		= DirectX::SimpleMath::Matrix::CreateTranslation(m_armLPosition);
	// �E�r�̈ړ��s��
	DirectX::SimpleMath::Matrix armRTranslationMat 
		= DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(1.3f, -m_parent->GetRadius() / 1.5f, 0.0f));
	DirectX::SimpleMath::Matrix parentPosRMatrix
		= DirectX::SimpleMath::Matrix::CreateTranslation(m_armRPosition);

	// �e�̉�]�s��
	DirectX::SimpleMath::Matrix parentRotation
		= DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation);


	// �ړ����ɘr����]������ׂ̍s��v�Z

	DirectX::SimpleMath::Vector3 forward = m_parent->GetForward();
	DirectX::SimpleMath::Vector3 horizontal = { forward.z, 0.0f, forward.x };

	DirectX::SimpleMath::Matrix rotateL;
	DirectX::SimpleMath::Matrix rotateR;
	// ���̂̌������牡�����S�̉�]�s��
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
// �I������
//------------------------------------------
void Arm::Finalize()
{
}

//------------------------------------------
// ���U���g���̍X�V
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

	// ���r�̔���̌������v�Z
	if (m_armLClapAngle <= DirectX::XMConvertToRadians(-70.0f))
	{
		m_clapLDirection *= -1;
	}
	else if (m_armLClapAngle >= DirectX::XMConvertToRadians(0.0f))
	{
		m_clapLDirection *= -1;
	}
	// �p�x���v�Z
	m_armLClapAngle += clapSpeed * m_clapLDirection;

	// �E�r�̔���̌������v�Z
	if (m_armRClapAngle >= DirectX::XMConvertToRadians(90.0f))
	{
		m_clapRDirection *= -1;
	}
	else if (m_armRClapAngle <= DirectX::XMConvertToRadians(20.0f))
	{
		m_clapRDirection *= -1;
	}
	// �p�x���v�Z
	m_armRClapAngle += clapSpeed * m_clapRDirection;

}
