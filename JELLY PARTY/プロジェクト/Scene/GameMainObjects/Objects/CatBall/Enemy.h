//--------------------------------------------------------------------------------------
// File: Enemy.h
//
// �v���C���[�N���X
//
//
// Last Update: 2023.10.31
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef ENEMY_DEFINED
#define ENEMY_DEFINED

#include "MyLib/PhysicalObject.h"
#include "Arm.h"

#include "ImaseLib/ObjCollision.h"
#include "Scene/Effects/LandParticle.h"
#include "Scene/Effects/JumpParticle.h"
#include "Scene/Effects/RunParticle.h"

class Enemy : public PhysicalObject
{
public:

	enum class ResultState
	{
		IDLE,
		LOOKUP,
		CLAP,
		RESULT,
	};

public:

	// �f�t�H���g�̑傫��
	const DirectX::SimpleMath::Vector3 DEFAULT_SCALE = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

public:
	// �R���X�g���N�^
	Enemy();

	// �f�X�g���N�^
	~Enemy();

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
	void Reflect(DirectX::SimpleMath::Quaternion pointRotation);

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
	void SetJumpFlag(const bool& flag)											{ m_isJumped = flag; }
	void SetGravityAc(const float& gravityAc)									{ m_gravityAcceleration = gravityAc; }
	void SetVelocity(const DirectX::SimpleMath::Vector3& velocity)				{ m_velocity = velocity; }

public: // �Q�b�^�[
	const DirectX::SimpleMath::Vector3& GetForward() 			{ return m_forward; }
	const float& GetGravityAc()									{ return m_gravityAcceleration; }
	const DirectX::SimpleMath::Vector3& GetVelocity()			{ return m_velocity; }
	const DirectX::SimpleMath::Vector3& GetTotalForce()			{ return m_totalForce; }
	const DirectX::SimpleMath::Quaternion& GetRotation() 		{ return m_rotation; }
	const float& GetJumpForce()									{ return m_jumpForce; }
	const bool& GetIsDead()										{ return m_isDead; }
	const DirectX::BoundingSphere& GetCollider() 				{ return m_collider; }
	const float& GetRadius()									{ return m_radius; }
	DirectX::Model* GetModel()									{ return m_model; }
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

	// �{�[���̈ړ�����
	DirectX::SimpleMath::Vector3 m_forward;
	// �{�[���̑���
	DirectX::SimpleMath::Vector3 m_velocity;

	// �e�̃q�b�g�ʒu
	DirectX::SimpleMath::Vector3 m_shadowHitPosition;

private:

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

	// �W�����v��
	DirectX::SoundEffectInstance* m_jumpSound;

private:
	// ���a
	float m_radius;

	// �W�����v��
	float m_jumpForce;
	// �W�����v�t���O
	bool m_isJumped;
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

#endif // ENEMY_DEFINED