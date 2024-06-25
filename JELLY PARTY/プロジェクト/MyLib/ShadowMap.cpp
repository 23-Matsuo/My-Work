//--------------------------------------------------------------------------------------
// File: ShadowMap.cpp
//
// �g�����W�V�����N���X
//
// Last Update: 2023/10/12
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "ShadowMap.h"
#include "ReadData.h"
#include "BinaryFile.h"
#include <iomanip>

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
ShadowMap::ShadowMap()
	:
	m_pDR(nullptr)
{
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
ShadowMap::~ShadowMap()
{

}

//------------------------------------------
// ������
//------------------------------------------
void ShadowMap::Initialize()
{
	m_pDR = Graphics::GetInstance()->GetDeviceResources();

	auto device = m_pDR->GetD3DDevice();

	RECT rect = { 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE };

	// �����_�[�e�N�X�`���̍쐬�i�V���h�E�}�b�v�p�j
	m_shadowMapRT = std::make_unique<DX::RenderTexture>(DXGI_FORMAT_R32_FLOAT);
	m_shadowMapRT->SetDevice(device);
	m_shadowMapRT->SetWindow(rect);

	// �f�v�X�X�e���V���̍쐬�i�V���h�E�}�b�v�p�j
	m_shadowMapDS = std::make_unique<DepthStencil>(DXGI_FORMAT_D32_FLOAT);
	m_shadowMapDS->SetDevice(device);
	m_shadowMapDS->SetWindow(rect);

	BinaryFile vs_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_VS_Depth.cso");
	BinaryFile ps_depth = BinaryFile::LoadFile(L"Resources/Shaders/SM_PS_Depth.cso");
	BinaryFile VS = BinaryFile::LoadFile(L"Resources/Shaders/SM_VS.cso");
	BinaryFile PS = BinaryFile::LoadFile(L"Resources/Shaders/SM_PS.cso");

	// ���_�V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
	if (FAILED(device->CreateVertexShader(vs_depth.GetData(), vs_depth.GetSize(), NULL, m_VS_Depth.ReleaseAndGetAddressOf())))
	{
		// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
	if (FAILED(device->CreatePixelShader(ps_depth.GetData(), ps_depth.GetSize(), NULL, m_PS_Depth.ReleaseAndGetAddressOf())))
	{
		// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
	// ���_�V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
	if (FAILED(device->CreateVertexShader(VS.GetData(), VS.GetSize(), NULL, m_VS.ReleaseAndGetAddressOf())))
	{
		// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�[�̍쐬�i�V���h�E�}�b�v�p�j
	if (FAILED(device->CreatePixelShader(PS.GetData(), PS.GetSize(), NULL, m_PS.ReleaseAndGetAddressOf())))
	{
		// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// �萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(ConstantBuffer));	// �m�ۂ���T�C�Y�i16�̔{���Őݒ肷��j
	// GPU (�ǂݎ���p) �� CPU (�������ݐ�p) �̗�������A�N�Z�X�ł��郊�\�[�X
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �萔�o�b�t�@�Ƃ��Ĉ���
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPU�����e��ύX�ł���悤�ɂ���
	DX::ThrowIfFailed(device->CreateBuffer(&bufferDesc, nullptr, m_constantBuffer.ReleaseAndGetAddressOf()));

	// �T���v���[�̍쐬
	D3D11_SAMPLER_DESC sampler_desc = CD3D11_SAMPLER_DESC(D3D11_DEFAULT);
	sampler_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_LESS;
	device->CreateSamplerState(&sampler_desc, m_shadowMapSampler.ReleaseAndGetAddressOf());

	// ���C�g�̍쐬
	m_light = std::make_unique<Light>();

	// ���C�g�̈ʒu
	m_lightPosition = m_light->GetPosition();

	// ���C�g�̉�]
	m_lightRotate = m_light->GetRotation();
}

//------------------------------------------
// �X�V
//------------------------------------------
void ShadowMap::Update()
{
}

//------------------------------------------
// �`��
//------------------------------------------
void ShadowMap::Render(
	CatSphere* pSphere,
	StageFloorManager* pStageFloorManager,
	StageSoftBodyManager* pSoftBodyManager
)
{
	auto context = m_pDR->GetD3DDeviceContext();

	auto states = Graphics::GetInstance()->GetCommonStates();

	// -------------------------------------------------------------------------- //
	// Pass1 �V���h�E�}�b�v�̍쐬
	// -------------------------------------------------------------------------- //

	auto rtv = m_shadowMapRT->GetRenderTargetView();
	auto srv = m_shadowMapRT->GetShaderResourceView();
	auto dsv = m_shadowMapDS->GetDepthStencilView();

	// �����_�[�^�[�Q�b�g��ύX�ishadowMapRT�j
	context->ClearRenderTargetView(rtv, DirectX::SimpleMath::Color(1.0f, 0.0f, 1.0f, 1.0f));
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->OMSetRenderTargets(1, &rtv, dsv);

	// �r���[�|�[�g��ݒ�
	D3D11_VIEWPORT vp = { 0.0f, 0.0f, SHADOWMAP_SIZE, SHADOWMAP_SIZE, 0.0f, 1.0f };
	context->RSSetViewports(1, &vp);

	m_view = Graphics::GetInstance()->GetViewMatrix();
	m_projection = Graphics::GetInstance()->GetProjectionMatrix();

	// ------------------------------------------------ //
	// ���C�g��Ԃ̃r���[�s��Ǝˉe�s����쐬����
	// ------------------------------------------------ //

	// ���C�g�̃r���[�s����쐬
	DirectX::SimpleMath::Matrix lightView = m_light->GetView();

	// ���C�g�̎ˉe�s����쐬
	DirectX::SimpleMath::Matrix lightProjection = m_light->GetProjection();

	// -------------------------------------------------------------------------- //
	// �萔�o�b�t�@���X�V
	// -------------------------------------------------------------------------- //

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// GPU���萔�o�b�t�@�ɑ΂��ăA�N�Z�X���s��Ȃ��悤�ɂ���
	context->Map(m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// �萔�o�b�t�@���X�V
	ConstantBuffer cb = {};
	DirectX::SimpleMath::Matrix lightViewProjection = lightView * lightProjection;
	cb.lightViewProj = DirectX::XMMatrixTranspose(lightViewProjection);
	cb.lightPosition = m_lightPosition;
	cb.lightAmbient = m_light->GetAmbient();

	*static_cast<ConstantBuffer*>(mappedResource.pData) = cb;

	// GPU���萔�o�b�t�@�ɑ΂��ẴA�N�Z�X��������
	context->Unmap(m_constantBuffer.Get(), 0);

	// ------------------------------------------------ //
	// �e�ɂȂ郂�f����`�悷��
	// ------------------------------------------------ //

	// �[���[�̉e�`��
	pSoftBodyManager->ShadowRender(lightView, lightProjection);

	// �v���C���[�̕`��
	pSphere->Render(context, states, lightView, lightProjection, [&]()
		{
			context->VSSetShader(m_VS_Depth.Get(), nullptr, 0);
			context->PSSetShader(m_PS_Depth.Get(), nullptr, 0);
		}
	);

	// -------------------------------------------------------------------------- //
	// �����_�[�^�[�Q�b�g�ƃr���[�|�[�g�����ɖ߂�
	// -------------------------------------------------------------------------- //
	auto renderTarget = m_pDR->GetRenderTargetView();
	auto depthStencil = m_pDR->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);
	auto const viewport = m_pDR->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// �X�e�[�W���̕`��
	pStageFloorManager->Render(context, states, m_view, m_projection, [&]()
		{
			// �萔�o�b�t�@�̐ݒ�
			ID3D11Buffer* cbuf[] = { m_constantBuffer.Get() };
			context->VSSetConstantBuffers(1, 1, cbuf);
			context->PSSetConstantBuffers(1, 1, cbuf);

			// �쐬�����V���h�E�}�b�v�����\�[�X�Ƃ��Đݒ�
			context->PSSetShaderResources(1, 1, &srv);

			// �e�N�X�`���T���v���[�̐ݒ�
			ID3D11SamplerState* samplers[] = { states->PointWrap(), m_shadowMapSampler.Get() };
			context->PSSetSamplers(0, 2, samplers);

			// �V�F�[�_�[�̐ݒ�
			context->VSSetShader(m_VS.Get(), nullptr, 0);
			context->PSSetShader(m_PS.Get(), nullptr, 0);
		}
	);

	// �v���C���[�̕`��
	pSphere->Render(context, states, m_view, m_projection, [&]()
		{
		}
	);

	// �[���[�`��
	pSoftBodyManager->Render();

	// ���\�[�X�̊��蓖�Ă���������ishadowMapRT�j
	ID3D11ShaderResourceView* nullsrv[] = { nullptr };
	context->PSSetShaderResources(1, 1, nullsrv);

	// ���̃G�t�F�N�g��`��
	pSphere->RenderEffect();
}