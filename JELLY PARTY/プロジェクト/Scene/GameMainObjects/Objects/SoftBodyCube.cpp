//--------------------------------------------------------------------------------------
// File: SoftBodyCube.h
//
// �����Ă���[���[
//
// Date: 2023.11.12
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "SoftBodyCube.h"

#include "MyLib/ObjectCollision.h"
#include "Scene/GameManager.h"
#include <random>
#include "MyLib/SoundUtility.h"

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
SoftBodyCube::SoftBodyCube()
	:
	m_cubeIndex(0)
	, m_appeared(false)
{
	auto deviceResources = Graphics::GetInstance()->GetDeviceResources();
	auto device = deviceResources->GetD3DDevice();
	auto context = deviceResources->GetD3DDeviceContext();

	// �e�N�X�`����ݒ�
	m_texture = Resources::GetInstance()->GetTexture(L"Jelly").texture;
	// �Փ˔���̕\���I�u�W�F�N�g�̍쐬
	m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);

	// �p�����[�^��ǂݍ���
	Resources::GetInstance()->ReadJsonFile(&m_parameters, L"Resources/Datas/JellyCubeParameter");

	// �X�|�[���̃C���^�[�o��
	m_spawnTime = m_parameters["SPAWN_INTERVAL"];

	// �L���[�u�̐�
	m_cubeMaxNum = m_parameters["CUBE_MAX_NUM"];
	// �L���[�u�̐�
	m_cubeNum = m_parameters["CUBE_NUM"];

	CreateCube();

	for (int i = 0; i < m_cubeMaxNum - m_cubeNum; i++)
	{
		DupricateCube();
	}

	// �O�̂��߃L���p���T�C�Y�ɍ��킹��
	m_boxes.shrink_to_fit();

	m_getParticle = std::make_unique<GetParticle>();
	m_getParticle->Create(deviceResources);

	m_hitSound = Resources::GetInstance()->GetSound(L"HitJelly");
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
SoftBodyCube::~SoftBodyCube()
{
	m_boxes.clear();
}

//------------------------------------------
// �L���[�u�̍쐬
//------------------------------------------
void SoftBodyCube::CreateCube()
{
	// �X�|�[���͈�
	const float SPAWN_RANGE = static_cast<float>(m_parameters["SPAWN_RANGE"]);
	// �X�|�[���̍���
	const float SPAWN_HEIGHT = static_cast<float>(m_parameters["SPAWN_HEIGHT"]);
	// �X�P�[�����z�͈̔�
	const float SCALE_DIST_MIN = static_cast<float>(m_parameters["SCALE_DIST_MIN"]);
	const float SCALE_DIST_MAX = static_cast<float>(m_parameters["SCALE_DIST_MAX"]);
	// �F���z�͈̔�
	const float COLOR_DIST_MIN = static_cast<float>(m_parameters["COLOR_DIST_MIN"]);
	const float COLOR_DIST_MAX = static_cast<float>(m_parameters["COLOR_DIST_MAX"]);

	// 1�̃L���[�u�̒��_�̐�
	const int POINT_NUM = m_parameters["POINT_NUM"];

	// �\�t�g�{�f�B���쐬
	std::random_device seed;
	std::mt19937 engine(seed());
	// �����_���Ȉʒu
	std::uniform_real_distribution<float> positionDist(-SPAWN_RANGE, SPAWN_RANGE);
	// �����_���ȉ�]
	std::uniform_real_distribution<float> radianDist(-DirectX::XM_PI, DirectX::XM_PI);
	std::uniform_int_distribution<>		  rotationDist(0, 1);
	// �����_���ȐF
	std::uniform_real_distribution<float> colorDist(COLOR_DIST_MIN, COLOR_DIST_MAX);
	// �����_���ȃX�P�[��
	std::uniform_real_distribution<float> scaleDist(SCALE_DIST_MIN, SCALE_DIST_MAX);

	// ��]���������_���Ɍ��߂�
	int pivot = rotationDist(engine);
	DirectX::SimpleMath::Matrix rotation;
	if (pivot == 0)
	{
		rotation = DirectX::SimpleMath::Matrix::CreateRotationX(radianDist(engine));
	}
	else
	{
		rotation = DirectX::SimpleMath::Matrix::CreateRotationX(radianDist(engine));
	}

	// �����_���X�P�[��
	float scale = scaleDist(engine);

	// �����_���ʒu
	DirectX::SimpleMath::Vector3 randomPos = { positionDist(engine),positionDist(engine) + SPAWN_HEIGHT ,positionDist(engine) };

	// �\�t�g�{�f�B���쐬
	m_boxes.push_back(
		SoftBodyFactory::CreateBox(
			POINT_NUM, POINT_NUM, POINT_NUM,
			randomPos,
			{},
			DirectX::SimpleMath::Vector3(scale, scale, scale),
			DirectX::SimpleMath::Color(colorDist(engine), colorDist(engine), colorDist(engine), 1.0f),
			m_texture.Get(),
			PhysicalPoint::STATE::UNFIXED
		)
	);

	DirectX::BoundingSphere sphere(m_boxes[m_boxes.size() - 1]->GetPosition(), scale / 2.0f);
	m_collisions.push_back(sphere);
}


