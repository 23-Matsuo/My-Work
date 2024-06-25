//--------------------------------------------------------------------------------------
// File: ISoftBodyFactory.h
//
// �\�t�g�{�f�B���쐬����C���^�[�t�F�[�X
//
// Last Update: 2023.10.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef ISOFTBODY_FACTORY_DEFINED
#define ISOFTBODY_FACTORY_DEFINED

#include "SoftBodyBox.h"
#include "SoftBodyCorn.h"
#include "SoftBodyCylinder.h"
#include "WICTextureLoader.h"

class ISoftBodyFactory
{
public:
	// ����̃\�t�g�{�f�B���쐬
	virtual std::unique_ptr<SoftBodyBox> CreateBox(
		const int& xNum, const int& yNum, const int& zNum,					// ���_��
		const DirectX::SimpleMath::Vector3& position,						// �ʒu
		const DirectX::SimpleMath::Quaternion& rotation,					// ��]
		const DirectX::SimpleMath::Vector3& scale,							// �X�P�[��
		const DirectX::SimpleMath::Color& color,							// �F
		ID3D11ShaderResourceView *texture,									// �e�N�X�`��
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED		// ���
	) = 0;

	// �R�[����̃\�t�g�{�f�B���쐬
	virtual std::unique_ptr<SoftBodyCorn> CreateCorn(
		const int& xNum, const int& zNum,									// ���_��
		const DirectX::SimpleMath::Vector3& position,						// �ʒu
		const DirectX::SimpleMath::Quaternion& rotation,					// ��]
		const DirectX::SimpleMath::Vector3& scale,							// �X�P�[��
		const DirectX::SimpleMath::Color& color,							// �F
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED		// �e�N�X�`��
	) = 0;																	// ���

	// �~����̃\�t�g�{�f�B���쐬
	virtual std::unique_ptr<SoftBodyCylinder> CreateCylinder(
		const int& xNum, const int& yNum,									// ���_��
		const float& radius,												// ���a
		const float& height,												// ����
		const DirectX::SimpleMath::Vector3& position,						// �ʒu
		const DirectX::SimpleMath::Quaternion& rotation,					// ��]
		const DirectX::SimpleMath::Vector3& scale,							// �X�P�[��
		const DirectX::SimpleMath::Color& color,							// �F
		ID3D11ShaderResourceView* texture,									// �e�N�X�`��
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED		// ���
	) = 0;
};

#endif	// ISOFTBODY_FACTORY_DEFINED
