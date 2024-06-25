//--------------------------------------------------------------------------------------
// File: PhysicalPoint.h
//
// ���_�ɗ͂�������N���X
// 
// Date: 2023.5.10
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef PHYSICAL_POINT_DEFINED
#define PHYSICAL_POINT_DEFINED

#include "MyLib/PhysicalObject.h"

#include "ImaseLib/ObjCollision.h"
class PhysicalPoint : public PhysicalObject
{
public:
	enum class STATE
	{
		FIXED,		// �Œ�
		UNFIXED,	// �Œ肵�Ȃ�
		WALL,		// ��
		BAR,		// �o�[
	};
public:
	// �R���X�g���N�^
	PhysicalPoint(DirectX::SimpleMath::Vector3 pos, const int& ID);

	// �f�X�g���N�^
	~PhysicalPoint() {};

	// �X�V
	void Update(const DX::StepTimer& timer) override;

	// �͂����Z����֐�
	void AddForce(DirectX::SimpleMath::Vector3 force);

	// ����
	void UpdateVector(DirectX::SimpleMath::Vector3 force);


	// �p�����[�^������
	void ParameterReset(const DirectX::SimpleMath::Vector3& position);

	// ���Z�b�g��ݒ�
	void ResultReset(const DirectX::SimpleMath::Vector3& position);

public:
	// UV���W���擾
	const DirectX::SimpleMath::Vector2 GetUV() 				{ return m_uv; }
	// ��Ԃ��擾
	const STATE GetState() 									{ return m_state; }
	// �ړ��ʂ��擾
	const DirectX::SimpleMath::Vector3 GetVector() 			{ return m_vector; }
	// �����ʒu���擾
	const DirectX::SimpleMath::Vector3 GetInitPosition()	{ return m_initPosition; }
	// �o�E���f�B���O�X�t�B�A�擾
	const DirectX::BoundingSphere GetBoundingSphere()		{ return m_boundingSphere; }
	// ���a�擾
	const float GetRadius() 								{ return static_cast<float>(m_parameters["SoftBodyParameter"]["PointParameter"]["RADIUS"]); }
public:

	// ��Ԃ�ݒ�
	void SetState(const STATE& state) { m_state = state; }
	// UV���W��ݒ�
	void SetUV(const DirectX::SimpleMath::Vector2& uv) { m_uv = uv; }
	// ���Z�b�g�ʒu��ݒ�
	void SetResetPosition(const DirectX::SimpleMath::Vector3& position) { m_resetPosition = position; }
	// �Փ˔���I�u�W�F�N�g�̃T�C�Y��ݒ�
	void SetColliderSize(const float& radius) { m_boundingSphere.Radius = radius; }

private:

	// ���_�̏��
	STATE m_state;
	
	// �x�N�g��
	DirectX::SimpleMath::Vector3 m_vector;

	// �d�͉����x
	float m_gravityAcceleration;

	// ����
	float m_mass;
	// �d��
	float m_gravity;

	// ���ړI�ɉ�����ꂽ��
	DirectX::SimpleMath::Vector3 m_addForce;

	// ������ꂽ�͂̍��v
	DirectX::SimpleMath::Vector3 m_totalForce;

	// �����ʒu
	DirectX::SimpleMath::Vector3 m_initPosition;

	// �o�E���f�B���O�X�t�B�A�ւ̃|�C���^
	DirectX::BoundingSphere m_boundingSphere;

	// ID�ԍ�
	int m_id;

	// UV���W
	DirectX::SimpleMath::Vector2 m_uv;

	// �p�����[�^
	nlohmann::json m_parameters;

	DirectX::SimpleMath::Vector3 m_resetPosition;
};

#endif // PHYSICAL_POINT_DEFINED