//--------------------------------------------------------------------------------------
// File: RenderSoftBody.cpp
//
// �\�t�g�{�f�B�̕`��N���X
//
// Last Update: 2023.10.20
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "RenderSoftBody.h"
#include "../BinaryFile.h"

//------------------------------------------
// �C���v�b�g���C�A�E�g
//------------------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC> RenderSoftBody::INPUT_LAYOUT =
{
	// �Z�}���e�B�b�N��, �ԍ�, �t�H�[�}�b�g, �X���b�g, �I�t�Z�b�g�o�C�g�l, �X���b�g�N���X, �C���X�^���X�̐�
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
RenderSoftBody::RenderSoftBody()
	:
	m_pDR(nullptr)
	, m_texture(nullptr)
	, m_IBuffer(nullptr)
	, m_indices{}
	, m_vertices{}
{
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
RenderSoftBody::~RenderSoftBody()
{
	m_pDR = nullptr;
	m_texture = nullptr;
	m_IBuffer = nullptr;
	m_indices.clear();
	m_vertices.clear();
}

//------------------------------------------
// ������
//------------------------------------------
void RenderSoftBody::Create(ID3D11ShaderResourceView* texture)
{
	m_pDR = Graphics::GetInstance()->GetDeviceResources();

	CreateShader();

	m_texture = texture;
}

//------------------------------------------
// �X�V
//------------------------------------------
void RenderSoftBody::Update(const std::vector<DirectX::SimpleMath::Vector3>& verticesPos)
{
	for (int i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].position = verticesPos[i];
	}
}

//------------------------------------------
// ��1����`��
//------------------------------------------
void RenderSoftBody::Render()
{
	auto context = m_pDR->GetD3DDeviceContext();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();
	
	DirectX::SimpleMath::Matrix world;
	DirectX::SimpleMath::Matrix view = Graphics::GetInstance()->GetViewMatrix();
	DirectX::SimpleMath::Matrix projection = Graphics::GetInstance()->GetProjectionMatrix();

	//�V�F�[�_�[�ɓn���o�b�t�@���쐬
	ConstBuffer cbuff;
	cbuff.matWorld = world.Transpose();
	cbuff.matView = view.Transpose();
	cbuff.matProj = projection.Transpose();
	cbuff.lightPosition = DirectX::SimpleMath::Vector4(0.0f, 1.0f, 0.0f, 0.0f);

	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, nullptr, &cbuff, 0, 0);

	RenderSetting(context);

	batch->Begin();
	
	batch->DrawIndexed(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		m_indices.data(), m_indices.size(),			// �C���f�b�N�X���
		m_vertices.data(), m_vertices.size()		// ���_���
	);
	
	batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}


//------------------------------------------
// �`��̐ݒ�
//------------------------------------------
void RenderSoftBody::RenderSetting(ID3D11DeviceContext1* context)
{
	auto states = Graphics::GetInstance()->GetCommonStates();

	//---------------------------
	// �萔�o�b�t�@��n��
	//---------------------------
	ID3D11Buffer* cb[] = { m_sCBuffer.Get(), m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 2, cb);
	context->PSSetConstantBuffers(0, 2, cb);

	//---------------------------
	// ���_�o�b�t�@��n��
	//---------------------------
	UINT stride = sizeof(DirectX::VertexPositionColorTexture);
	UINT offset = 0;
	ID3D11Buffer* vb[1] = { m_VBuffer.Get() };
	context->IASetVertexBuffers(0, 1, vb, &stride, &offset);

	//---------------------------
	// �C���f�b�N�X�o�b�t�@��n��
	//---------------------------
	context->IASetIndexBuffer(m_IBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//�������`��w��
	ID3D11BlendState* blendstate = states->AlphaBlend();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɓǂݎ����Q�Ƃ���
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// �J�����O�͍�����
	// �ɂ��������ǕύX�Ɏ��Ԃ��|���邩��J�����O�Ȃ�
	context->RSSetState(states->CullNone());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, &m_texture);

}

//------------------------------------------
// ��1���̉e�`��
//------------------------------------------
void RenderSoftBody::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	auto context = m_pDR->GetD3DDeviceContext();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();

	DirectX::SimpleMath::Matrix world;
	//�V�F�[�_�[�ɓn���o�b�t�@���쐬
	ShadowConstBuffer scbuff;
	scbuff.matWorld = world.Transpose();
	scbuff.matView = lightView.Transpose();
	scbuff.matProj = lightProjection.Transpose();

	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_sCBuffer.Get(), 0, nullptr, &scbuff, 0, 0);

	RenderShadowSetting(context);

	batch->Begin();

	batch->DrawIndexed(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		m_indices.data(), m_indices.size(),			// �C���f�b�N�X���
		m_vertices.data(), m_vertices.size()		// ���_���
	);

	batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

