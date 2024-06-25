//--------------------------------------------------------------------------------------
// File: PushSpace\.h
//
// �^�C�g���w�i�N���X
//
// Date: 2023/12/02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef PUSH_SPACE_DIFINED
#define PUSH_SPACE_DIFINED

class PushSpace
{
public:

	enum STATE
	{
		IDLE,
		POPOUT,
		POPIN,
	};

public:
	// �R���X�g���N�^
	PushSpace(const float& width, const float& height);

	// �f�X�g���N�^
	~PushSpace();

	// �X�V
	void Update(const float& elapsedTime);

	// �`��
	void Render(); 

	// �|�b�v�A�j���[�V����
	void Pop(const float& elapsedTime);

	// ��Ԑݒ�
	void SetState(const STATE& state) { m_state = state; }
	// �傫���ݒ�
	void SetScale(const DirectX::SimpleMath::Vector2& scale) { m_scale = scale; }

	// ��Ԃ��擾
	const STATE& GetState() { return m_state; }

private:

	// �|�b�v�A�E�g
	void PopOut();
	// �|�b�v�C��
	void PopIn();

private:

	// �摜�̏��
	STATE m_state;

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �ʒu
	DirectX::SimpleMath::Vector2 m_position;

	// �摜�̃X�P�[��
	DirectX::SimpleMath::Vector2 m_scale;

	// �A�j���[�V��������
	float m_time;

	// �A�j���[�V�����p�����[�^
	nlohmann::json m_popAnimation;

	// �摜�̏����̑傫��
	DirectX::SimpleMath::Vector2 m_defaultScale;

};

#endif // PUSH_SPACE_DIFINED
