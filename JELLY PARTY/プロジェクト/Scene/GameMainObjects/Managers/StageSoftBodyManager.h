//--------------------------------------------------------------------------------------
// File: StageSoftBodyManager.h
//
// �X�e�[�W��̃\�t�g�{�f�B�Ǘ��N���X
//
// Date: 2023.11.26
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef STAGE_SOFTBODY_MANAGER_DEFINED
#define STAGE_SOFTBODY_MANAGER_DEFINED

#include "MyLib/SoftBody/SoftBodyFactory.h"
#include "Scene/GameMainObjects/Objects/SoftBodyBar.h"
#include "Scene/GameMainObjects/Objects/CenterSoftBody.h"
#include "Scene/GameMainObjects/Objects/SoftBodyCube.h"

class StageSoftBodyManager
{
public:

	enum SceneState
	{
		COUNTDOWN,
		MAIN,
		RESULT,
	};

	// �o�[�̉�]���x
	static const float ROTATE_SPEED;

	// ��]�̍ō����x
	static const float MAX_SPEED;

public:

	// �R���X�g���N�^
	StageSoftBodyManager();

	// �f�X�g���N�^
	~StageSoftBodyManager();

	// �X�V
	void Update(const int& spendTime);

	// �`��
	void Render();

	// �e�`��
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);


public:

	// �[���[�L���[�u�̃��U���g���̏�����
	void CubeResultInitialize(Score* pScore);

	// �V�[���̏�Ԃ�ݒ�
	void SetSceneState(SceneState state) { m_sceneState = state; }
	
	// �J�E���g�I���������ǂ���
	const bool& GetCountFinished() { return m_cube->GetCountFinished(); }

private:

	// �����̂��
	std::unique_ptr<CenterSoftBody> m_center;

	// �o�[�̂��
	std::unique_ptr<SoftBodyBar> m_bar;

	// �[���[�L���[�u�̃\�t�g�{�f�B
	std::unique_ptr<SoftBodyCube> m_cube;

private:

	// ��]�s��
	DirectX::SimpleMath::Matrix m_rotation;
	// �o�[�̉�]���x
	float m_anglarVelocity;

	// �V�[���̏��
	SceneState m_sceneState;
};

#endif // SOFTBODY_FLOOR_MANAGER_DEFINED