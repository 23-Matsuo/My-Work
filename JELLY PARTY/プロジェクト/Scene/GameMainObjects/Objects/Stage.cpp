//--------------------------------------------------------------------------------------
// File: Stage.cpp
//
// �X�e�[�W��̃I�u�W�F�N�g�Ǘ��N���X
//
// Date: 2023.7.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Stage.h"

using namespace DirectX;

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Stage::Stage(
	DX::DeviceResources* pDR,
	DirectX::Model* pModel,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Matrix& rotation,
	const float& scale
)
	:
	m_pDR(pDR)
	, m_kitchenModel(pModel)
	, m_position(position)
	, m_rotation(rotation)
	, m_scale(scale)
	, m_isActive(true)

{

}

//------------------------------------------
// ������
//------------------------------------------
void Stage::Initialize()
{
}

//------------------------------------------
// �X�V
//------------------------------------------
void Stage::Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position)
{
	UNREFERENCED_PARAMETER(position);
	UNREFERENCED_PARAMETER(elapsedTime);

}

//------------------------------------------
// �`��
//------------------------------------------
void Stage::Render(
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
	// �L�b�`�����f���`��
	m_kitchenModel->Draw(context, *states, world, view, projection, false);
}

//------------------------------------------
/// �I������
//------------------------------------------
void Stage::Finalize()
{
}

