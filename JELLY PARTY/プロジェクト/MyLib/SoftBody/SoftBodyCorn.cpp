//--------------------------------------------------------------------------------------
// File: SoftBodyCorn.h
//
// �\�t�g�{�f�B���쐬����N���X
//
// Date: 2023.5.17
// LastUpdate Date: 2023/08/28 
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "SoftBodyCorn.h"

SoftBodyCorn::SoftBodyCorn()
	:
	m_position{}
	, m_pObjCollision(nullptr)
	, m_pSphere(nullptr)
	, m_rotation{}
	//, m_scale(1.0)
{
}

void SoftBodyCorn::Intialize(DirectX::SimpleMath::Vector3 startPos)
{
	// �����ʒu��ݒ�
	m_position = startPos;
}

void SoftBodyCorn::Update(const DX::StepTimer& timer)
{
	// �o�E���f�B���O�{�b�N�X�̒��a
	DirectX::SimpleMath::Vector3 max = m_points[0].GetPosition();
	DirectX::SimpleMath::Vector3 min = m_points[0].GetPosition();
	for (PhysicalPoint& point : m_points)
	{
		// ���_�̍X�V
		point.Update(timer);
		//// ���_�Ƌ��̓����蔻��
		//CollisionDitection(point);

		// �{�b�N�X�̔��a���X�V
		DirectX::SimpleMath::Vector3 pos = point.GetPosition();
		// ���a�̍ő�l��ݒ�
		if (max.x < pos.x) max.x = pos.x;
		if (max.y < pos.y) max.y = pos.y;
		if (max.z < pos.z) max.z = pos.z;
		// ���a�̍ŏ��l��ݒ�
		if (min.x > pos.x) min.x = pos.x;
		if (min.y > pos.y) min.y = pos.y;
		if (min.z > pos.z) min.z = pos.z;
	}

	// �΂˂̍X�V
	for (Spring &spring : m_springs)
	{
		spring.CalculateForce();
	}

	// �ʂƂ̓����蔻��
	for (SBTriangle &face : m_faces)
	{
		face.Update();
		if (face.CollisionDitection(m_pSphere))
		{
			break;
		}
	}
	//// �o�E���f�B���O�{�b�N�X�̒��S�ʒu���X�V
	//int num = POINT_X_NUM * POINT_Y_NUM * POINT_Z_NUM - 1;
	//m_sbBox->Center = (m_points[0].GetPosition() + m_points[num].GetPosition()) / 2;
	//// ���a�X�V
	//m_sbBox->Extents = (max - min) / 2;
	//// ���̒��S�_���{�b�N�X�̒���������
	//if (m_sbBox->Contains(m_pSphere->GetPosition()))
	//{
	//	// �\�t�g�{�f�B�̒������ׂ�
	//	if (IsPointInsideBox())
	//	{
	//		ClosestFace();
	//	}
	//}
}

/// <summary>
/// �`��
/// </summary>
/// <param name="batch"></param>
void SoftBodyCorn::Render(DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* batch)
{
	//// ���̕`��
	//for (auto &spring : m_springs)
	//{
	//	spring.Render(batch);
	//}
	//for (auto &face : m_faces)
	//{
	//	face.DebugRender(batch);
	//}
	//DX::Draw(batch, *m_sbBox.get(), DirectX::Colors::White);
	// �`��J�n
	batch->Begin();

	// ���_���(�|���S���̂S���_�̍��W���j
	std::vector<DirectX::VertexPositionColorTexture> vertex;
	// �`��J�n���_�̗v�f�ԍ�
	int num = 0;
	// �ʂ̐������[�v
	for (int i = 0; i < m_faces.size(); i += 1)
	{
		// �ʂ̒��_�ʒu��VPCT�ɕϊ�
		for (int j = 0; j < m_faces[i].GetPointsPosition().size(); j++)
		{
			vertex.push_back(DirectX::VertexPositionColorTexture(
				m_faces[i].GetPointsPosition()[j],
				DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 0.5f),
				DirectX::SimpleMath::Vector2(0.5f, 0.5f)));
		}
		// �`��
		batch->DrawTriangle(vertex[num], vertex[num + 1], vertex[num + 2]);
		// �`�悵�����_�̐����v���X
		num += 3;
	}
	// �`��I��
	batch->End();

}

