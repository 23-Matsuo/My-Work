//--------------------------------------------------------------------------------------
// File: CenterSoftBody.h
//
// ��]���钆�S�̃I�u�W�F�N�g
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "CenterSoftBody.h"

#include "MyLib/ObjectCollision.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
CenterSoftBody::CenterSoftBody()
{
	auto deviceResources = Graphics::GetInstance()->GetDeviceResources();
	auto device = deviceResources->GetD3DDevice();
	auto context = deviceResources->GetD3DDeviceContext();
	// �Փ˔���̕\���I�u�W�F�N�g�̍쐬
	m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);

	// �e�N�X�`����ݒ�
	m_texture = Resources::GetInstance()->GetTexture(L"Jelly").texture;

	// �p�����[�^��ǂݍ���
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/JellyCenterParameter");

	// ���_�̐��f�[�^
	for (const auto& num : m_parameters["POINT_NUM"])
	{
		m_pointXNum = num[0];
		m_pointYNum = num[1];
	}
	// �ʒu�f�[�^
	DirectX::SimpleMath::Vector3 initPos;
	for (const auto& position : m_parameters["INITIAL_POSITION"])
	{
		initPos = { position[0], position[1], position[2] };
	}
	// ���a
	const float RADIUS = static_cast<float>(m_parameters["RADIUS"]);
	// ����
	const float HEIGHT = static_cast<float>(m_parameters["HEIGHT"]);

	// �V�����_�[���쐬
	m_cylinder = SoftBodyFactory::CreateCylinder(
		m_pointXNum, m_pointYNum,
		RADIUS,
		HEIGHT,
		initPos,
		{},
		DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
		DirectX::SimpleMath::Color(0.9f, 0.25f, 0.25f, 1.0f),
		m_texture.Get()
	);

	m_collision[0] = DirectX::BoundingSphere(DirectX::SimpleMath::Vector3(0.0f, HEIGHT - 0.5f, 0.0f), RADIUS - 0.2f);
	m_collision[1] = DirectX::BoundingSphere(DirectX::SimpleMath::Vector3(0.0f, HEIGHT + 0.8f, 0.0f), RADIUS - 0.2f);

	// �W���C���g�쐬
	CreateJoint();

	m_hitParticle = std::make_unique<HitParticle>();
	m_hitParticle->Create(deviceResources);
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
CenterSoftBody::~CenterSoftBody()
{
	m_cylinder.reset();
}

//------------------------------------------
// �X�V
//------------------------------------------
void CenterSoftBody::Update(DirectX::SimpleMath::Matrix rotation)
{
	auto timer = Graphics::GetInstance()->GetStepTimer();

	// ��
	m_cylinder->Rotate(rotation);

	// �\�t�g�{�f�B�̍X�V
	m_cylinder->Update(*timer);

	for (int i = 0; i < 2; i++)
	{
		DirectX::SimpleMath::Vector3 hitPosition;
		if (ObjectCollision::GetInstance()->CatToShaft(m_collision[i], m_cylinder.get(), &hitPosition))
		{
			// �����������̃G�t�F�N�g����
			m_hitParticle->CreateHitParticle(hitPosition, m_cylinder.get()->GetColor());
			m_hitParticle->SetState(HitParticle::State::PLAY);
			break;
		}
	}
	// �G�t�F�N�g�X�V
	m_hitParticle->Update(*timer);
}

//------------------------------------------
// �`��
//------------------------------------------
void CenterSoftBody::Render()
{
	// �\�t�g�{�f�B�e�N�X�`���`��
	m_cylinder->Render();

	// �p�[�e�B�N�����r���{�[�h�����ĕ`��
	m_hitParticle->CreateBillboard(DirectX::SimpleMath::Vector3::Up);
	m_hitParticle->Render();
}

//------------------------------------------
// �e�`��
//------------------------------------------
void CenterSoftBody::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection)
{
	// �\�t�g�{�f�B�e�`��
	m_cylinder->ShadowRender(lightView, lightProjection);

}

//------------------------------------------
// �W���C���g�쐬
//------------------------------------------
void CenterSoftBody::CreateJoint()
{
	//-----�o�[�ƌq����ʒu��ݒ�-----//
	int PointXNum = m_cylinder->GetXNum();
	int PointYNum = m_cylinder->GetYNum();
	SoftBodyCylinder::Points& points = *m_cylinder->GetPoints();

	// ����
	m_jointPoints.push_back(points[0].GetPosition());
	points[0].SetState(PhysicalPoint::STATE::FIXED);
	// �E��
	m_jointPoints.push_back(points[PointXNum - 1].GetPosition());
	points[PointXNum - 1].SetState(PhysicalPoint::STATE::FIXED);
	// ����
	m_jointPoints.push_back(points[PointXNum].GetPosition());
	points[PointXNum].SetState(PhysicalPoint::STATE::FIXED);
	// �E��
	m_jointPoints.push_back(points[PointXNum * PointYNum - 1].GetPosition());
	points[PointXNum * PointYNum - 1].SetState(PhysicalPoint::STATE::FIXED);

	// ����
	m_jointPoints.push_back(points[PointXNum / 2].GetPosition());
	points[PointXNum / 2].SetState(PhysicalPoint::STATE::FIXED);
	// �E��
	m_jointPoints.push_back(points[PointXNum / 2 + 1].GetPosition());
	points[PointXNum / 2 + 1].SetState(PhysicalPoint::STATE::FIXED); 

	// ����
	m_jointPoints.push_back(points[PointXNum + PointXNum / 2].GetPosition());
	points[PointXNum + PointXNum / 2].SetState(PhysicalPoint::STATE::FIXED);

	// �E��
	m_jointPoints.push_back(points[PointXNum + PointXNum / 2 + 1].GetPosition());
	points[PointXNum + PointXNum / 2 + 1].SetState(PhysicalPoint::STATE::FIXED);

}