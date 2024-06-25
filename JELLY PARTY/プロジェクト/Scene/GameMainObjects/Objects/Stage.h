//--------------------------------------------------------------------------------------
// File: Stage.h
//
// �X�e�[�W��̃I�u�W�F�N�g�Ǘ��N���X
//
// Date: 2023.7.20
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef STAGE_DEFINED
#define STAGE_DEFINED

class Stage
{
public:

	// �R���X�g���N�^
	Stage(
		DX::DeviceResources* pDR,
		DirectX::Model* pModel,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Matrix& rotation,
		const float& scale
	);
	// �f�X�g���N�^
	~Stage() 
	{
		Finalize();
	};
	// ������
	void Initialize();

	// �X�V
	void Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position);

	// �`��
	void Render(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection,
		std::function<void __cdecl()> function
	);

	// �I������
	void Finalize();

public:
	// ID���擾
	int GetObjectID() { return 0; }

	// �ʒu���擾
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	// ���f�����擾
	DirectX::Model* GetModel() { return m_kitchenModel; }

	// �I�u�W�F�N�g�̏�Ԃ�ݒ�
	void SetActive(bool isActive) { m_isActive = isActive; }

private:

	// �f�o�C�X���\�[�X�ւ̃|�C���^
	DX::DeviceResources* m_pDR;
	// ���f��
	DirectX::Model* m_kitchenModel;
	// �|�W�V����
	DirectX::SimpleMath::Vector3 m_position;
	// ��]�s��
	DirectX::SimpleMath::Matrix m_rotation;
	// �g�k
	float m_scale;
	// ���
	bool m_isActive;

};

#endif // STAGE_DEFINED