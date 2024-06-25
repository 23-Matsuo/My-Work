//--------------------------------------------------------------------------------------
// File: Arm.h
//
// �{�[���̘r�N���X
//
//
// Last Update: 2023.12.08
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef ARM_DEFINED
#define ARM_DEFINED


class CatSphere;

class Arm
{
public:
	static const DirectX::SimpleMath::Vector3 SCALE;

	enum class ArmState
	{
		IDLE,
		LOOKUP,
		CLAP,
		LOOKCAMERA
	};
public:

	// �R���X�g���N�^
	Arm(CatSphere* parent);

	// �f�X�g���N�^
	~Arm();

	// ������
	void Initialize(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& rotation);

	// �X�V
	void Update(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& rotation);

	// �`��
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection,
		std::function<void __cdecl()> function,
		const DirectX::SimpleMath::Vector3& scale
	);

	// �㏈��
	void Finalize();

	// ���U���g���̍X�V
	void ResultUpdate();

public:

	DirectX::SimpleMath::Vector3 GetPosition() { return m_armLPosition; }

	const float& GetArmLAngle() { return m_armLAngle; }
	const float& GetArmRAngle() { return m_armRAngle; }
	// �r�̉�]��ݒ�
	void SetArmLAngle(const float& angle) { m_armLAngle = angle; }
	void SetArmRAngle(const float& angle) { m_armRAngle = angle; }

	// �r�̏�Ԃ�ݒ�
	void SetArmState(ArmState state)	  { m_state = state; }
	
private:

	void Clap();

private:

	// �r�̏��
	ArmState m_state;

	// ���f��
	DirectX::Model* m_armModel;

	// �e
	CatSphere* m_parent;

	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;

	// �ʒu
	DirectX::SimpleMath::Vector3 m_armLPosition;
	DirectX::SimpleMath::Vector3 m_armRPosition;

	// ��]�p
	float m_armLAngle;
	float m_armRAngle;
	// ���肷�鎞�̃A���O��
	float m_armLClapAngle;
	float m_armRClapAngle;
	// ����̌���
	char m_clapLDirection;
	char m_clapRDirection;

};
#endif		// ARM_DEFINED