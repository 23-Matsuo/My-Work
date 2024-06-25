//--------------------------------------------------------------------------------------
// File: Enemy.cpp
//
// �v���C���[�N���X
//
// Last Update: 2023.10.31
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Enemy.h"
#include "MyLib/Resources.h"
#include "MyLib/Graphics.h"
#include "MyLib/ObjectCollision.h"
#include "MyLib/SoundUtility.h"

using namespace DirectX;


//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Enemy::Enemy()
	:
	m_collider(SimpleMath::Vector3::Zero, 0.0f)
	, m_isHit(false)
	, m_isJumped(false)
	, m_isDead(false)
	, m_landParticlePlayed(false)
	, m_jumpForce(0.0f)
	, m_gravityAcceleration{}
	, m_model(nullptr)
	, m_scale(DEFAULT_SCALE)
	, m_animated(false)
	, m_timer(0.0f)
	, m_resultState(ResultState::IDLE)
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	// �����蔻��\���p
	m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);

	// ���f���ݒ�
	m_model = Resources::GetInstance()->GetModel(L"Enemy");

	//// �Փ˔���Ƀ|�C���^��n��
	//ObjectCollision::GetInstance()->SetCatBody(this);

	// �p�[�e�B�N���쐬
	m_landParticle = std::make_unique<LandParticle>();
	m_landParticle->Create(Graphics::GetInstance()->GetDeviceResources());
	m_jumpParticle = std::make_unique<JumpParticle>();
	m_jumpParticle->Create(Graphics::GetInstance()->GetDeviceResources());
	m_runParticle = std::make_unique<RunParticle>();
	m_runParticle->Create(Graphics::GetInstance()->GetDeviceResources());

	// �v���C���[�̃p�����[�^��ǂݍ���
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/PlayerParameter");

	m_radius = static_cast<float>(m_parameters["RADIUS"]);
	m_collider.Radius = m_radius;

	// �r���쐬����
	m_arm = std::make_unique<Arm>(this);

	// �W�����v��
	m_jumpSound = Resources::GetInstance()->GetSound(L"Jump");
}


//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
Enemy::~Enemy()
{
}

//------------------------------------------
// ������
//------------------------------------------
void Enemy::Intialize()
{
	m_isHit = false;
	m_isJumped = false;
	//m_isDead = false;
	// �d�͉����x���v�Z
	m_gravityAcceleration = static_cast<float>(m_parameters["GRAVITY"]) * static_cast<float>(m_parameters["MASS"]);

	// �ʒu�f�[�^
	DirectX::SimpleMath::Vector3 initPos;
	for (const auto& point : m_parameters["START_POSITION"])
	{
		initPos = { point[0], point[1], point[2] };
	}
	// �X�^�[�g�ʒu�̐ݒ�
	SetPosition(initPos);

	// �{�[���̈ړ������̏�����
	m_forward = SimpleMath::Vector3(0.0f, 0.0f, 1.0f);

	m_shadowHitPosition = {};
	// �ړ����x������
	m_velocity = {};
	//��]������
	m_rotation = SimpleMath::Quaternion::CreateFromRotationMatrix(
		SimpleMath::Matrix::CreateLookAt(GetPosition(), GetPosition() + m_forward, SimpleMath::Vector3::Up));
	m_totalForce = {};

	// �r������
	m_arm->Initialize(GetPosition(), m_rotation);

	m_scale = DEFAULT_SCALE;
}


//------------------------------------------
// �X�V
//------------------------------------------
void Enemy::Update(const DX::StepTimer& timer)
{
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());


	// �L�[����
	KeyInput(elapsedTime);
	// ����
	Movement(elapsedTime);

	// �r�̍X�V
	m_arm->Update(GetPosition(), m_rotation);


	// �����蔻��I�u�W�F�N�g�̈ʒu��ݒ�
	m_collider.Center = GetPosition();
	m_collider.Center.y += m_radius;

	// �G�t�F�N�g�X�V
	m_landParticle->Update(timer);
	m_jumpParticle->Update(timer);

	m_runParticle->Update(timer);

}

