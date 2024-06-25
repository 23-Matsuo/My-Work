//--------------------------------------------------------------------------------------
// File: Transition.cpp
//
// �g�����W�V�����N���X
//
// Last Update: 2023/10/12
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Transition.h"
#include "Resources.h"
#include "Graphics.h"
#include "MyLib/BinaryFile.h"

//------------------------------------------
// �C���v�b�g���C�A�E�g
//------------------------------------------
const std::vector<D3D11_INPUT_ELEMENT_DESC> Transition::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

const float Transition::TRANS_SPEED = 2.0f;

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Transition::Transition()
	:
	m_vertex{}
	, m_state(STATE::SLIDEOUT)
	, m_slideRatio(DirectX::SimpleMath::Vector4::One)
{
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
Transition::~Transition()
{

}

//------------------------------------------
// ������
//------------------------------------------
void Transition::Initialize()
{
	auto device = Graphics::GetInstance()->GetDeviceResources()->GetD3DDevice();
	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/TransitionVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/TransitionGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/TransitionPS.cso");
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
// �X�V
//------------------------------------------
void Transition::Update()
{
	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());
	
	m_slideRatio.Clamp(DirectX::SimpleMath::Vector4::Zero, DirectX::SimpleMath::Vector4::One);


	// ��Ԃɉ����čX�V
	switch (m_state)
	{
	case STATE::IDLE:
		break;
	case STATE::SLIDEIN:
		SlideIn(TRANS_SPEED, elapsedTime);
		break;
	case STATE::SLIDEOUT:
		SlideOut(TRANS_SPEED, elapsedTime);
		break;
	}
}

//------------------------------------------
// �`��
//------------------------------------------
void Transition::Render()
{
	auto context = Graphics::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = Graphics::GetInstance()->GetCommonStates();
	auto batch = Graphics::GetInstance()->GetPrimitiveBatchVPCT();
	//auto spbatch = Resources::GetInstance()->GetSpriteBatch();

	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_vertex =
	{
		DirectX::SimpleMath::Vector3::One,
		DirectX::SimpleMath::Vector4::One,
		DirectX::SimpleMath::Vector2::One
	};


	//�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬
	ConstBuffer cbuff;
	cbuff.slidePos = m_slideRatio;

	//�������`��w��
	ID3D11BlendState* blendstate = states->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �[�x�o�b�t�@�ɏ������ݎQ�Ƃ���
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// �J�����O�͍�����
	context->RSSetState(states->CullNone());

	//�󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	//�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//�V�F�[�_���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	// �|���S����`��
	batch->Begin();
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertex, 1);
	batch->End();

	//�V�F�[�_�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void Transition::Restart()
{
	m_slideRatio = DirectX::SimpleMath::Vector4::One;
	m_state = STATE::SLIDEOUT;
}

//------------------------------------------
// �X���C�h�C��
//------------------------------------------
void Transition::SlideIn(const float time, float elapsedTime)
{
	m_slideRatio.y += time * elapsedTime;
	// �o�ߎ��Ԃ��ړ��ɂ����鎞�Ԃ𒴂��Ă��邩
	if (m_slideRatio.y >= 1.0f)
	{
		// �X�e�[�g��҂���Ԃ�
		m_state = STATE::IDLE;
	}
}

//------------------------------------------
// �X���C�h�A�E�g
//------------------------------------------
void Transition::SlideOut(const float time, float elapsedTime)
{
	m_slideRatio.y -= time * elapsedTime;

	// �o�ߎ��Ԃ��ړ��ɂ����鎞�Ԃ𒴂��Ă��邩
	if (m_slideRatio.y <= 0.0f)
	{
		// �X�e�[�g��҂����
		m_state = STATE::IDLE;
	}
}

