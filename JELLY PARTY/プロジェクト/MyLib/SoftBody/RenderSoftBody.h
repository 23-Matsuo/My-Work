//--------------------------------------------------------------------------------------
// File: RenderSoftBody.cpp
//
// �\�t�g�{�f�B�̕`��N���X
//
// Last Update: 2023.10.20
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once

#ifndef RENDER_SOFTBODY_DEFINED
#define RENDER_SOFTBODY_DEFINED

#include <DeviceResources.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <vector>

class RenderSoftBody
{
public:

	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	lightPosition;
	};

	struct ShadowConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
	};
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

public:

	// �R���X�g���N�^
	RenderSoftBody();

	// �f�X�g���N�^
	~RenderSoftBody();

	// ������
	void Create(ID3D11ShaderResourceView* texture);

	// �X�V
	void Update(const std::vector<DirectX::SimpleMath::Vector3>& verticesPos);

	// �`��
	void Render();

	// �e�`��
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

	// ���_�o�b�t�@���쐬
	void CreateBuffer(const std::vector<DirectX::VertexPositionColorTexture> vertices);

	// �C���f�b�N�X�ԍ����v�Z����
	void CalculateIndex(const int& XNUM, const int& YNUM);

	// �N���[��
	void Clone(RenderSoftBody* other, const std::vector<DirectX::VertexPositionColorTexture> vertices)
	{
		other->SetIndices(m_indices);
		other->SetTexture(m_texture);
		other->SetInputLayout(m_inputLayout.Get());
		other->SetShaders(
			m_pixelShader.Get(), m_vertexShader.Get(),
			m_pixelShadowShader.Get(), m_vertexShadowShader.Get());
		other->CreateCloneBuffer(vertices);
	}

public: // �Z�b�^�[

	// �C���f�b�N�X��ݒ�
	void SetIndices(const std::vector<uint16_t>& indices) { m_indices = indices; }
	// �e�N�X�`����ݒ�
	void SetTexture(ID3D11ShaderResourceView* texture) { m_texture = texture; }
	// �F��ݒ�
	void SetColor(const DirectX::SimpleMath::Color& color);
	// �C���v�b�g���C�A�E�g��ݒ�
	void SetInputLayout(ID3D11InputLayout* inputLayout) { m_inputLayout = inputLayout; }
	// �V�F�[�_�[��ݒ�
	void SetShaders(
		ID3D11PixelShader* pixel, ID3D11VertexShader* vertex, 
		ID3D11PixelShader* pixelSM, ID3D11VertexShader* vertexSM
	);
	// �o�b�t�@��ݒ�
	void CreateCloneBuffer(const std::vector<DirectX::VertexPositionColorTexture> vertices);


public: // �Q�b�^�[

	// �C���f�b�N�X���擾
	const std::vector<uint16_t>& GetIndices() { return m_indices; }

private:

	// �`��ݒ�
	void RenderSetting(ID3D11DeviceContext1* context);

	// �e�̕`��ݒ�
	void RenderShadowSetting(ID3D11DeviceContext1* context);

	// �V�F�[�_�[�쐬
	void CreateShader();

private:
	DX::DeviceResources* m_pDR;

	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	// �萔�o�b�t�@(�V���h�E�}�b�v�p)
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_sCBuffer;

	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_VBuffer;

	// �C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_IBuffer;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// �e�N�X�`���n���h��
	ID3D11ShaderResourceView* m_texture;

	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;

	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	// ���_�V�F�[�_�i�V���h�E�}�b�v�p�j
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShadowShader;

	// �s�N�Z���V�F�[�_�i�V���h�E�}�b�v�p�j
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShadowShader;

	// ���_
	std::vector<DirectX::VertexPositionColorTexture> m_vertices;

	// �O�p�|���S���̃C���f�b�N�X
	std::vector<uint16_t> m_indices;
};

#endif	//RENDER_SOFTBODY_DEFINED
