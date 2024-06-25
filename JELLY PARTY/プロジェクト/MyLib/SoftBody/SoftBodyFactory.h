//--------------------------------------------------------------------------------------
// File: SoftBodyFactory.h
//
// �\�t�g�{�f�B���쐬����N���X
//
// Last Update: 2023.10.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef SOFTBODY_FACTORY_DEFINED
#define SOFTBODY_FACTORY_DEFINED

#include "DirectXHelpers.h"
#include "SimpleMath.h"

#include "ISoftBodyFactory.h"

class SoftBodyFactory : public ISoftBodyFactory
{
public:
	// ����̃\�t�g�{�f�B���쐬
	static std::unique_ptr<SoftBodyBox> CreateBox(
		const int& xNum, const int& yNum, const int& zNum,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotation,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Color& color,
		ID3D11ShaderResourceView* texture,
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED
	);

	// �R�[����̃\�t�g�{�f�B���쐬
	static std::unique_ptr<SoftBodyCorn> CreateCorn(
		const int& xNum, const int& zNum,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotation,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Color& color,
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED
	);

	// �~����̃\�t�g�{�f�B���쐬
	static std::unique_ptr<SoftBodyCylinder> CreateCylinder(
		const int& xNum, const int& yNum,
		const float& radius,
		const float& height,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotation,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Color& color,
		ID3D11ShaderResourceView* texture,
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED
	);
};

#endif			// SOFTBODY_FACTORY_DEFINED
