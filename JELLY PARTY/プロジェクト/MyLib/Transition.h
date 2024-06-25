//--------------------------------------------------------------------------------------
// File: Transition.h
//
// �g�����W�V�����N���X
//
// Date: 2023/07/30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef TRANSITION_DEFINED
#define TRANSITION_DEFINED

class Transition
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 slidePos;
	};

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;
public:
	// �g�����W�V�������
	enum class STATE
	{
		IDLE,
		SLIDEIN,
		SLIDEOUT
	};

	// �X���C�h�̑��x
	static const float TRANS_SPEED;

public:

	// �R���X�g���N�^
	Transition();
	// �f�X�g���N�^
	~Transition();

	// ������
	void Initialize();

	// �X�V
	void Update();

	// �`��
	void Render();

	// ��Ԃ�ݒ�
	void SetState(STATE state)	{ m_state = state; }

	// ��Ԃ��擾
	STATE GetState() const		{ return m_state; }

	void Restart();
private:

	// �摜�̃X���C�h�C��
	void SlideIn(const float time, float elapsedTime);

	// �摜�̃X���C�h�A�E�g
	void SlideOut(const float time, float elpasedTime);

private:
	// ���
	STATE m_state;

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	DirectX::VertexPositionColorTexture m_vertex;

	DirectX::SimpleMath::Vector4 m_slideRatio;

};
#endif // TRANSITION_DEFINED
