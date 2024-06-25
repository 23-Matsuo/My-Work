//--------------------------------------------------------------------------------------
// File: UserInterface.cpp
//
// ���[�U�[�C���^�[�t�F�[�X�N���X
//
// Last Update: 2023.10.30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------
#pragma once

#ifndef USERINTERFACE_DEFINED
#define USERINTERFACE_DEFINED


#include "StepTimer.h"
#include <DeviceResources.h>
#include <vector>
//UI�̃A���J�[�|�C���g�̗񋓐�
enum ANCHOR
{
	TOP_LEFT = 0,
	TOP_CENTER,
	TOP_RIGHT,

	MIDDLE_LEFT,
	MIDDLE_CENTER,
	MIDDLE_RIGHT,

	BOTTOM_LEFT,
	BOTTOM_CENTER,
	BOTTOM_RIGHT

};

class UserInterface
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4	windowSize;
		DirectX::SimpleMath::Vector2	scale;
		DirectX::SimpleMath::Vector2	textureSize;

	};
public:
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	static const DirectX::SimpleMath::Vector2 DEFAULT_SCALE;

	UserInterface();
	~UserInterface();
	// �e�N�X�`�������[�h
	void LoadTexture(const wchar_t* name);
	// �V�F�[�_�[�쐬
	void Create(
		const wchar_t* name
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, ANCHOR anchor);
	// �`��
	void Render();

public:
	void SetScale(DirectX::SimpleMath::Vector2 scale)		{ m_scale = scale; }
	void SetRenderRatio(float ratio)						{ m_renderRatio = ratio; }
	void SetPosition(DirectX::SimpleMath::Vector2 position) { m_position = position; }
	void SetColor(DirectX::SimpleMath::Color color)			{ m_color = color; }
	void SetAnchor(ANCHOR anchor)							{ m_anchor = anchor; }
public:
	DirectX::SimpleMath::Vector2 GetScale()		{ return m_scale; }
	DirectX::SimpleMath::Vector2 GetBaseScale() { return m_baseScale; }
	DirectX::SimpleMath::Vector2 GetPosition()	{ return m_position;}
	ANCHOR GetAnchor()							{ return m_anchor; }
	float GetRenderRatio()						{ return m_renderRatio; }

private:

	void CreateShader();
private:
	DX::DeviceResources* m_pDR;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	int m_windowWidth , m_windowHeight;
	int m_textureWidth, m_textureHeight;

	DirectX::SimpleMath::Vector2 m_scale;
	DirectX::SimpleMath::Vector2 m_baseScale;
	DirectX::SimpleMath::Vector2 m_position;
	DirectX::SimpleMath::Color m_color;

	ANCHOR m_anchor;

	// �`��̊���
	float m_renderRatio;
};


#endif // USERINTERFACE_DEFINED