//------------------------------------------
// �`��
//------------------------------------------
void Enemy::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection,
	std::function<void __cdecl()> function
)
{
	//// �����蔻���`��
	//m_displayCollision->AddBoundingVolume(m_boundingEnemy);
	//m_displayCollision->DrawCollision(context, states, view, projection);

	// �g�k�s��쐬
	SimpleMath::Matrix scaleMatrix = DirectX::SimpleMath::Matrix::CreateScale(m_scale);

	// ���a�����炷�ړ��s��
	SimpleMath::Matrix radiusMatrix = SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.0f, -GetRadius(), 0.0f));

	// ��]�s����v�Z
	SimpleMath::Matrix rotMatrix = SimpleMath::Matrix::CreateFromQuaternion(m_rotation);

	// �ړ��s����v�Z
	SimpleMath::Matrix transMatrix = SimpleMath::Matrix::CreateTranslation(
		GetPosition() + SimpleMath::Vector3(0.0f, GetRadius(), 0.0f));

	// ���[���h�s����v�Z
	SimpleMath::Matrix world;

	world *= scaleMatrix * radiusMatrix * rotMatrix * transMatrix;

	// �{�f�B���f���`��
	m_model->Draw(context, *states, world, view, projection, false, function);

	// �r�`��
	m_arm->Render(context, states, view, projection, function, m_scale);


}

