//--------------------------------------------------------------------------------------
// File: UserInterface.cpp
//
// ���[�U�[�C���^�[�t�F�[�X�N���X
//
// Last Update: 2023.10.30
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "UserInterface.h"

#include "BinaryFile.h"
#include "Resources.h"
#include "Graphics.h"
using namespace DirectX;

//------------------------------------------
// �C���v�b�g���C�A�E�g
//------------------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC> UserInterface::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3)+ sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const DirectX::SimpleMath::Vector2 UserInterface::DEFAULT_SCALE = { 0.5f,0.5f };

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
UserInterface::UserInterface()
	: 
	m_pDR(nullptr)
	, m_textureHeight(0)
	, m_textureWidth(0)
	, m_texture(nullptr)
	, m_scale(SimpleMath::Vector2::One)
	, m_position(SimpleMath::Vector2::Zero)
	, m_color(DirectX::Colors::White)
	, m_anchor(ANCHOR::TOP_LEFT)
	, m_renderRatio(1.0f)
{
	RECT windowSize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_windowHeight = windowSize.bottom;
	m_windowWidth = windowSize.right;
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
UserInterface::~UserInterface()
{
}

//------------------------------------------
// �e�N�X�`���ǂݍ���
//------------------------------------------
void UserInterface::LoadTexture(const wchar_t* name)
{
	// �e�N�X�`���擾
	Resources::TextureTable texture = Resources::GetInstance()->GetTexture(name);
	// �e�N�X�`�����ݒ�
	m_texture = texture.texture;
	m_textureWidth = texture.textureWidth / 2;
	m_textureHeight = texture.textureHeight / 2;

}

//------------------------------------------
// �����֐�
//------------------------------------------
void UserInterface::Create(
	const wchar_t* name
	, DirectX::SimpleMath::Vector2 position
	, DirectX::SimpleMath::Vector2 scale
	, ANCHOR anchor)
{
	m_position = position;
	m_baseScale = DEFAULT_SCALE;
	m_scale = scale;
	m_anchor = anchor;

	//�V�F�[�_�[�̍쐬
	CreateShader();

	//�摜�̓ǂݍ���
	LoadTexture(name);
}

//------------------------------------------
// Shader�쐬
//------------------------------------------
void UserInterface::CreateShader()
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/UIVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/UIGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/UIPS.cso");

	//�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_vertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_geometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

//------------------------------------------
// �`��֐�
//------------------------------------------
void UserInterface::Render()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();
	RECT windowSize = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

		// ���_���
		// Position.xy	:�A���J�[���W(�s�N�Z���w��:1280 �~720)
		// Position.z	:�A���J�[�^�C�v(0�`8)�̐����Ŏw��
		// Color.xy�@	:�F
		// Tex.xy		:�E�B���h�E�T�C�Y�i�o�b�t�@�������B������͖��g�p�j

	VertexPositionColorTexture vertex[1] = {
		VertexPositionColorTexture(
		SimpleMath::Vector3(m_position.x, m_position.y, static_cast<float>(m_anchor))
		, m_color
		, SimpleMath::Vector2(m_renderRatio, 0))
	};

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.windowSize = SimpleMath::Vector4(static_cast<float>(windowSize.right), static_cast<float>(windowSize.bottom), 1, 1);
	cbuff.scale = m_scale;
	cbuff.textureSize = SimpleMath::Vector2(static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight));

	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//�������`��w��
	ID3D11BlendState* blendstate = states->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// �J�����O�͍�����
	context->RSSetState(states->CullNone());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	// �|���S����`��
	batch->Begin();
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}