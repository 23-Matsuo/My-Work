//--------------------------------------------------------------------------------------
// File: SoftBodyBar.h
//
// ��]����o�[
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "SoftBodyBar.h"
#include "MyLib/ObjectCollision.h"
#include "MyLib/SoftBody/SoftBodyFactory.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
SoftBodyBar::SoftBodyBar(const std::vector<DirectX::SimpleMath::Vector3>& jointPoints)
{
	auto deviceResources = Graphics::GetInstance()->GetDeviceResources();
	auto device = deviceResources->GetD3DDevice();
	auto context = deviceResources->GetD3DDeviceContext();

	// �e�N�X�`����ݒ�
	m_texture = Resources::GetInstance()->GetTexture(L"Jelly").texture;
	// �Փ˔���̕\���I�u�W�F�N�g�̍쐬
	m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);

	// �p�����[�^��ǂݍ���
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/JellyBarParameter");

	// ���_�̐��f�[�^
	for (const auto& num : m_parameters["POINT_NUM"])
	{
		m_pointXNum = num[0];
		m_pointYNum = num[1];
		m_pointZNum = num[2];
	}
	// �ʒu�f�[�^
	DirectX::SimpleMath::Vector3 initPos;
	for (const auto& position : m_parameters["INITIAL_POSITION"])
	{
		initPos = { position[0], position[1], position[2] };
	}


	// �q���钸�_�̈ʒu�������o�ϐ��ɑ��
	m_jointPoints = jointPoints;

	// �{�b�N�X�̍쐬
	m_box[0] = SoftBodyFactory::CreateBox(
		m_pointXNum, m_pointYNum, m_pointZNum,
		initPos,
		{},
		DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
		GetColor(COLORS::MAGENTA),
		m_texture.Get(),
		PhysicalPoint::STATE::BAR
	);

	m_box[1] = std::make_unique<SoftBodyBox>();
	m_box[0]->Clone(m_box[1].get());

	// 180�x��]���������̂𔽑Α��ɍ��
	DirectX::SimpleMath::Matrix rotation =
		DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XM_PI - DirectX::XMConvertToRadians(22.5f));
	m_box[1]->Rotate(rotation);


	// ���_�̏�Ԃ�ݒ�
	SetBarPointsState();

	//-----���_���W���C���g������-----//
	JointPoints();


	m_hitParticle = std::make_unique<HitParticle>();
	m_hitParticle->Create(deviceResources);

}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
SoftBodyBar::~SoftBodyBar()
{
	m_box[0].reset();
	m_box[1].reset();
}

//------------------------------------------
// �X�V
//------------------------------------------
void SoftBodyBar::Update(
	DirectX::SimpleMath::Matrix rotation,
	SoftBodyCylinder* pCylinder,
	const float& anglarVelocity
)
{
	auto timer = Graphics::GetInstance()->GetStepTimer();

	DirectX::SimpleMath::Vector3 hitPosition;

	for (int i = 0; i < BAR_NUM; i++)
	{
		// ��
		m_box[i]->Rotate(rotation);

		// �\�t�g�{�f�B�̍X�V
		m_box[i]->Update(*timer);

		// �Փ˔���
		if (ObjectCollision::GetInstance()->CatToBarPoints(
			m_box[i].get(), pCylinder, anglarVelocity, &hitPosition))
		{
			// �����������̃G�t�F�N�g����
			m_hitParticle->CreateHitParticle(hitPosition, m_box[i]->GetColor());
			m_hitParticle->SetState(HitParticle::State::PLAY);
		}
	}

	m_hitParticle->Update(*timer);
}

//------------------------------------------
// �`��
//------------------------------------------
void SoftBodyBar::Render()
{

	for (int i = 0; i < BAR_NUM; i++)
	{
		// �\�t�g�{�f�B�e�N�X�`���`��
		m_box[i]->Render();
	}

	// �p�[�e�B�N�����r���{�[�h�����ĕ`��
	m_hitParticle->CreateBillboard(DirectX::SimpleMath::Vector3::Up);
	m_hitParticle->Render();

}

