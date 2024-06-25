//--------------------------------------------------------------------------------------
// File: SoftBodyBar.h
//
// ��]����o�[
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef SOFTBODY_BAR_DEFINED
#define SOFTBODY_BAR_DEFINED

#include "CenterSoftBody.h"
#include "Scene/Effects/HitParticle.h"

class SoftBodyBar
{
public:
	enum class COLORS
	{
		MAGENTA,
		CYAN,
		YELLOW
	};

	// �o�[�̐�
	static const int BAR_NUM = 2;

public:

	// �R���X�g���N�^
	SoftBodyBar(const std::vector<DirectX::SimpleMath::Vector3>& jointPoints);

	// �f�X�g���N�^
	~SoftBodyBar();

	// �X�V
	void Update(
		DirectX::SimpleMath::Matrix rotation,
		SoftBodyCylinder* pCylinder,
		const float& anglarVelocity
	);

	// �`��
	void Render();

	// �e�`��
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

public:

	// �J���[���擾
	DirectX::SimpleMath::Color GetColor(COLORS color);

	// �\�t�g�{�f�B�擾
	SoftBodyBox* GetSoftBodies(const int& index) { return m_box[index].get(); }

private:

	// �o�[�̏�Ԃ�ݒ�
	void SetBarPointsState();

	// ���_���q����
	void JointPoints();

private:

	// �q���钸�_
	std::vector<DirectX::SimpleMath::Vector3> m_jointPoints;
	// �{�b�N�X�ւ̃|�C���^
	std::unique_ptr<SoftBodyBox> m_box[BAR_NUM];

	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// �Փ˔���̕\���I�u�W�F�N�g�ւ̃|�C���^
	std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// ���������G�t�F�N�g
	std::unique_ptr<HitParticle> m_hitParticle;

	// �p�����[�^
	nlohmann::json m_parameters;

	// ���_�̐�
	int m_pointXNum;
	int m_pointYNum;
	int m_pointZNum;
};

#endif // SOFTBODY_BAR_DEFINED
