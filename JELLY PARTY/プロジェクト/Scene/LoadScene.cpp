//--------------------------------------------------------------------------------------
// File: LoadScene.cpp
//
// ���[�h�V�[���N���X
//
// Date: 2023.7.21
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "LoadScene.h"
#include "PlayScene.h"
#include "MyLib/Resources.h"
#include "MyLib/Graphics.h"

using namespace DirectX;

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
LoadScene::LoadScene()
{
}

//------------------------------------------
// ������
//------------------------------------------
void LoadScene::Initialize()
{

	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	m_loading = std::make_unique<Loading>();
}

//------------------------------------------
/// �X�V
//------------------------------------------
void LoadScene::Update()
{
	m_loading->Update();
}

//------------------------------------------
/// �`��
//------------------------------------------
void LoadScene::Render()
{
	m_loading->Render();
}

//------------------------------------------
/// �I������
//------------------------------------------
void LoadScene::Finalize()
{
	m_loading.reset();
}

//------------------------------------------
/// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
//------------------------------------------
void LoadScene::CreateDeviceDependentResources()
{

}

//------------------------------------------
/// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
//------------------------------------------
void LoadScene::CreateWindowSizeDependentResources()
{
}

//------------------------------------------
/// �f�o�C�X���X�g�������ɌĂяo�����֐�
//------------------------------------------
void LoadScene::OnDeviceLost()
{
	Finalize();
}
