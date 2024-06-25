//--------------------------------------------------------------------------------------
// File: SoftBodyFactory.cpp
//
// �\�t�g�{�f�B���쐬����N���X
//
// Last Update: 2023.10.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"

#include "SoftBodyFactory.h"

//------------------------------------------
// ���̌`�𐶐�
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
	// �C���X�^���X����
	box.reset(new SoftBodyBox(xNum, yNum, zNum, position, rotation, scale, color, state));
	// �\�t�g�{�f�B����
	box->CreateFigure();
	// ������
	box->Intialize(texture);

	return std::move(box);
}

//------------------------------------------
// �~���𐶐�
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
	// �C���X�^���X����
	cylinder.reset(new SoftBodyCylinder(xNum, yNum, radius, height, position, rotation, scale, color, state));
	// �\�t�g�{�f�B����
	cylinder->CreateFigure();
	// ������
	cylinder->Intialize(texture);

	return std::move(cylinder);
}

