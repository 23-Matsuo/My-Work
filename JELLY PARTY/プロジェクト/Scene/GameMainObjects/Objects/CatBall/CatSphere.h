//--------------------------------------------------------------------------------------
// File: Sphere.h
//
// �v���C���[�N���X
//
//
// Last Update: 2023.10.31
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef SPHERE_DEFINED
#define SPHERE_DEFINED

#include "MyLib/PhysicalObject.h"
#include "Arm.h"

#include "ImaseLib/ObjCollision.h"
#include "Scene/Effects/LandParticle.h"
#include "Scene/Effects/JumpParticle.h"
#include "Scene/Effects/RunParticle.h"

#include "MyLib/IPlayerState.h"

class PhysicalObject;

class CatSphere : public PhysicalObject
{
public:

	class Running;
	class Jumping;

	Running* GetRunning() { return &m_running; }
	Jumping* GetJumping() { return &m_jumping; }

public:

	// �����j���O�X�e�[�g�N���X
	class Running : public IPlayerState
	{
	public:
		// �R���X�g���N�^
		Running(CatSphere* pSphere);

		// �f�X�g���N�^
		~Running();

		// ����������
		inline void Initialize() override;

		// �`��
		inline void Render() override;

		// �L�[����
		inline void Update(const float& elapsedTime) override;

	private:

		CatSphere* m_pSphere;
	};

public:

	// �W�����v�X�e�[�g�N���X
	class Jumping : public IPlayerState
	{
	public:
		// �R���X�g���N�^
		Jumping(CatSphere* pSphere);

		// �f�X�g���N�^
		~Jumping();

		// ����������
		inline void Initialize() override;

		// �`��
		inline void Render() override;

		// �L�[����
		inline void Update(const float& elapsedTime) override;

	private:

		CatSphere* m_pSphere;

		// �W�����v��
		DirectX::SoundEffectInstance* m_jumpSound;
	};

private:
	
	// �����j���O���
	Running m_running;
	// �W�����v���
	Jumping m_jumping;

public:

	enum class ResultState
	{
		IDLE,
		LOOKUP,
		CLAP,
		RESULT,
	};

public:
	// �R���X�g���N�^
	CatSphere();

	// �f�X�g���N�^
	~CatSphere();

	// ������
	void Intialize();

	// �X�V
	void Update(const DX::StepTimer& timer) override;

	// �`��
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection,
		std::function<void __cdecl()> function
	);

	void RenderEffect();

	// �I������
	void Finalize();


public:

	// ����
	void UpdateForce(DirectX::SimpleMath::Vector3 force);

	// ���U���g���̍X�V
	void ResultUpdate();


private:

	// �ړ�
	void Movement(float elapsedTime);

	// �L�[����
	void KeyInput(float elapsedTime);

	// ���n�A�j���[�V����
	void LandingAnimation();

public:	// �Z�b�^�[
	void SetResultState(const ResultState& state)								{ m_resultState = state; }
	void SetHitFlag(const bool& flag)		 									{ m_isHit = flag; }	
	void SetGravityAc(const float& gravityAc)									{ m_gravityAcceleration = gravityAc; }
	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity)				{ m_velocity = velocity; }
	void SetTotalForce(const DirectX::SimpleMath::Vector3& force)				{ m_totalForce = force; }
	void SetForward(const DirectX::SimpleMath::Vector3& forward)				{ m_forward = forward; }
	void SetCurrentState(IPlayerState* currentState)							{ m_currentState = currentState; }

public: // �Q�b�^�[
	const DirectX::SimpleMath::Vector3& GetForward() 			{ return m_forward; }
	const float& GetGravityAc()									{ return m_gravityAcceleration; }
	const float& GetMoveSpeed()									{ return m_moveSpeed; }
	const DirectX::SimpleMath::Vector3& GetVelocity()			{ return m_velocity; }
	const DirectX::SimpleMath::Vector3& GetTotalForce()			{ return m_totalForce; }
	const DirectX::SimpleMath::Quaternion& GetRotation() 		{ return m_rotation; }
	const float& GetJumpForce()									{ return m_jumpForce; }
	const bool& GetIsDead()										{ return m_isDead; }
	const DirectX::BoundingSphere& GetCollider() 				{ return m_collider; }
	const float& GetRadius()									{ return m_radius; }
	DirectX::Model* GetModel()									{ return m_model; }
	const IPlayerState* GetCurrentState()						{ return m_currentState; }

private:

	// ���S����ʒu
	const float DEADLINE = -20.0f;

	// �����蔻��
	DirectX::BoundingSphere m_collider;

private:

	// ������ꂽ�͂̍��v
	DirectX::SimpleMath::Vector3 m_totalForce;
	// �d�͉����x
	float m_gravityAcceleration;
	// �{�[���̉�]
	DirectX::SimpleMath::Quaternion m_rotation;
	// �X�P�[��
	DirectX::SimpleMath::Vector3 m_scale;
	// �����X�P�[��
	DirectX::SimpleMath::Vector3 m_defaultScale;

	// �{�[���̈ړ�����
	DirectX::SimpleMath::Vector3 m_forward;
	// �{�[���̑���
	DirectX::SimpleMath::Vector3 m_velocity;

	// ��������
	float m_moveSpeed;
	// �W�����v��
	float m_jumpForce;
	// ���C
	float m_fricition;

private:

	// ���݂̏��
	IPlayerState* m_currentState;

	// ���f���ւ̃|�C���^
	DirectX::Model* m_model;

	// �A�[���ւ̃|�C���^
	std::unique_ptr<Arm> m_arm;

	// �Փ˔���̕\���I�u�W�F�N�g�ւ̃|�C���^
	std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// �p�[�e�B�N���ւ̃|�C���^
	std::unique_ptr<LandParticle> m_landParticle;
	std::unique_ptr<JumpParticle> m_jumpParticle;
	std::unique_ptr<RunParticle> m_runParticle;

private:

	// ���a
	float m_radius;

	// �Փ˃t���O
	bool m_isHit;
	// ���S�t���O
	bool m_isDead;
	// ���n�G�t�F�N�g�o������
	bool m_landParticlePlayed;
	// �A�j���[�V����������
	bool m_animated;

	// �A�j���[�V�����̎���
	float m_timer;

	// �p�����[�^
	nlohmann::json m_parameters;

	// ���U���g���̏��
	ResultState m_resultState;
};

#endif // SPHERE_DEFINED