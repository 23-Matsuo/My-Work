//--------------------------------------------------------------------------------------
// File: GameObject.h
//
// �Q�[������3D�I�u�W�F�N�g�̊��N���X
//
// Last Update : 2023.10.26
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef GAMEOBJECT_DEFINED
#define GAMEOBJECT_DEFINED

#include "DirectXHelpers.h"
#include "SimpleMath.h"
#include "StepTimer.h"
#include "Model.h"

class GameObject
{
public:

	// �f�X�g���N�^
	virtual ~GameObject() = default;

	// ������
	virtual void Initialize() = 0;

	// �X�V
	virtual void Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position) = 0;

	// �`��
	virtual void Render() = 0;

	// �㏈��
	virtual void Finalize() = 0;
	
public:

	// ID���擾
	virtual int GetObjectID() = 0;

	// �ʒu���擾
	virtual DirectX::SimpleMath::Vector3 GetPosition() = 0;

	// ���f�����擾
	virtual DirectX::Model* GetModel() = 0;

	// �I�u�W�F�N�g�̏�Ԃ�ݒ�
	virtual void SetActive(bool isActive) = 0;
	

private:
	
};

#endif // GAMEOBJECT_DEFINED