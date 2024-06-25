//--------------------------------------------------------------------------------------
// File: ISoftBodyFactory.h
//
// ソフトボディを作成するインターフェース
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
	// 箱状のソフトボディを作成
	virtual std::unique_ptr<SoftBodyBox> CreateBox(
		const int& xNum, const int& yNum, const int& zNum,					// 頂点数
		const DirectX::SimpleMath::Vector3& position,						// 位置
		const DirectX::SimpleMath::Quaternion& rotation,					// 回転
		const DirectX::SimpleMath::Vector3& scale,							// スケール
		const DirectX::SimpleMath::Color& color,							// 色
		ID3D11ShaderResourceView *texture,									// テクスチャ
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED		// 状態
	) = 0;

	// コーン状のソフトボディを作成
	virtual std::unique_ptr<SoftBodyCorn> CreateCorn(
		const int& xNum, const int& zNum,									// 頂点数
		const DirectX::SimpleMath::Vector3& position,						// 位置
		const DirectX::SimpleMath::Quaternion& rotation,					// 回転
		const DirectX::SimpleMath::Vector3& scale,							// スケール
		const DirectX::SimpleMath::Color& color,							// 色
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED		// テクスチャ
	) = 0;																	// 状態

	// 円柱状のソフトボディを作成
	virtual std::unique_ptr<SoftBodyCylinder> CreateCylinder(
		const int& xNum, const int& yNum,									// 頂点数
		const float& radius,												// 半径
		const float& height,												// 高さ
		const DirectX::SimpleMath::Vector3& position,						// 位置
		const DirectX::SimpleMath::Quaternion& rotation,					// 回転
		const DirectX::SimpleMath::Vector3& scale,							// スケール
		const DirectX::SimpleMath::Color& color,							// 色
		ID3D11ShaderResourceView* texture,									// テクスチャ
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED		// 状態
	) = 0;
};

#endif	// ISOFTBODY_FACTORY_DEFINED