//------------------------------------------
// �X�V
//------------------------------------------
void SoftBodyCube::Update(const int& spendingTime, CenterSoftBody* pCylinder)
{
	auto timer = Graphics::GetInstance()->GetStepTimer();

	m_spendingTime = spendingTime;

	// �L���[�u���o��
	AppearCubes();

	for (int i = 0; i < m_cubeNum; i++)
	{
		// �X�V
		CubesUpdate(i);

		// ���f���ƒ��_�̓����蔻��
		ObjectCollision::GetInstance()->PointsToModel(m_boxes[i].get(), 0.01f);

		for (int j = 0; j < m_boxes.size(); j++)
		{
			if (i == j) continue;
			if (m_cubeNum == 1) continue;
			// �[���[�L���[�u���m�̓����蔻��
			ObjectCollision::GetInstance()->CubeToOtherCube(
				m_boxes[i].get(), m_collisions[i],
				m_boxes[j].get(), m_collisions[j]
			);
		}

		// ���ƃ[���[�L���[�u�̓����蔻��
		if (ObjectCollision::GetInstance()->CatToCube(m_collisions[i]))
		{
			// �������Ă�����쐬
			m_getParticle->CreateGetParticle(m_boxes[i]->GetPosition(), m_boxes[i]->GetColor());
			// �Đ�
			m_getParticle->SetState(GetParticle::State::PLAY);

			// �q�b�g����炷
			PlaySound(m_hitSound, false);

			// �p�����[�^������
			ResetParameters(i);
		}

		// ���Z�b�g�ʒu
		const float RESET_HEIGHT = 30;

		// ���݂̈ʒu
		DirectX::SimpleMath::Vector3 position = m_boxes[i]->GetPosition();
		// �����ʒu
		DirectX::SimpleMath::Vector3 initialPosition = m_boxes[i]->GetInitialPosition();

		// ���݂̈ʒu�������ʒu - ���Z�b�g�ʒu���Ⴍ�Ȃ����珉����
		if (position.y <= initialPosition.y - RESET_HEIGHT)
		{
			ResetParameters(i);
		}

		// ���S�̃[���[�ƃL���[�u�̏Փ˔���
		ObjectCollision::GetInstance()->ShaftToCube(pCylinder, m_boxes[i].get(), m_collisions[i]);
	}

	// �擾�p�[�e�B�N���̍X�V
	m_getParticle->Update(*timer);
}

//------------------------------------------
// �`��
//------------------------------------------
void SoftBodyCube::Render()
{
	if (m_boxes.size() == 0) return;

	for (int i = 0; i < m_cubeNum; i++)
	{
		// �\�t�g�{�f�B�e�N�X�`���`��
		m_boxes[i]->Render();
	}

	// �p�[�e�B�N���`��
	m_getParticle->CreateBillboard(DirectX::SimpleMath::Vector3::Up);
	m_getParticle->Render();
}


//------------------------------------------
// �e�`��
//------------------------------------------
void SoftBodyCube::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	for (int i = 0; i < m_cubeNum; i++)
	{
		// �\�t�g�{�f�B�e�`��
		m_boxes[i]->ShadowRender(lightView, lightProjection);
	}
}

//------------------------------------------
// ���_�̏�Ԃ�ݒ�
//------------------------------------------
void SoftBodyCube::CubesUpdate(const int& index)
{
	auto timer = Graphics::GetInstance()->GetStepTimer();

	// �\�t�g�{�f�B�̍X�V
	m_boxes[index]->Update(*timer);

	// ���_�擾
	SoftBodyBox::Points points = *m_boxes[index]->GetPoints();

	DirectX::SimpleMath::Vector3 p1 = points[0].GetPosition();
	DirectX::SimpleMath::Vector3 p2 = points[points.size() - 1].GetPosition();

	// ���S�_��ݒ�
	DirectX::SimpleMath::Vector3 center = (p1 + p2) / 2;
	m_collisions[index].Center = center;
}

