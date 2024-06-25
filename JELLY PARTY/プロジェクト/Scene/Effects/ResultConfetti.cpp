//--------------------------------------------------------------------------------------
// File: ResultConfetti.cpp
//
// �p�[�e�B�N���N���X
//
// Last Update: 2023.10.02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "ResultConfetti.h"

#include "MyLib/BinaryFile.h"
#include <algorithm>
#include <random>

//-----------------------------------
// �C���v�b�g���C�A�E�g
//-----------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC> ResultConfetti::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

//-----------------------------------
// �R���X�g���N�^
//-----------------------------------
ResultConfetti::ResultConfetti()
	: m_pDR(nullptr)
	, m_timer(0.0f)
{
}

//-----------------------------------
// �f�X�g���N�^
//-----------------------------------
ResultConfetti::~ResultConfetti()
{
}

//-----------------------------------
// �����֐�
//-----------------------------------
void ResultConfetti::Create(DX::DeviceResources* pDR)
{	
	m_pDR = pDR;

	//�V�F�[�_�[�̍쐬
	CreateShader();
}

//-----------------------------------
// ������쐬
//-----------------------------------
void ResultConfetti::CreateConfetti(const float& elapsedTime)
{
	// 3.0�b���Ƃɐ���
	m_timer += elapsedTime;
	if (m_timer >= 0.1f)
	{
		std::random_device seed;
		std::default_random_engine engine(seed());
		std::uniform_real_distribution<float> positionDist(-5.0f, 5.0f);
		std::uniform_real_distribution<float> velocityDist(0.5f, 2.0f);
		std::uniform_real_distribution<float> rotateDist(-5.0f, 5.0f);
		std::uniform_real_distribution<float> scaleDist(0.05f, 0.2f);
		std::uniform_int_distribution<> colorDist(0, 6);

		// �����_�����x
		DirectX::SimpleMath::Vector3 velocity(0.0f, velocityDist(engine), 0.0f);
		// �����_����]
		DirectX::SimpleMath::Vector3 rotation(rotateDist(engine), rotateDist(engine), rotateDist(engine));
		// �����_���X�P�[��
		//DirectX::SimpleMath::Vector3 scale(scaleDist(engine), scaleDist(engine), scaleDist(engine));
		DirectX::SimpleMath::Vector3 scale(0.05f, 0.05f, 0.05f);
		

		float x = static_cast<float>(positionDist(engine));
		float z = static_cast<float>(positionDist(engine));

		// ���C���X�^���X�쐬
		ConfettiUtility confetti(
			10.0f,												// ��������
			DirectX::SimpleMath::Vector3(x, 8.0f, z + 5.0f),	// ����W
			velocity,											// ���x
			DirectX::SimpleMath::Vector3::Zero,					// �����x
			rotation,											// ��]
			scale,												// �X�P�[��			
			GetColor(colorDist(engine))							// �J���[
		);

		m_confettiUtility.push_back(confetti);
		m_timer = 0.0f;
	}
}

//-----------------------------------
// �X�V
//-----------------------------------
void ResultConfetti::Update(const float& elapsedTime)
{
	//timer��n����m_effect�̍X�V�������s��
	for (std::list<ConfettiUtility>::iterator ite = m_confettiUtility.begin(); ite != m_confettiUtility.end(); ite++)
	{
		// �q�N���X����false�ŏ���
		if (!(ite)->Update(elapsedTime))
		{
			ite = m_confettiUtility.erase(ite);
			if (ite == m_confettiUtility.end())
			{
				break;
			}
		}
	}
}


