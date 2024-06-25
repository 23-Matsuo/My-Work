//--------------------------------------------------------------------------------------
// File: HowToPlay\.h
//
// �^�C�g���w�i�N���X
//
// Date: 2023/12/02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef HOW_TO_PLAY_DIFINED
#define HOW_TO_PLAY_DIFINED

class HowToPlay
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
	HowToPlay(const float& width, const float& height);

	// �f�X�g���N�^
	~HowToPlay();

	// �X�V
	void Update(const float& elapsedTime);

	// �`��
	void Render(); 

	// �|�b�v�A�j���[�V����
	void Pop(const float& elapsedTime);

	// ��Ԑݒ�
	void SetState(const STATE& state) { m_state = state; }
	// ��Ԃ��擾
	const STATE& GetState() { return m_state; }

	// �X�P�[���擾
	const DirectX::SimpleMath::Vector2& GetScale() { return m_scale; }

	// �f�t�H���g�X�P�[���擾
	const DirectX::SimpleMath::Vector2& GetDefaultScale() { return m_defaultScale; }

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

	// �|�b�v�������ǂ���
	bool m_popped;

	// �A�j���[�V�����p�����[�^
	nlohmann::json m_popAnimation;

	// �摜�̏����̑傫��
	DirectX::SimpleMath::Vector2 m_defaultScale;

};

#endif // HOW_TO_PLAY_DIFINED
