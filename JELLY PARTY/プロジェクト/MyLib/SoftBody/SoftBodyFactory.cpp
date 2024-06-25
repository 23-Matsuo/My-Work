//--------------------------------------------------------------------------------------
// File: SoftBodyFactory.cpp
//
// ソフトボディを作成するクラス
//
// Last Update: 2023.10.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"

#include "SoftBodyFactory.h"

//------------------------------------------
// 箱の形を生成
//------------------------------------------
std::unique_ptr<SoftBodyBox> SoftBodyFactory::CreateBox(
	const int& xNum, const int& yNum, const int& zNum,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotation,
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Color& color,
	ID3D11ShaderResourceView* texture,
	const PhysicalPoint::STATE& state
)
{
	std::unique_ptr<SoftBodyBox> box;
	// インスタンス生成
	box.reset(new SoftBodyBox(xNum, yNum, zNum, position, rotation, scale, color, state));
	// ソフトボディ生成
	box->CreateFigure();
	// 初期化
	box->Intialize(texture);

	return std::move(box);
}

//------------------------------------------
// 円柱を生成
//------------------------------------------
std::unique_ptr<SoftBodyCylinder> SoftBodyFactory::CreateCylinder(
	const int& xNum, const int& yNum,
	const float& radius,
	const float& height,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotation,
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Color& color,
	ID3D11ShaderResourceView* texture,
	const PhysicalPoint::STATE& state
)
{
	std::unique_ptr<SoftBodyCylinder> cylinder;
	// インスタンス生成
	cylinder.reset(new SoftBodyCylinder(xNum, yNum, radius, height, position, rotation, scale, color, state));
	// ソフトボディ生成
	cylinder->CreateFigure();
	// 初期化
	cylinder->Intialize(texture);

	return std::move(cylinder);
}