/// <summary>
/// �\�t�g�{�f�B���`���i3�����j
/// </summary>
void SoftBodyCorn::CreateFigure()
{
	const float SCALE = 1.0f;
	// �R���e�i������
	m_points.clear();
	float height = 5.0f;
	// �_�̃C���X�^���X����	
	DirectX::SimpleMath::Vector3 pos;

	for (int h = 0; h < POINT_Z_NUM; h++)
	{
		// ���̓_�̐������[�v
		for (int j = 0; j < POINT_X_NUM; j++)
		{

			// x���W�v�Z(j - �_�̐�����1�����ā��Q������)
			float x = (j - static_cast<float>(POINT_X_NUM - 1) / 2.0f);
			// z���W�v�Z
			float z = (h - static_cast<float>(POINT_Z_NUM - 1) / 2.0f);

			// �_�̃C���X�^���X����	
			pos = DirectX::SimpleMath::Vector3(x, 0.0f, z);
			pos += m_position;
			PhysicalPoint p(pos * SCALE);
			// vector�z��Ɋi�[
			m_points.push_back(p);
		}
	}
	pos = DirectX::SimpleMath::Vector3(0.0f, height, 0.0f);
	pos += m_position;
	PhysicalPoint topPoint(pos);

	m_points.push_back(topPoint);
	m_points.shrink_to_fit();
	// �΂˂��쐬
	CreateArms();
	// �ʂ��쐬
	CreateFaces();
	// �o�E���f�B���O�{�b�N�X���쐬
	m_sbBox = std::make_unique<DirectX::BoundingBox>(m_position, DirectX::SimpleMath::Vector3::Zero);
}

/// <summary>
/// �_�Ɠ_���Ȃ��΂˂��쐬�i3�����j
/// </summary>
void SoftBodyCorn::CreateArms()
{
	m_springs.clear();
	// �΂߉������������q����
	const static int diagonally[9][3] = {
		{1, 0, 0}, {1, 0, 0},{-1, 0, 0},
		{0, 0, 1}, {1, 0, 1},{0, 0, -1},
		{-1, 0, -1}, {1, 0, -1}
	};

	for (int h = 0; h < POINT_Z_NUM; h++)
	{
		for (int j = 0; j < POINT_X_NUM; j++)
		{
			// �_�̉E��E�����㍶���ɓ_�����邩
			for (const auto& dia : diagonally)
			{
				// dia = -1�`1�̒l
				int	x = j + dia[0];
				int z = h + dia[2];

				// ����a��b���_�̐��͈̔͊O����Ȃ�������
				if (x >= 0 && x < POINT_X_NUM &&
					z >= 0 && z < POINT_Z_NUM)
				{
					// �Q�Ƃ��钸�_�̃|�C���^�̈ʒu���v�Z
					int a = h * POINT_X_NUM + j;

					// ���̒��_�̃|�C���^�̈ʒu���v�Z
					int b = z * POINT_X_NUM + x;
					// �΂˂̍쐬
					Spring spring(&m_points[a], &m_points[b]);
					// �΂˂̏���ۑ�
					m_springs.push_back(spring);
				}
			}
		}
	}

	int last = POINT_X_NUM * POINT_Z_NUM;
	Spring spring1(&m_points[0], &m_points[last]);
	m_springs.push_back(spring1);
	Spring spring2(&m_points[1], &m_points[last]);
	m_springs.push_back(spring2);
	Spring spring3(&m_points[2], &m_points[last]);
	m_springs.push_back(spring3);
	Spring spring4(&m_points[3], &m_points[last]);
	m_springs.push_back(spring4);
	// �]���Ȕz�������
	m_springs.shrink_to_fit();
}

