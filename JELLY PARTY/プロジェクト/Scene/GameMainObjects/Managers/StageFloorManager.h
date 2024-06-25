//--------------------------------------------------------------------------------------
// File: StageFloorManager.h
//
// �X�e�[�W�̏��Ǘ�
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------
#pragma once

#ifndef STAGE_FLOOR_MANAGER_DEFINED
#define STAGE_FLOOR_MANAGER_DEFINED

#include "DeviceResources.h"
#include "ImaseLib/ObjCollision.h"
#include "MyLib/GameObject.h"
#include "Scene/GameMainObjects/Objects/Floor.h"
#include "Scene/GameMainObjects/Objects/Stage.h"

class StageFloorManager
{
public:

	// ���I�u�W�F�N�g�̐�
	static const int NUM;

	static const DirectX::SimpleMath::Vector3 FLOOR_RADUIS;

	static const DirectX::SimpleMath::Vector3 POSITION;

public:
	
	// �R���X�g���N�^
	StageFloorManager(DX::DeviceResources* pDR);

	// �f�X�g���N�^
	~StageFloorManager();

	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection,
		std::function<void __cdecl()> function
	);

	// �I������
	void Finalize();

public:
	
	// ���f���̓����蔻����擾
	std::vector<Imase::ObjCollision> GetObjCollisions() { return m_objCollisions; }

private:
	// ���쐬
	std::unique_ptr<Floor> CreateFloor(
		DirectX::Model* model,
		DirectX::SimpleMath::Vector3 position,
		DirectX::SimpleMath::Matrix rotation,
		const float& scale
	);

	// �����蔻��쐬
	Imase::ObjCollision CreateCollider(
		const wchar_t* path,
		DirectX::SimpleMath::Vector3 position,
		DirectX::SimpleMath::Matrix rotation,
		const float& scale
	);

	// �f�o�C�X���\�[�X�ւ̃|�C���^
	DX::DeviceResources* m_pDR;

	// �Փ˔���̕\���I�u�W�F�N�g�ւ̃|�C���^
	std::unique_ptr<Imase::DisplayCollision> m_displayCollision;

	// �Փ˔���̃I�u�W�F�N�g
	std::vector<Imase::ObjCollision> m_objCollisions;

	// ���̃I�u�W�F�N�g
	std::unique_ptr<Floor> m_floor;
	// �X�e�[�W�̃I�u�W�F�N�g
	std::unique_ptr<Stage> m_stage;

	DirectX::SimpleMath::Vector3 m_position;
};

#endif // STAGE_FLOOR_MANAGER_DEFINED