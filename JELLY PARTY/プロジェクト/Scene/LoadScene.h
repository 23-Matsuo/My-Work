//--------------------------------------------------------------------------------------
// File: LoadScene.h
//
// ���[�h�V�[���N���X
//
// Usage: �V�[���J�ڂ̊Ԃɂ��̃V�[�����͂���
// 
// Date: 2023.7.21
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#include "MyLib/SceneManager.h"

#include "LoadSceneObjects/Loading.h"
class LoadScene : public LoadingScreen
{
public:
	
	// �R���X�g���N�^
	LoadScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update() override;

	// �`��
	void Render() override;

	// �I������
	void Finalize() override;

	// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateDeviceDependentResources() override;

	// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateWindowSizeDependentResources() override;

	// �f�o�C�X���X�g�������ɌĂяo�����֐�
	void OnDeviceLost() override;


private:
	std::unique_ptr<Loading> m_loading;

};