//-----------------------------------
// Shader�쐬
//-----------------------------------
void ResultConfetti::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ConfettiVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ConfettiGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ConfettiPS.cso");
	
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
// �`��
//------------------------------------------
void ResultConfetti::Render()
{
	auto context = m_pDR->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();
	DirectX::SimpleMath::Matrix view = Graphics::GetInstance()->GetViewMatrix();
	DirectX::SimpleMath::Matrix projection = Graphics::GetInstance()->GetProjectionMatrix();

	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();

	// ���_���(�|���S���̂S���_�̍��W���j
	m_confettiUtility.sort(
		[&](ConfettiUtility lhs, ConfettiUtility  rhs)
			{
				//�J�������ʂ̋����Ń\�[�g
				return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
			});
	//�o�^����Ă��钸�_�����Z�b�g
	m_vertices.clear();
	for (auto& cU : m_confettiUtility)
	{
		if (cameraDir.Dot(cU.GetPosition() - m_cameraPosition) < 0.0f) {
			//���ς��}�C�i�X�̏ꍇ�̓J�����̌��Ȃ̂ŕ\������K�v�Ȃ�
			continue;
		}

		// ��]
		DirectX::SimpleMath::Vector3 rotataion = cU.GetRotation();

		// �ʒu
		DirectX::VertexPositionColorTexture vPCT;
		vPCT.position = cU.GetPosition();

		// �F���
		DirectX::SimpleMath::Color color = cU.GetColor();

		// �A���t�@�l�͂���Ȃ��̂ŉ�]��x��n��
		color.w = rotataion.x;
	
		// �F�����Z�b�g
		vPCT.color = color;

		// UV���W�͂���Ȃ��̂ŉ�]�p��yz��n��
		vPCT.textureCoordinate = { rotataion.y, rotataion.z };
		m_vertices.push_back(vPCT);
	}
		
	//�\������_���Ȃ��ꍇ�͕`����I���
	if (m_vertices.empty())
	{
		return;
	}

	DirectX::SimpleMath::Matrix world;

	//world = m_rotation[0];

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = projection.Transpose();
	cbuff.matWorld = world.Transpose();
	cbuff.Diffuse = DirectX::SimpleMath::Vector4(1, 1, 1, 1);

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

	// �J�����O�͂Ȃ�
	context->RSSetState(states->CullNone());

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = Resources::GetInstance()->GetTexture(L"Smoke").texture;

	//�s�N�Z���V�F�[�_�Ƀe�N�X�`����o�^����B
	context->PSSetShaderResources(0, 1, texture.GetAddressOf());

	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	// �|���S����`��
	batch->Begin();
	for (int i = 0; i < m_vertices.size(); i++)
	{
		batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[i], 1);
	}
	batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

//-----------------------------------
// �r���{�[�h�쐬�֐�
//-----------------------------------
void ResultConfetti::CreateBillboard(
	DirectX::SimpleMath::Vector3 up
)
{
	DirectX::SimpleMath::Vector3 target = Graphics::GetInstance()->GetCameraTarget();
	DirectX::SimpleMath::Vector3 eye = Graphics::GetInstance()->GetCameraEye();

	m_billboard =
		//SimpleMath::Matrix::CreateBillboard(target, eye , up);
		DirectX::SimpleMath::Matrix::CreateBillboard(DirectX::SimpleMath::Vector3::Zero,eye - target, up);

	DirectX::SimpleMath::Matrix rot = DirectX::SimpleMath::Matrix::Identity;
	rot._11 = -1;
	rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;
	m_billboard = rot * m_billboard;
}


//-----------------------------------
// �F���擾
//-----------------------------------
const DirectX::SimpleMath::Color ResultConfetti::GetColor(int rnd)
{
	DirectX::SimpleMath::Color color;

	switch (rnd)
	{
	case 0:	// ��
		color = DirectX::Colors::Red;
		break;
	case 1:	// ��
		color = DirectX::Colors::Green;
		break;
	case 2:	// ��
		color = DirectX::Colors::Blue;
		break;
	case 3:	// ���F
		color = DirectX::Colors::Yellow;
		break;
	case 4:	// ��
		color = DirectX::Colors::Purple;
		break;
	case 5:	// �I�����W
		color = DirectX::Colors::Orange;
		break;
	case 6:	// �V�A��
		color = DirectX::Colors::Cyan;
		break;
	default:
		break;
	}

	return color;
}