//------------------------------------------
// �e�`��̐ݒ�
//------------------------------------------
void RenderSoftBody::RenderShadowSetting(ID3D11DeviceContext1* context)
{
	auto states = Graphics::GetInstance()->GetCommonStates();

	//---------------------------
	// �萔�o�b�t�@��n��
	//---------------------------
	ID3D11Buffer* cb[1] = { m_sCBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//---------------------------
	// ���_�o�b�t�@��n��
	//---------------------------
	UINT stride = sizeof(DirectX::VertexPositionColorTexture);
	UINT offset = 0;
	ID3D11Buffer* vb[1] = { m_VBuffer.Get() };
	context->IASetVertexBuffers(0, 1, vb, &stride, &offset);

	//---------------------------
	// �C���f�b�N�X�o�b�t�@��n��
	//---------------------------
	context->IASetIndexBuffer(m_IBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//�摜�p�T���v���[�̓o�^
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//�������`��w��
	ID3D11BlendState* blendstate = states->AlphaBlend();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɓǂݎ����Q�Ƃ���
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// �J�����O�͍�����
	// �ɂ��������ǕύX�Ɏ��Ԃ��|���邩��J�����O�Ȃ�
	context->RSSetState(states->CullNone());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShadowShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShadowShader.Get(), nullptr, 0);

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, &m_texture);
}


//------------------------------------------
// Shader�쐬�����������������֐�
//------------------------------------------
void RenderSoftBody::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/JellyVS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/JellyPS.cso");
	BinaryFile SM_VSData = BinaryFile::LoadFile(L"Resources/Shaders/JellySM_VS.cso");
	BinaryFile SM_PSData = BinaryFile::LoadFile(L"Resources/Shaders/JellySM_PS.cso");

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

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_pixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(SM_VSData.GetData(), SM_VSData.GetSize(), NULL, m_vertexShadowShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(SM_PSData.GetData(), SM_PSData.GetSize(), NULL, m_pixelShadowShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.ByteWidth = sizeof(ConstBuffer);					// �o�b�t�@�̃T�C�Y(���_��)
	cbd.Usage = D3D11_USAGE_DEFAULT;						// �g�p���@
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;				// �o�b�t�@�̎��
	cbd.CPUAccessFlags = 0;									// CPU�A�N�Z�X�ݒ�
	device->CreateBuffer(&cbd, nullptr, &m_CBuffer);

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@�����i�V���h�E�}�b�v�p�j
	D3D11_BUFFER_DESC scbd;
	ZeroMemory(&scbd, sizeof(scbd));
	scbd.ByteWidth = sizeof(ShadowConstBuffer);					// �o�b�t�@�̃T�C�Y(���_��)
	scbd.Usage = D3D11_USAGE_DEFAULT;						// �g�p���@
	scbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;				// �o�b�t�@�̎��
	scbd.CPUAccessFlags = 0;									// CPU�A�N�Z�X�ݒ�
	device->CreateBuffer(&scbd, nullptr, &m_sCBuffer);
}

//------------------------------------------
// �o�b�t�@���쐬
//------------------------------------------
void RenderSoftBody::CreateBuffer(const std::vector<DirectX::VertexPositionColorTexture> vertices)
{
	auto device = m_pDR->GetD3DDevice();

	// ���_�o�b�t�@���쐬
	D3D11_BUFFER_DESC vbd = {};
	// �o�b�t�@�T�C�Y���̃����������ׂď�����
	//ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = static_cast<UINT>(sizeof(DirectX::VertexPositionColorTexture) * vertices.size());
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// �����f�[�^��ݒ�
	D3D11_SUBRESOURCE_DATA vbdInitData = {};
	vbdInitData.pSysMem = vertices.data();
	// �G���[�n���h�����O
	if (FAILED(device->CreateBuffer(&vbd, &vbdInitData, &m_VBuffer)))
	{
		MessageBox(0, L"CreatVertexBuffer failed.", NULL, MB_OK);
	}
	// ���_�����R�s�[
	m_vertices = vertices;


	// �C���f�b�N�X�o�b�t�@���쐬
	D3D11_BUFFER_DESC ibd = {};
	//ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = static_cast<UINT>(sizeof(uint16_t) * m_indices.size());
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// �����f�[�^��ݒ�
	D3D11_SUBRESOURCE_DATA ibdInitData = {};
	ibdInitData.pSysMem = m_indices.data();
	// �G���[�n���h�����O
	if (FAILED(device->CreateBuffer(&ibd, &ibdInitData, &m_IBuffer)))
	{
		MessageBox(0, L"CreatIndexBuffer failed.", NULL, MB_OK);
	}
}

