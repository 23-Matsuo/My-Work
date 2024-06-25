//--------------------------------------------------------------------------------------
// File: Floor.cpp
//
// �X�e�[�W�̏�
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Floor.h"


//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Floor::Floor(
	DX::DeviceResources* pDR,
	DirectX::Model* pModel,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Matrix& rotation,
	const float& scale
)
	:
	m_pDR(pDR)
	, m_floorModel(pModel)
	, m_position(position)
	, m_rotation(rotation)
	, m_scale(scale)
	, m_isActive(true)
{

}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
Floor::~Floor()
{

}

//------------------------------------------
// ������
//------------------------------------------
void Floor::Initialize()
{
}

//------------------------------------------
// �X�V
//------------------------------------------
void Floor::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position)
{
	UNREFERENCED_PARAMETER(position);
	UNREFERENCED_PARAMETER(elapsedTime);
}

//------------------------------------------
// �`��
//------------------------------------------
void Floor::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection,
	std::function<void __cdecl()> function
)
{

	// ���[���h�s��
	DirectX::SimpleMath::Matrix world;

	DirectX::SimpleMath::Matrix translationMat;
	DirectX::SimpleMath::Matrix rotationMat;
	DirectX::SimpleMath::Matrix scaleMat;

	// �g�k�s��쐬
	scaleMat = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
	// ��]�s��쐬
	rotationMat = m_rotation;
	// �ړ��s��쐬
	translationMat = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	// ���[���h�s��쐬
	world = scaleMat * rotationMat * translationMat;
	// ���ꃂ�f���`��
	m_floorModel->Draw(context, *states, world, view, projection, false, function);
}


//------------------------------------------
// �I������
//------------------------------------------
void Floor::Finalize()
{

}