//------------------------------------------
// �o�[�ƃ[���[�L���[�u�̓����蔻��
//------------------------------------------
void SoftBodyCube::HitBar(const int& index, SoftBodyBar* pBar, const float& speed)
{
	const float FORCE_RATIO = 25.0f;
	for (int i = 0; i < pBar->BAR_NUM; i++)
	{
		// �����������_����[���[�L���[�u�܂ł̌���
		DirectX::SimpleMath::Vector3 direction;
		// �o�[�ƃ[���[�L���[�u�̏Փ˔���
		if (ObjectCollision::GetInstance()->BarToCubeBody(
			m_collisions[index], pBar->GetSoftBodies(i), &direction)
			)
		{
			// �͂̌������v�Z
			DirectX::SimpleMath::Vector3 newForce =
				-direction * speed * FORCE_RATIO;
			newForce.y /= FORCE_RATIO;

			// ���_�S�Ăɗ͂�ݒ�
			SoftBodyBox::Points& points = *m_boxes[index]->GetPoints();
			for (auto& point : points)
			{
				point.UpdateVector(newForce);
			}
		}
	}
}


//------------------------------------------
// �[���[�L���[�u���o��������
//------------------------------------------
void SoftBodyCube::AppearCubes()
{
	if (m_spendingTime == 0 || m_spendingTime % m_spawnTime != 0) return;
	if (m_cubeMaxNum == m_cubeNum) return;

	m_cubeNum++;
}


//------------------------------------------
// �L���[�u�𕡐�����
//------------------------------------------
void SoftBodyCube::DupricateCube()
{
	std::unique_ptr<SoftBodyBox> box;
	box = std::make_unique<SoftBodyBox>();
	// �N���[���𐶐�
	m_boxes[0]->Clone(box.get());

	const float SPAWN_RANGE = static_cast<float>(m_parameters["SPAWN_RANGE"]);
	const float SPAWN_HEIGHT = static_cast<float>(m_parameters["SPAWN_HEIGHT"]);

	const float COLOR_DIST_MIN = static_cast<float>(m_parameters["COLOR_DIST_MIN"]);
	const float COLOR_DIST_MAX = static_cast<float>(m_parameters["COLOR_DIST_MAX"]);

	// �\�t�g�{�f�B���쐬
	std::random_device seed;
	std::mt19937 engine(seed());
	// �����_���Ȉʒu
	std::uniform_real_distribution<float> positionDist(-SPAWN_RANGE, SPAWN_RANGE);
	DirectX::SimpleMath::Vector3 randomPos(positionDist(engine), positionDist(engine) + SPAWN_HEIGHT, positionDist(engine));

	// �����_���ȐF
	std::uniform_real_distribution<float> colorDist(COLOR_DIST_MIN, COLOR_DIST_MAX);
	DirectX::SimpleMath::Color newColor(colorDist(engine), colorDist(engine), colorDist(engine), 1.0f);

	box->ResetParameters(randomPos);
	box->SetColor(newColor);

	m_boxes.push_back(std::move(box));

	// �Փ˔�����쐬
	DirectX::BoundingSphere sphere(m_boxes[m_boxes.size() - 1]->GetPosition(), m_boxes[0]->GetScale().x / 1.8f);
	m_collisions.push_back(sphere);

}

//------------------------------------------
// �p�����[�^��������
//------------------------------------------
void SoftBodyCube::ResetParameters(const int& index)
{
	const float SPAWN_RANGE = static_cast<float>(m_parameters["SPAWN_RANGE"]);
	const float SPAWN_HEIGHT = static_cast<float>(m_parameters["SPAWN_HEIGHT"]);

	const float COLOR_DIST_MIN = static_cast<float>(m_parameters["COLOR_DIST_MIN"]);
	const float COLOR_DIST_MAX = static_cast<float>(m_parameters["COLOR_DIST_MAX"]);

	// �\�t�g�{�f�B���쐬
	std::random_device seed;
	std::mt19937 engine(seed());
	// �����_���Ȉʒu
	std::uniform_real_distribution<float> positionDist(-SPAWN_RANGE, SPAWN_RANGE);
	DirectX::SimpleMath::Vector3 randomPos(positionDist(engine), positionDist(engine) + SPAWN_HEIGHT, positionDist(engine));

	// �����_���ȐF
	std::uniform_real_distribution<float> colorDist(COLOR_DIST_MIN, COLOR_DIST_MAX);
	DirectX::SimpleMath::Color newColor(colorDist(engine), colorDist(engine), colorDist(engine), 1.0f);

	m_boxes[index]->ResetParameters(randomPos);

	m_boxes[index]->SetColor(newColor);
}