//------------------------------------------
// �G�t�F�N�g�`��
//------------------------------------------
void Enemy::RenderEffect()
{
	if (m_resultState != ResultState::IDLE) return;

	// �G�t�F�N�g�`��
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
// �I������
//------------------------------------------
void Enemy::Finalize()
{
	
}


//------------------------------------------
// �ړ�
//------------------------------------------
void Enemy::Movement(float elapsedTime)
{

	// ���C����
	m_velocity *= static_cast<float>(m_parameters["FRICTION"]) ;
	// ���x����
	m_velocity.Clamp(-SimpleMath::Vector3::One / 8, SimpleMath::Vector3::One / 8);
	// ���x�x�N�g�����v�Z
	SimpleMath::Vector3 vec = m_velocity;
	// ���x�����Z
	m_totalForce += vec;

	// ���C����
	m_totalForce.x *= static_cast<float>(m_parameters["FRICTION"]);
	m_totalForce.z *= static_cast<float>(m_parameters["FRICTION"]);

	// �d�͂ƃW�����v�����Z
	m_totalForce.y += m_jumpForce - m_gravityAcceleration;

	// �{�[���ړ�
	SetPosition(GetPosition() + m_totalForce);

	// �Փ˔�����`�F�b�N����
	if (ObjectCollision::GetInstance()->CatToModel())
	{
		// y�̗͂�0��
		m_totalForce.y = 0.0f;
		m_gravityAcceleration = {};
		// �W�����v���ł͂Ȃ�
		m_isJumped = false;
		// �������Ă���
		m_isHit = true;

		if (!m_landParticlePlayed)
		{
			m_landParticle->CreateLandParticle(GetPosition());
			m_landParticle->SetState(LandParticle::State::PLAY);
			m_landParticlePlayed = true;
			PlaySound(m_jumpSound, false);
		}
		
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
	else
	{
		// �X�P�[�������X�Ɍ��̑傫����
		m_scale = SimpleMath::Vector3::Lerp(m_scale, DEFAULT_SCALE, 0.2f);
		// �d�͉����x���v�Z
		m_gravityAcceleration = static_cast<float>(m_parameters["GRAVITY"]) * static_cast<float>(m_parameters["MASS"]);

		// �������Ă��Ȃ�
		m_isHit = false;
		// ���n�̃A�j���[�V���������Ă��Ȃ�
		m_animated = false;
		// ���n�̃p�[�e�B�N�����o���Ă��Ȃ�
		m_landParticlePlayed = false;
	}

	// �{�[����]
	m_rotation = SimpleMath::Quaternion::CreateFromRotationMatrix(
		SimpleMath::Matrix::CreateLookAt(GetPosition(), GetPosition() + m_forward, SimpleMath::Vector3::Up));

	if (GetPosition().y < DEADLINE)
	{
		m_isDead = true;
	}
}

//------------------------------------------
// �L�[����
//------------------------------------------
void Enemy::KeyInput(float elapsedTime)
{
	auto keyState = Graphics::GetInstance()->GetKeyboardStateTracker();
	auto key = keyState->GetLastState();
	// �W�����v��
	m_jumpForce = 0.0f;

	// �ړ����x
	float moveSpeed = static_cast<float>(m_parameters["MOVE_SPEED"]) * elapsedTime;
	// ��]�Ɋ|���鎞��
	float time = 0.05f;

	// ���̓L�[�ɍ��킹�ăX�s�[�h�Ɛi�s�������v�Z
	if (key.W || key.Up)
	{
		m_velocity.z -= moveSpeed;
		m_forward = SimpleMath::Vector3::Lerp(m_forward, SimpleMath::Vector3(0.0f, 0.0f, -1.0f), time);
	}
	if (key.S || key.Down)
	{
		m_velocity.z += moveSpeed;
		m_forward = SimpleMath::Vector3::Lerp(m_forward, SimpleMath::Vector3(0.0f, 0.0f, 1.0f), time);
	}
	if (key.A || key.Left)
	{
		m_velocity.x -= moveSpeed;
		m_forward = SimpleMath::Vector3::Lerp(m_forward, SimpleMath::Vector3(1.0f, 0.0f, 0.0f), time);
	}
	if (key.D || key.Right)
	{
		m_velocity.x += moveSpeed;
		m_forward = SimpleMath::Vector3::Lerp(m_forward, SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), time);
	}

	// �W�����v�͂��ǂ���
	if (!m_isJumped && m_isHit)
	{
		// �W�����v���ł͂Ȃ��Ȃ�
		// �X�y�[�X�ŃW�����v
		if (keyState->pressed.Space)
		{
			// �W�����v���ɃZ�b�g
			m_isJumped = true;
			// �W�����v�͂��Z�b�g
 			m_jumpForce = static_cast<float>(m_parameters["JUMP_FORCE"]);

			// �p�[�e�B�N���Đ�
			m_jumpParticle->CreateJumpParticle(GetPosition());
			m_jumpParticle->SetState(JumpParticle::State::PLAY);

			m_timer = 0.0f;
			//PlaySound(m_jumpSound, false);
		}
	}
}

//------------------------------------------
// ����
//------------------------------------------
void Enemy::Reflect(DirectX::SimpleMath::Quaternion pointRotation)
{
	// �͂𔽓]
	m_totalForce.y *= -1;
	// �����������_�̊p�x���璵�˕Ԃ�p�x���v�Z
	m_totalForce = SimpleMath::Vector3::Transform(m_totalForce, pointRotation);

	m_velocity *= -1;
}

//------------------------------------------
// �͂��X�V
//------------------------------------------
void Enemy::UpdateForce(DirectX::SimpleMath::Vector3 force)
{
	m_totalForce = force;
}

//------------------------------------------
// ���U���g���̏���
//------------------------------------------
void Enemy::ResultUpdate()
{
	switch (m_resultState)
	{
	case Enemy::ResultState::LOOKUP:
	{
		// ������グ���]
		SimpleMath::Quaternion lookUpQuaternion =
			SimpleMath::Quaternion::CreateFromRotationMatrix(
				SimpleMath::Matrix::CreateRotationX(XMConvertToRadians(45.0f)) *
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180.0f))
				);

		// ���X�ɏ�����グ��
		m_rotation = SimpleMath::Quaternion::Lerp(
			m_rotation,
			lookUpQuaternion,
			0.1f
		);

		// ���グ��
		m_arm->SetArmState(Arm::ArmState::LOOKUP);

		break;
	}
	case Enemy::ResultState::CLAP:
	{
		// ���ʂ�����
		SimpleMath::Quaternion resultQuaternion =
			SimpleMath::Quaternion::CreateFromRotationMatrix(
				SimpleMath::Matrix::CreateRotationX(XMConvertToRadians(20.0f)) *
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180.0f))
			);

		//��]������
		m_rotation = SimpleMath::Quaternion::Lerp(
			m_rotation,
			resultQuaternion,
			0.1f
		);

		m_arm->SetArmState(Arm::ArmState::CLAP);
		break;
	}
	case Enemy::ResultState::RESULT:
	{
		// ���ʂ�����
		SimpleMath::Quaternion resultQuaternion =
			SimpleMath::Quaternion::CreateFromRotationMatrix(
				SimpleMath::Matrix::CreateRotationX(XMConvertToRadians(20.0f)) *
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180.0f))
			);

		//��]������
		m_rotation = SimpleMath::Quaternion::Lerp(
			m_rotation,
			resultQuaternion,
			0.1f
		);

		// ���ʂ�����
		m_arm->SetArmState(Arm::ArmState::LOOKCAMERA);

		break;
	}
	default:
		break;
	}
	
	// �r�X�V
	m_arm->ResultUpdate();
}

//------------------------------------------
// ���n�A�j���[�V����
//------------------------------------------
void Enemy::LandingAnimation()
{
	float scale = 0.0f;

	if (m_timer > 0.8f)
	{
		m_timer = 0.0f;
		m_scale = DEFAULT_SCALE;
		m_animated = true;
	}
	else if (m_timer > 0.2f)
	{
		scale = 0.05f;
		m_scale = SimpleMath::Vector3::Lerp(m_scale, DEFAULT_SCALE, 0.2f);
	}
	else if (m_timer > 0.0f)
	{
		scale = 0.07f;
		m_scale += DirectX::SimpleMath::Vector3(scale, -scale, scale);
	}

	// �傫���𐧌�����
	m_scale.Clamp({ DEFAULT_SCALE.x ,0.2f, DEFAULT_SCALE.z }, {1.2f, DEFAULT_SCALE.y, 1.2f});
}