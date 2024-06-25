//--------------------------------------------------------------------------------------
// File: PhysicalObject.h
//
// �����v�Z�ŕK�v�ȏ��������N���X
//
//
// Date: 2023.5.10
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef PHYSICAL_OBJECT_DEFINED
#define PHYSICAL_OBJECT_DEFINED


#include "DirectXHelpers.h"
#include "SimpleMath.h"
#include "StepTimer.h"

class PhysicalObject
{
public:
	
	// �R���X�g���N�^
	PhysicalObject() :
		m_position{}
	{};
	// �f�X�g���N�^
	~PhysicalObject() {};

	// �X�V
	virtual void Update(const DX::StepTimer& timer) = 0;

	// �|�W�V������ݒ�
	virtual void SetPosition(const DirectX::SimpleMath::Vector3 position)	{ m_position = position; }

public:

	// �|�W�V�������擾
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }

private:

	// �|�W�V����
	DirectX::SimpleMath::Vector3 m_position;
	 
};

#endif // PHYSICAL_OBJECT_DEFINED