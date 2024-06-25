//--------------------------------------------------------------------------------------
// File: CenterSoftBody.h
//
// ��]����o�[�̒��S
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef CENTER_SOFTBODY_DEFINED
#define CENTER_SOFTBODY_DEFINED

#include "MyLib/SoftBody/SoftBodyFactory.h"
#include "Scene/Effects/HitParticle.h"

class CenterSoftBody
{
public:

public:
	// �R���X�g���N�^
	CenterSoftBody();

	// �f�X�g���N�^
	~CenterSoftBody();

	// �X�V
	void Update(DirectX::SimpleMath::Matrix rotation);

	// �`��
	void Render();

	// �e�`��
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

public:

	SoftBodyCylinder::Points* GetPoints() { return m_cylinder->GetPoints(); }
	// ���_���q����
	std::vector<DirectX::SimpleMath::Vector3> GetJointPoints() const { return m_jointPoints; }

	DirectX::BoundingSphere GetCollision() const { return m_collision[0]; }

	SoftBodyCylinder* GetCylinder() { return m_cylinder.get(); }
private:

	// �W���C���g�쐬
	void CreateJoint();

private:
	
	// �W���C���g����ʒu
	std::vector<DirectX::SimpleMath::Vector3> m_jointPoints;

	// �V�����_�[�ւ̃|�C���^
	std::unique_ptr<SoftBodyCylinder> m_cylinder;

	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// �Փ˔���I�u�W�F�N�g
	DirectX::BoundingSphere m_collision[2];

	// �Փ˔���̕\���I�u�W�F�N�g�ւ̃|�C���^
	std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// �p�����[�^
	nlohmann::json m_parameters;

	// ���_�̐�
	int m_pointXNum;
	int m_pointYNum;

	// ���������G�t�F�N�g
	std::unique_ptr<HitParticle> m_hitParticle;
};

#endif // CENTER_SOFTBODY_DEFINED