//------------------------------------------
// �e�`��
//------------------------------------------
void SoftBodyBar::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	for (int i = 0; i < BAR_NUM; i++)
	{
		// �\�t�g�{�f�B�e�`��
		m_box[i]->ShadowRender(lightView, lightProjection);
	}

}

//------------------------------------------
// �F�̎擾
//------------------------------------------
DirectX::SimpleMath::Color SoftBodyBar::GetColor(COLORS color)
{
	DirectX::SimpleMath::Vector4 sbbColor;
	sbbColor.w = 1.0f;
	switch (color)
	{
	case COLORS::CYAN:
		sbbColor.x = 0.0f;
		sbbColor.y = 191.0f / (0.0f + 191.0f + 255.0f) * 1.5f;
		sbbColor.z = 255.0f / (0.0f + 191.0f + 255.0f) * 1.5f;
		break;
	case COLORS::MAGENTA:
		sbbColor.x = 255.0f / (255.0f + 20.0f + 147.0f) * 1.5f;
		sbbColor.y = 20.0f / (255.0f + 20.0f + 147.0f) * 1.5f;
		sbbColor.z = 137.0f / (255.0f + 20.0f + 147.0f) * 1.5f;
		break;
	case COLORS::YELLOW:
		sbbColor.x = 1.0f;
		sbbColor.y = 1.0f;
		sbbColor.z = 0.0f;
		break;
	default:
		break;
	}
	return sbbColor;
}

//------------------------------------------
// �o�[�̒��_�̏�Ԃ�ݒ�
//------------------------------------------
void SoftBodyBar::SetBarPointsState()
{
	for (int i = 0; i < BAR_NUM; i++)
	{
		SoftBodyBox::Points& points = *m_box[i]->GetPoints();

		for (PhysicalPoint& point : points)
		{
			// �S�ăo�[�̏�Ԃɂ���
			point.SetState(PhysicalPoint::STATE::BAR);
		}
	}
}

//------------------------------------------
// ���S�̃\�t�g�{�f�B�ƒ��_�q����
//------------------------------------------
void SoftBodyBar::JointPoints()
{
	// �E���̒��_�̐ݒ���s��
	SoftBodyBox::Points& pointsA = *m_box[0]->GetPoints();

	for (int i = 0; i < pointsA.size(); i += m_pointXNum)
	{
		pointsA[i].SetState(PhysicalPoint::STATE::FIXED);
	}
	// ����
	pointsA[m_pointXNum * m_pointYNum].SetPosition(m_jointPoints[0]);
	// �E��
	pointsA[0].SetPosition(m_jointPoints[1]);
	// ����
	pointsA[m_pointXNum * m_pointYNum * m_pointZNum - m_pointXNum].SetPosition(m_jointPoints[2]);
	// �E��
	pointsA[m_pointXNum * m_pointYNum - m_pointXNum].SetPosition(m_jointPoints[3]);

	// �����̒��_�̐ݒ���s��
	SoftBodyBox::Points& pointsB = *m_box[1]->GetPoints();

	for (int i = 0; i < pointsB.size(); i += m_pointXNum)
	{
		pointsB[i].SetState(PhysicalPoint::STATE::FIXED);
	}
	// ����
	pointsB[m_pointXNum * m_pointYNum].SetPosition(m_jointPoints[5]);
	// �E��
	pointsB[0].SetPosition(m_jointPoints[4]);
	// ����
	pointsB[m_pointXNum * m_pointYNum * m_pointZNum - m_pointXNum].SetPosition(m_jointPoints[7]);
	// �E��
	pointsB[m_pointXNum * m_pointYNum - m_pointXNum].SetPosition(m_jointPoints[6]);

}