//------------------------------------------
// �C���f�b�N�X���v�Z
//------------------------------------------
void RenderSoftBody::CalculateIndex(const int& XNUM, const int& YNUM)
{
	int total = XNUM * YNUM;
	// ���݂̍s��
	int row = 1;

	for (int i = 0; i < total - XNUM - 1; ++i)
	{
		// �l�p�`���\�����钸�_�C���f�b�N�X
		uint16_t p[4];

		// i���[�ɂȂ�����
		// ���̍s�ɍX�V
		if (i == (XNUM * row) - 1)
		{
			i++; row++;
		}
		
		// ���_�ʒu���v�Z
		p[0] = static_cast<uint16_t>(i);			p[1] = static_cast<uint16_t>(i + 1);
		p[2] = static_cast<uint16_t>(i + XNUM);		p[3] = static_cast<uint16_t>(i + XNUM + 1);

		// 0->1->2, 1->2->3�̏�
		m_indices.push_back(p[0]);
		m_indices.push_back(p[1]);
		m_indices.push_back(p[2]);

		m_indices.push_back(p[1]);
		m_indices.push_back(p[2]);
		m_indices.push_back(p[3]);
	}
}

//------------------------------------------
// �F��ݒ�
//------------------------------------------
void RenderSoftBody::SetColor(const DirectX::SimpleMath::Color& color)
{
	for (int i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].color = color;
	}
}

//------------------------------------------
// �V�F�[�_�[��ݒ�
//------------------------------------------
void RenderSoftBody::SetShaders(
	ID3D11PixelShader* pixel, ID3D11VertexShader* vertex, 
	ID3D11PixelShader* pixelSM, ID3D11VertexShader* vertexSM
)
{
	m_pixelShader = pixel;
	m_vertexShader = vertex;
	m_pixelShadowShader = pixelSM;
	m_vertexShadowShader = vertexSM;
}

//------------------------------------------
// �o�b�t�@�𕡐�
//------------------------------------------
void RenderSoftBody::CreateCloneBuffer(const std::vector<DirectX::VertexPositionColorTexture> vertices)
{
	m_pDR = Graphics::GetInstance()->GetDeviceResources();
	auto device = m_pDR->GetD3DDevice();

	// ���_�o�b�t�@���쐬
	D3D11_BUFFER_DESC vbd = {};
	// �o�b�t�@�T�C�Y���̃����������ׂď�����
	//ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = static_cast<UINT>(sizeof(DirectX::VertexPositionColorTexture) * vertices.size());
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// �����f�[�^��ݒ�
	D3D11_SUBRESOURCE_DATA vbdInitData = {};
	vbdInitData.pSysMem = vertices.data();
	// �G���[�n���h�����O
	if (FAILED(device->CreateBuffer(&vbd, &vbdInitData, &m_VBuffer)))
	{
		MessageBox(0, L"CreatVertexBuffer failed.", NULL, MB_OK);
	}
	// ���_�����R�s�[
	m_vertices = vertices;

	// �C���f�b�N�X�o�b�t�@���쐬
	D3D11_BUFFER_DESC ibd = {};
	//ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = static_cast<UINT>(sizeof(uint16_t) * m_indices.size());
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// �����f�[�^��ݒ�
	D3D11_SUBRESOURCE_DATA ibdInitData = {};
	ibdInitData.pSysMem = m_indices.data();
	// �G���[�n���h�����O
	if (FAILED(device->CreateBuffer(&ibd, &ibdInitData, &m_IBuffer)))
	{
		MessageBox(0, L"CreatIndexBuffer failed.", NULL, MB_OK);
	}

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.ByteWidth = sizeof(ConstBuffer);					// �o�b�t�@�̃T�C�Y(���_��)
	cbd.Usage = D3D11_USAGE_DEFAULT;						// �g�p���@
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;				// �o�b�t�@�̎��
	cbd.CPUAccessFlags = 0;									// CPU�A�N�Z�X�ݒ�
	device->CreateBuffer(&cbd, nullptr, &m_CBuffer);

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC scbd;
	ZeroMemory(&scbd, sizeof(scbd));
	scbd.ByteWidth = sizeof(ShadowConstBuffer);					// �o�b�t�@�̃T�C�Y(���_��)
	scbd.Usage = D3D11_USAGE_DEFAULT;							// �g�p���@
	scbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;				// �o�b�t�@�̎��
	scbd.CPUAccessFlags = 0;									// CPU�A�N�Z�X�ݒ�
	device->CreateBuffer(&scbd, nullptr, &m_sCBuffer);

}
