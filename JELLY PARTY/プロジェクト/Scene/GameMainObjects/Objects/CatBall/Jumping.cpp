//--------------------------------------------------------------------------------------
// File: Jumping.cpp
//
// �v���C���[�̃W�����v��ԃN���X
//
// Last Update: 2023.01.15
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "CatSphere.h"

#include "MyLib/SoundUtility.h"
//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
CatSphere::Jumping::Jumping(CatSphere* pSphere)
	:
	m_pSphere(pSphere)
{
	// �W�����v��
	m_jumpSound = Resources::GetInstance()->GetSound(L"Jump");
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
CatSphere::Jumping::~Jumping()
{
}

//------------------------------------------
// ������
//------------------------------------------
void CatSphere::Jumping::Initialize()
{
}

//------------------------------------------
// �G�t�F�N�g�`��
//------------------------------------------
void CatSphere::Jumping::Render()
{

}

//------------------------------------------
// �X�V
//------------------------------------------
void CatSphere::Jumping::Update(const float& elapsedTime)
{
	// �W�����v�͂��v�Z
	DirectX::SimpleMath::Vector3 force = m_pSphere->GetTotalForce();
	force.y += m_pSphere->GetJumpForce();
 	m_pSphere->SetTotalForce(force);

	// ����炷
	PlaySound(m_jumpSound, false);

	// �����j���O��Ԃɐݒ�
	m_pSphere->SetCurrentState(m_pSphere->GetRunning());
}