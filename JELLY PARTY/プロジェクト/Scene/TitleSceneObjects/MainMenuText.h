//--------------------------------------------------------------------------------------
// File: MainMenuText.h
//
// ���j���[�̕����N���X
//
// Date: 2023/12/02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef MAIN_MENU_TEXT_DIFINED
#define MAIN_MENU_TEXT_DIFINED

class MainMenuText
{
public:

	static const DirectX::SimpleMath::Vector2 SCALE;

	enum STATE
	{
		IDLE,
		POPOUT,
		POPIN,
	};
public:
	// �R���X�g���N�^
	MainMenuText(const float& width, const float& height);

	// �f�X�g���N�^
	~MainMenuText();

	// �X�V
	bool Update(const float& elapsedTime);

	// �`��
	void Render(); 

	void SetPosition(const DirectX::SimpleMath::Vector2& position)	{ m_position = position; }
	void SetState(const STATE& state) { m_state = state; }

	const DirectX::SimpleMath::Vector2& GetPosition()				{ return m_position; }
	const STATE& GetState() { return m_state; }

private:

	// �o�l�̓���
	void Damping(float* scale, float* velocity, float defaultScale);

private:

	// ���
	STATE m_state;

	// �E�B���h�E�T�C�Y
	DirectX::SimpleMath::Vector2 m_windowSize;

	// �ʒu
	DirectX::SimpleMath::Vector2 m_position;

	// �����x
	float m_acceleration;

	// �d��
	float m_gravity;

	// �摜�̑傫��
	DirectX::SimpleMath::Vector2 m_scale;

	// �o�l�̗�
	DirectX::SimpleMath::Vector2 m_damping;

	// �A�j���[�V��������
	float m_time;

	// �N�����A�j���[�V�����t���O
	bool m_animated;

	float m_alpha;
};

#endif // MAIN_MENU_TEXT_DIFINED
