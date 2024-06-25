//--------------------------------------------------------------------------------------
// File: SoftBodyFactory.h
//
// ソフトボディを作成するクラス
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
	// 箱状のソフトボディを作成
	static std::unique_ptr<SoftBodyBox> CreateBox(
		const int& xNum, const int& yNum, const int& zNum,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotation,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Color& color,
		ID3D11ShaderResourceView* texture,
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED
	);

	// コーン状のソフトボディを作成
	static std::unique_ptr<SoftBodyCorn> CreateCorn(
		const int& xNum, const int& zNum,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotation,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Color& color,
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED
	);

	// 円柱状のソフトボディを作成
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