//------------------------------------------
// ���_�̏�Ԃ�ݒ�
//------------------------------------------
void SoftBodyCube::SetPointsState()
{
	for (int i = 0; i < m_cubeNum; i++)
	{
		SoftBodyBox::Points& points = *m_boxes[i]->GetPoints();

		for (auto& p : points)
		{
			p.SetState(PhysicalPoint::STATE::UNFIXED);
		}
	}
}

//------------------------------------------
// ���U���g���̏�����
//------------------------------------------
void SoftBodyCube::ResultInitialize(Score* pScore)
{
	m_pScore = pScore;
	m_timer = 0;
	m_interval = 0.0f;

	m_resultResetPosition = DirectX::SimpleMath::Vector3(0.0f, -10.0f, -7.0f);

	// ����̃X�R�A
	const int NOW_SCORE = GameManager::GetInstance()->GetNowScore();

	// �X�R�A�̐����o�����Ă���L���[�u�̗ʂ��傫��
	if (m_boxes.size() < NOW_SCORE)
	{
		// �X�R�A�̐����L���[�u�̍ő吔���傫���ꍇ
		if (NOW_SCORE > m_cubeMaxNum)
		{
			// �ő吔�܂ō쐬
			for (int i = m_boxes.size(); i < m_cubeMaxNum; i++)
			{
				DupricateCube();
			}
		}
		else
		{
			// �X�R�A�̐��܂ō쐬
			for (int i = m_boxes.size(); i < NOW_SCORE; i++)
			{
				DupricateCube();
			}
		}
	}

	for (int i = 0; i < m_boxes.size(); i++)
	{
		m_boxes[i]->ResultReset(m_resultResetPosition);
	}
}

//------------------------------------------
// ���U���g���̍X�V
//------------------------------------------
void SoftBodyCube::ResultUpdate()
{
	auto timer = Graphics::GetInstance()->GetStepTimer();
	float elapsedTime = static_cast<float>(timer->GetElapsedSeconds());

	const int INTERVAL = 5;

	// ����̃X�R�A
	const int NOW_SCORE = GameManager::GetInstance()->GetNowScore();

	m_timer++;
	if (m_timer % INTERVAL == 0)
	{
		// �J�E���g���擾�������Ɠ�����������
		if (m_pScore->GetScore() == NOW_SCORE)
		{
			m_interval += elapsedTime;
			// �L���������ʂ������܂ł̎���
			if (m_interval > 0.2f)
			{
				// �J�E���g�I��
				m_pScore->SetResultCountFinish(true);
			}
			return;
		}

		// �J�E���g�A�b�v
		m_pScore->CountUp();

		if (m_pScore->GetScore() >= NOW_SCORE)
		{
			// �J�E���g���擾�������ȏゾ������擾�������ɐݒ�
			m_pScore->SetScore(NOW_SCORE);
		}
		else
		{
			// �q�b�g����炷
			PlaySound(m_hitSound, false);
		}
	}

	// �J�E���g�I������
	if (!m_pScore->GetResultCountFinish()) return;

	// �X�R�A���L���[�u�̍ő吔���傫����΃��U���g�ŏo���L���[�u�̐��𐧌�
	if (NOW_SCORE > m_cubeMaxNum)
	{
		m_cubeIndex = m_cubeMaxNum;
	}
	else
	{
		m_cubeIndex = NOW_SCORE;
	}

	for (int i = 0; i < m_cubeIndex; i++)
	{
		CubesUpdate(i);

		// ���f���ƒ��_�̓����蔻��
		ObjectCollision::GetInstance()->PointsToModel(m_boxes[i].get(), 0.01f);

		for (int j = 0; j < m_cubeIndex; j++)
		{
			if (i == j) continue;
			// �[���[�L���[�u���m�̓����蔻��
			ObjectCollision::GetInstance()->CubeToOtherCube(
				m_boxes[i].get(), m_collisions[i],
				m_boxes[j].get(), m_collisions[j]
			);
		}
	}
}

//------------------------------------------
// ���U���g���̕`��
//------------------------------------------
void SoftBodyCube::ResultRender()
{
	if (!m_pScore->GetResultCountFinish()) return;


	for (int i = 0; i < m_cubeIndex; i++)
	{
		// �\�t�g�{�f�B�e�N�X�`���`��
		m_boxes[i]->Render();
	}
}


//------------------------------------------
// �e�`��
//------------------------------------------
void SoftBodyCube::ResultShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	for (int i = 0; i < m_cubeIndex; i++)
	{
		// �\�t�g�{�f�B�e�`��
		m_boxes[i]->ShadowRender(lightView, lightProjection);
	}
}