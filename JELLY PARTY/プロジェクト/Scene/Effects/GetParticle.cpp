//--------------------------------------------------------------------------------------
// File: GetParticle.cpp
//
// �p�[�e�B�N���N���X
//
// Last Update: 2023.10.02
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "GetParticle.h"

#include "MyLib/BinaryFile.h"
#include <algorithm>
#include <random>

//-----------------------------------
// �C���v�b�g���C�A�E�g
//-----------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC> GetParticle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

//-----------------------------------
// �R���X�g���N�^
//-----------------------------------
GetParticle::GetParticle()
	: m_pDR(nullptr)
	, m_state(State::IDLE)
{
}

//-----------------------------------
// �f�X�g���N�^
//-----------------------------------
GetParticle::~GetParticle()
{
}

//-----------------------------------
// �����֐�
//-----------------------------------
void GetParticle::Create(DX::DeviceResources* pDR)
{	
	m_pDR = pDR;

	//�V�F�[�_�[�̍쐬
	CreateShader();
}

//-----------------------------------
// �p�[�e�B�N���쐬
//-----------------------------------
void GetParticle::CreateGetParticle(
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Color color
)
{
	const int PARTICLE_NUM = 8;
	const float RADIUS = 3.5f;


	for (int i = 0; i < PARTICLE_NUM; i++)
	{
		float angle = i * (360.0f / PARTICLE_NUM);
		float x = RADIUS * std::cos(angle * DirectX::XM_PI / 180.0f);
		float z = RADIUS * std::sin(angle * DirectX::XM_PI / 180.0f);
		ParticleUtility pU(
			0.3f,															// ��������
			position,														// ����W
			DirectX::SimpleMath::Vector3(x * 2, 0.0f, z * 2),				// ���x
			DirectX::SimpleMath::Vector3(-x * 2, -10.0f, -z * 2),			// �����x
			DirectX::SimpleMath::Vector3(0.5f, 0.5f, 1.0f),					// �����X�P�[��
			DirectX::SimpleMath::Vector3(0.1f, 0.1f, 1.0f),					// �ŏI�X�P�[��
			DirectX::SimpleMath::Color(color.x, color.y, color.z, 0.8f),	// �����J���[
			DirectX::SimpleMath::Color(color.x, color.y, color.z, 0.0f)		// �ŏI�J���[
		);

		m_particleUtility.push_front(pU);
	}
}

//-----------------------------------
// �X�V
//-----------------------------------
void GetParticle::Update(const DX::StepTimer& timer)
{
	float elapsedTime = static_cast<float>(timer.GetElapsedSeconds());
	if (m_state != State::PLAY) return;
	//timer��n����m_effect�̍X�V�������s��
	for (std::list<ParticleUtility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)
	{
		// �q�N���X����false�ŏ���
		if (!(ite)->Update(elapsedTime))
		{
			m_particleUtility.clear();
			m_state = State::IDLE;
			break;
		}
	}
}


//-----------------------------------
// Shader�쐬
//-----------------------------------
void GetParticle::CreateShader()
{
	auto device = m_pDR->GetD3DDevice();

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");
	
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
void GetParticle::Render()
{
	if (m_state != State::PLAY) return;

	auto context = m_pDR->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();
	DirectX::SimpleMath::Matrix view = Graphics::GetInstance()->GetViewMatrix();
	DirectX::SimpleMath::Matrix projection = Graphics::GetInstance()->GetProjectionMatrix();

	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();

	// ���_���(�|���S���̂S���_�̍��W���j
	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
			{
				//�J�������ʂ̋����Ń\�[�g
				return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) >
					cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
			});
	//�o�^����Ă��钸�_�����Z�b�g
	m_vertices.clear();
	for (auto& pU : m_particleUtility)
	{
		if (cameraDir.Dot(pU.GetPosition() - m_cameraPosition) < 0.0f) {
			//���ς��}�C�i�X�̏ꍇ�̓J�����̌��Ȃ̂ŕ\������K�v�Ȃ�
			continue;
		}
		DirectX::VertexPositionColorTexture vPCT;
		vPCT.position = DirectX::XMFLOAT3(pU.GetPosition());
		vPCT.color = pU.GetNowColor();
		vPCT.textureCoordinate = DirectX::XMFLOAT2(pU.GetNowScale().x, pU.GetNowScale().y );
			
		m_vertices.push_back(vPCT);
	}
		
	//�\������_���Ȃ��ꍇ�͕`����I���
	if (m_vertices.empty())
	{
		return;
	}

	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	ConstBuffer cbuff;
	cbuff.matView = view.Transpose();
	cbuff.matProj = projection.Transpose();
	cbuff.matWorld = m_billboard.Transpose();
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

	// �J�����O�͍�����
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
 	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());
	batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

//-----------------------------------
/// �r���{�[�h�쐬�֐�
//-----------------------------------
void GetParticle::CreateBillboard(
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

