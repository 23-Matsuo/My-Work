//--------------------------------------------------------------------------------------
// File: StageSoftBodyManager.cpp
//
// �X�e�[�W��̃\�t�g�{�f�B�Ǘ��N���X
//
// Date: 2023.10.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------


#include "pch.h"
#include "StageSoftBodyManager.h"
#include "MyLib/ObjectCollision.h"

// �͈͐����֐�
template<class T> T Clamp(T value, T min, T max)
{
	if (value <= min)
	{
		return min;
	}
	else if (value >= max)
	{
		return max;
	}
	return value;
}

// �o�[�̉�]���x
const float StageSoftBodyManager::ROTATE_SPEED = 0.0008f;

// ��]�̍ō����x
const float StageSoftBodyManager::MAX_SPEED = 0.0425f;

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
StageSoftBodyManager::StageSoftBodyManager()
	:
	m_anglarVelocity(0.0f)
	, m_sceneState(SceneState::COUNTDOWN)
{
	// �����̎��쐬
	m_center = std::make_unique<CenterSoftBody>();
	// �o�[�쐬
	m_bar = std::make_unique<SoftBodyBar>(m_center->GetJointPoints());
	// �[���[�L���[�u�쐬
	m_cube = std::make_unique<SoftBodyCube>();
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
StageSoftBodyManager::~StageSoftBodyManager()
{
}

//------------------------------------------
// �X�V
//------------------------------------------
void StageSoftBodyManager::Update(const int& spendTime)
{
	// ���U���g�Ȃ�[���[�L���[�u�ȊO�������Ȃ�
	if (m_sceneState == SceneState::RESULT) 
	{
		m_cube->ResultUpdate();
		return;
	} 

	// ��]�̑������v�Z
	m_anglarVelocity += DirectX::XMConvertToRadians(ROTATE_SPEED);
	// ��]�̑����𐧌�
	m_anglarVelocity = Clamp(m_anglarVelocity, 0.0f, MAX_SPEED);
	// ��]�s����쐬
	m_rotation = DirectX::SimpleMath::Matrix::CreateRotationY(m_anglarVelocity);

	// �[���[�L���[�u�̍X�V
	m_cube->Update(spendTime, m_center.get());

	// �����̎��X�V
	m_center->Update(m_rotation);

	// �o�[�̍X�V
	m_bar->Update(m_rotation, m_center->GetCylinder(), m_anglarVelocity);
}

//------------------------------------------
// �`��
//------------------------------------------
void StageSoftBodyManager::Render()
{

	// ���U���g�Ȃ�[���[�L���[�u�ȊO�`�悵�Ȃ�
	if (m_sceneState == SceneState::RESULT)
	{
		m_cube->ResultRender();
		return;
	}

	if (m_sceneState != COUNTDOWN)
	{
		// �[���[�L���[�u�̕`��
		m_cube->Render();
	}

	// �o�[�̕`��
	m_bar->Render();

	// �����̎��`��
	m_center->Render();
}


//------------------------------------------
// �`��
//------------------------------------------
void StageSoftBodyManager::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	if (m_sceneState == SceneState::RESULT && m_cube->GetCountFinished())
	{
		// ���U���g���X�R�A�J�E���g���I����Ă�����
		// �e�̕`��
		m_cube->ResultShadowRender(lightView, lightProjection);
		return;
	}
	else if (m_sceneState == SceneState::RESULT && !m_cube->GetCountFinished())
	{
		// ���U���g���X�R�A�J�E���g���I����Ă��Ȃ�
		return;
	}

	if (m_sceneState != COUNTDOWN)
	{
		// �[���[�L���[�u�̕`��
		m_cube->ShadowRender(lightView, lightProjection);
	}

	// �o�[�̕`��
	m_bar->ShadowRender(lightView, lightProjection);

	// �����̎��`��
	m_center->ShadowRender(lightView, lightProjection);
}


//------------------------------------------
// ���U���g���̏�����
//------------------------------------------
void StageSoftBodyManager::CubeResultInitialize(Score* pScore)
{
	m_cube->ResultInitialize(pScore);
}