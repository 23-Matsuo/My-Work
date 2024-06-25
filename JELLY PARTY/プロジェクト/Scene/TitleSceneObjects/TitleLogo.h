//--------------------------------------------------------------------------------------
// File: TitleLogo.h
//
// �^�C�g�����S�N���X
//
// Date: 2023/12/02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef TITLE_LOGO_DIFINED
#define TITLE_LOGO_DIFINED

class TitleLogo
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
	TitleLogo(const float& width, const float& height);

	// �f�X�g���N�^
	~TitleLogo();

	void Initialize();

	// �X�V
	bool Update(const float& elapsedTime);

	// �`��
	void Render(); 

	void Pop(const float& elapsedTime);



	void SetPosition(const DirectX::SimpleMath::Vector2& position)	{ m_position = position; }
	void SetScale(const DirectX::SimpleMath::Vector2& scale)	{ m_scale = scale; }
	void SetState(const STATE& state) { m_state = state; }

	const DirectX::SimpleMath::Vector2& GetPosition()				{ return m_position; }
	const STATE& GetState() { return m_state; }

private:

	void PopOut();
	void PopIn();

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

	// �A�j���[�V�����p�����[�^
	nlohmann::json m_popAnimation;

	// �摜�̏����̑傫��
	DirectX::SimpleMath::Vector2 m_defaultScale;
};

#endif // TITLE_LOGO_DIFINED