/// <summary>
/// �ʂ��쐬
/// </summary>
void SoftBodyCorn::CreateFaces()
{
	m_faces.clear();
	// �O��㉺���E�̖ʂ��i�[����R���e�i
	std::vector<SBTriangle> sideTriangles;
	std::vector<SBTriangle> bottomTriangles;

	int p0 = 0;
	int p1 = POINT_X_NUM - 1;
	int p2 = POINT_X_NUM * POINT_Z_NUM - POINT_X_NUM;
	int p3 = POINT_X_NUM * POINT_Z_NUM - 1;
	SBTriangle sideTriangle2(&m_points[p2], &m_points[p3], &m_points[m_points.size() - 1]);
	sideTriangles.push_back(sideTriangle2);
	SBTriangle sideTriangle0(&m_points[p0], &m_points[p1], &m_points[m_points.size() - 1]);
	sideTriangles.push_back(sideTriangle0);
	SBTriangle sideTriangle1(&m_points[p0], &m_points[p2], &m_points[m_points.size() - 1]);
	sideTriangles.push_back(sideTriangle1);
	SBTriangle sideTriangle3(&m_points[p1], &m_points[p3], &m_points[m_points.size() - 1]);
	sideTriangles.push_back(sideTriangle3);

	SBTriangle bottomTriangle1(&m_points[p0], &m_points[p1], &m_points[p2]);
	sideTriangles.push_back(bottomTriangle1);
	SBTriangle bottomTriangle2(&m_points[p1], &m_points[p2], &m_points[p3]);
	sideTriangles.push_back(bottomTriangle2);

	std::copy(sideTriangles.begin(), sideTriangles.end(), std::back_inserter(m_faces));
	std::copy(bottomTriangles.begin(), bottomTriangles.end(), std::back_inserter(m_faces));

	// �]���ȃL���p���폜
	m_faces.shrink_to_fit();
}

void SoftBodyCorn::CollisionDitection(PhysicalPoint& point)
{

	// ���_�̈ʒu���擾
	DirectX::SimpleMath::Vector3 pointPos = point.GetPosition();
	// ���̈ʒu���擾
	DirectX::SimpleMath::Vector3 spherePos = m_pSphere->GetPosition();
	// �_�Ɠ_�̋���
	DirectX:: SimpleMath::Vector3 distance = point.GetPosition() - m_pSphere->GetPosition();
	// �����̓��ς����߂�
	float distSq = distance.Dot(distance);
	// ���a�̍��v
	float radisuSum = m_pSphere->RADIUS + point.GetRadius();
	// �����Ɣ��a���r
	if (distSq <= radisuSum * radisuSum)
	{
		// ���a�̍��v�̂ق����傫����Γ������Ă���
		// ���������_�ɗ͂�������
		point.AddForce(m_pSphere->GetTotalForce() * 20);
		// �{�[������
		m_pSphere->Reflect(m_rotation);
	}
}

/// <summary>
/// �_���\�t�g�{�f�B�̒������ׂ�
/// </summary>
/// <returns>�����������̗]��(1�Œ�, 0�ŊO)</returns>
bool SoftBodyCorn::IsPointInsideBox()
{
	// �ŋߐړ_��
	m_closestPoints.clear();
	// ����
	DirectX::SimpleMath::Ray ray = {
		m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(0.0f,  1.0f, 0.0f)
		//{m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f)},
		//{m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(0.0f,  0.0f, 1.0f)},
		//{m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f)},
		//{m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(1.0f,  0.0f, 0.0f)},
		//{m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f)},
	};
	// ������
	int intersectCount = 0;
	for (auto& face : m_faces)
	{
		// �����Ɩʂ̌�������
		if (face.RayIntersectsQuad(ray, m_pSphere))
		{
			// �������Ă�����J�E���g
			intersectCount++;
			// �ŋߐړ_���i�[
			m_closestPoints.push_back(face.GetClosestPoint());
		}
	}
	return intersectCount % 2;
}

/// <summary>
/// �ʏ�̈�ԋ߂��_���v�Z
/// </summary>
void SoftBodyCorn::ClosestFace()
{
	float distance = 100.0f;
	distance;
	int num = 0;
	num;

	// ����
	DirectX::SimpleMath::Ray ray = {
		m_pSphere->GetPosition(), DirectX::SimpleMath::Vector3(0.0f,  1.0f, 0.0f)
	};
	for (auto& face : m_faces)
	{
		face.CalculateForce(ray, m_pSphere);
	}

	//for (int i = 0; i < m_closestPoints.size(); i++)
	//{
	//	// ���������߂�
	//	float tmp = DirectX::SimpleMath::Vector3::DistanceSquared(m_pSphere->GetPosition(), m_closestPoints[i]);
	//	// ����������������ݒ肷��
	//	if (distance > tmp)
	//	{
	//		// ������ݒ�
	//		distance = tmp;
	//		// �ԍ���ݒ�
	//		num = i;
	//	}
	//}
	//
	//m_pSphere->SetPosition(m_closestPoints[num]);

}