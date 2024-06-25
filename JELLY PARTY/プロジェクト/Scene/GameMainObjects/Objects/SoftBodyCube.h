//--------------------------------------------------------------------------------------
// File: SoftBodyCube.h
//
// ��]����o�[
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef SOFTBODY_CUBE_DEFINED
#define SOFTBODY_CUBE_DEFINED

#include "MyLib/SoftBody/SoftBodyFactory.h"
#include "Scene/GameMainObjects/Objects/SoftBodyBar.h"
#include "Scene/GameMainObjects/Objects/CenterSoftBody.h"
#include "Scene/Effects/GetParticle.h"
#include "Scene/GameMainObjects/UI/Score.h"

class SoftBodyCube
{
public:

	// �V�[���̏��
	enum SceneState
	{
		MAIN,
		RESULT	
	};

public:

	// �R���X�g���N�^
	SoftBodyCube();

	// �f�X�g���N�^
	~SoftBodyCube();

	// ������
	void CreateCube();

	// �X�V
	void Update(const int& spendingTime, CenterSoftBody* pCylinder);

	// �`��
	void Render();

	// �e�`��
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

public:

	// ���U���g���̏�����
	void ResultInitialize(Score* pScore);

	// ���U���g���̍X�V����
	void ResultUpdate();

	// ���U���g���̕`��
	void ResultRender();

	// ���U���g���̉e�`��
	void ResultShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

	// �J�E���g�I���������ǂ���
	const bool& GetCountFinished() { return m_pScore->GetResultCountFinish(); }

private:

	// �ǂ̏�Ԃ��m�F
	void SetPointsState();

	// �[���[�L���[�u���o��
	void AppearCubes();

	// �X�V
	void CubesUpdate(const int& index);

	void HitBar(const int& index, SoftBodyBar* pBar, const float& speed);

	// �w�肳�ꂽ�ԍ��̃I�u�W�F�N�g�̃p�����[�^������
	void ResetParameters(const int& index);
	
	// ��������
	void DupricateCube();

private:

	// �[���[�L���[�u
	std::vector<std::shared_ptr<SoftBodyBox>> m_boxes;

	// �o������(�t���[������)
	int m_spawnTime;

	// �ő�o����
	int m_cubeMaxNum;
	// �o����
	int m_cubeNum;

	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// �Փ˔���̕\���I�u�W�F�N�g�ւ̃|�C���^
	std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// �����蔻��I�u�W�F�N�g
	std::vector<DirectX::BoundingSphere> m_collisions;

	// �擾���ւ̃|�C���^
	DirectX::SoundEffectInstance* m_hitSound;

	// �o�ߎ���
	int m_spendingTime;

	// �[���[�L���[�u�̃C���f�b�N�X�ԍ�
	int m_cubeIndex;

	// �o������
	bool m_appeared;

	// �擾���̃p�[�e�B�N��
	std::unique_ptr<GetParticle> m_getParticle;

private:

	// ���U���g���̏o������
	int m_timer;

	// ���U���g���̃L���������ʂ������܂ł̎���
	float m_interval;

	// �X�R�A�\���I�u�W�F�N�g�ւ̃|�C���^
	Score* m_pScore;

	// ���Z�b�g�̈ʒu
	DirectX::SimpleMath::Vector3 m_resultResetPosition;

	// �p�����[�^
	nlohmann::json m_parameters;
};

#endif // SOFTBODY_CUBE_DEFINED
