//--------------------------------------------------------------------------------------
// File: SoftBodyCylinder.cpp
//
// �~���\�t�g�{�f�B�N���X
//
// Last Update: 2023.11.14
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "SoftBodyCylinder.h"

const float SCALE = 1.0f;

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
SoftBodyCylinder::SoftBodyCylinder(
	const int& xNum, const int& yNum,
	const float& radius,
	const float& height,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotation,
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Color& color,
	const PhysicalPoint::STATE& state
)
	:
	POINT_Y_NUM(yNum)
	, m_radius(radius)
	, m_height(height)
	, m_position(position)
	, m_initPosition(m_position)
	, m_rotation(rotation)
	, m_scale(scale)
	, m_color(color)
	, m_state(state)
	, m_isHit(false)
{
	// 8�̔{��
	POINT_X_NUM *= xNum;


	// uv�̃I�t�Z�b�g���v�Z
	m_uvOffset.x = 1.0f / (POINT_X_NUM  / 4);
	m_uvOffset.y = 1.0f / (POINT_Y_NUM - 1);
	//m_uvOffset.z = 1.0f / (POINT_X_NUM / 2 + 1);
}

//------------------------------------------
// ������
//------------------------------------------
void SoftBodyCylinder::Intialize(ID3D11ShaderResourceView* texture)
{
	// �ʂ̐�
	for (auto& render : m_renders)
	{
		// �����_�[�̍쐬
		render = std::make_unique<RenderSoftBody>();
		render->Create(texture);
	}

	for (int i = 0; i < FACENUM; i++)
	{
		std::vector<DirectX::VertexPositionColorTexture> vertices;

		// �g�b�v�ƃT�C�h�̂݌v�Z����
		if (!(i % 2))
		{
			CalculateUV(i);
			CalculateIndex(i);
		}

		for (int j = 0; j < m_faceIndices[i].size(); j++)
		{
			// ���_���쐬
			DirectX::VertexPositionColorTexture vertex = {
				m_points[m_faceIndices[i][j]].GetPosition(),
				m_color,
				m_points[m_faceIndices[i][j]].GetUV()
			};
			vertices.push_back(vertex);
		}
		// �o�b�t�@���쐬
		m_renders[i]->CreateBuffer(vertices);
	}
}

//------------------------------------------
// �X�V
//------------------------------------------
void SoftBodyCylinder::Update(const DX::StepTimer& timer)
{
	// ���_�̍X�V
	for (PhysicalPoint& point : m_points)
	{
		point.Update(timer);
	}

	UpdateRenderPosition();

	// �΂˂̍X�V
	for (Spring& spring : m_springs)
	{
		spring.CalculateForce();
	}

	int num = static_cast<int>(m_points.size() - 1);
	// ���S�_�̈ʒu
	m_position = (m_points[0].GetPosition() + m_points[num].GetPosition()) / 2;

}

//------------------------------------------
// ��]
//------------------------------------------
void SoftBodyCylinder::Rotate(DirectX::SimpleMath::Matrix rotation)
{
	for (PhysicalPoint& point : m_points)
	{
		point.SetPosition(DirectX::SimpleMath::Vector3::Transform(point.GetPosition(), rotation));
	}
}

//------------------------------------------
// �`��
//------------------------------------------
void SoftBodyCylinder::Render()
{
	for (auto& render : m_renders)
	{
		render->Render();
	}
}

//------------------------------------------
// �e�`��
//------------------------------------------
void SoftBodyCylinder::ShadowRender(
	const DirectX::SimpleMath::Matrix& lightView,
	const DirectX::SimpleMath::Matrix& lightProjection
)
{
	// �ʂ�`��
	for (auto& render : m_renders)
	{
		render->ShadowRender(lightView, lightProjection);
	}
}



//------------------------------------------
// �`��ʒu���X�V
//------------------------------------------
void SoftBodyCylinder::UpdateRenderPosition()
{
	// �ʂ̈ʒu
	for (int i = 0; i < m_faceIndices.size(); i++)
	{
		std::vector<DirectX::SimpleMath::Vector3> verticesPos;
		// ���_�̃C���f�b�N�X
		for (int j = 0; j < m_faceIndices[i].size(); j++)
		{
			DirectX::SimpleMath::Vector3 position = m_points[m_faceIndices[i][j]].GetPosition();
			verticesPos.push_back(position);
		}
		m_renders[i]->Update(verticesPos);

	}
}

//------------------------------------------
// �\�t�g�{�f�B���`���i3�����j
//------------------------------------------
void SoftBodyCylinder::CreateFigure()
{

	// �R���e�i������
	m_points.clear();

	int ID = 1;
	
	for (int j = 0; j < POINT_Y_NUM; j++)
	{
		for (int i = 0; i < POINT_X_NUM; ++i)
		{
			float angle = DirectX::XM_2PI * static_cast<float>(i) / static_cast<float>(POINT_X_NUM);
			float x = m_radius * cosf(angle);
			float y = m_radius * sinf(angle);

			// Vector3�ɕϊ�
			DirectX::SimpleMath::Vector3 pos = { x, m_position.y + m_height * j ,y };
			// �g�k
			pos *= SCALE;
			// ��]
			pos = DirectX::SimpleMath::Vector3::Transform(pos, m_rotation);
			// �ړ�
			pos += m_position;
			// �_�̃|�W�V�����ݒ�
			PhysicalPoint p(pos, ID);
			// ���_�̃R���C�_�[�̃T�C�Y��ݒ�
			p.SetColliderSize(0.2f);
			// vector�z��Ɋi�[
			m_points.push_back(p);

			ID++;
		}
	}
	// �]���ȃL���p�폜
	m_points.shrink_to_fit();
	// �\�t�g�{�f�B�̏�Ԃ�����
	CheckBodyState();
	// �E���ɖʂ��쐬
	CreateFaces();
}

//------------------------------------------
// �Œ肷�邩�ǂ���
//------------------------------------------
void SoftBodyCylinder::CheckBodyState()
{
	for (int i = 0; i < POINT_X_NUM; i++)
	{
		m_points[i].SetState(PhysicalPoint::STATE::BAR);
		m_points[i + POINT_X_NUM].SetState(PhysicalPoint::STATE::BAR);
	}
}

//------------------------------------------
// �_�Ɠ_���Ȃ��΂˂��쐬�i3�����j
//------------------------------------------
void SoftBodyCylinder::CreateArms()
{
	// �R���e�i������
	m_springs.clear();

	// �~���̉~��������ڑ�
	for (int i = 0; i < m_pointsIndices[SIDE].size() - 3; i += 4)
	{
		Spring s0(&m_points[m_pointsIndices[SIDE][i]], &m_points[m_pointsIndices[SIDE][i + 1]]);
		Spring s1(&m_points[m_pointsIndices[SIDE][i + 1]], &m_points[m_pointsIndices[SIDE][i + 2]]);
		Spring s2(&m_points[m_pointsIndices[SIDE][i + 2]], &m_points[m_pointsIndices[SIDE][i + 3]]);
		Spring s3(&m_points[m_pointsIndices[SIDE][i + 3]], &m_points[m_pointsIndices[SIDE][i]]);
		m_springs.push_back(s0);
		m_springs.push_back(s1);
		m_springs.push_back(s2);
		m_springs.push_back(s3);
	}
	// ���̕��Ɛڑ�
	for (int i = 0; i < POINT_X_NUM / 2; i++)
	{
		Spring s0(&m_points[i], &m_points[i + POINT_X_NUM / 2]);
		int next = i + POINT_X_NUM;
		Spring s1(&m_points[next], &m_points[next + (POINT_X_NUM / 2)]);

		m_springs.push_back(s0);
		m_springs.push_back(s1);
	}

	// �]���ȃL���p�폜
	m_springs.shrink_to_fit();
}

//------------------------------------------
// �ʂ��쐬
//------------------------------------------
void SoftBodyCylinder::CreateFaces()
{

	// �ʍ쐬
	CreateTBFaces();
	CreateSideFaces();

	// �΂˂��쐬
	CreateArms();

	for (auto& pointIndices : m_pointsIndices)
	{
		pointIndices.clear();
	}

}

//------------------------------------------
// ���ʂ��쐬
//------------------------------------------
void SoftBodyCylinder::CreateSideFaces()
{
	for (int i = 0; i < POINT_X_NUM; i++)
	{
		int a[4];
		if (i == POINT_X_NUM - 1)
		{
			a[0] = POINT_X_NUM - 1; a[1] = 0;
			a[2] = POINT_X_NUM * 2 - 1; a[3] = POINT_X_NUM;

			m_pointsIndices[SIDE].push_back(a[3]); m_pointsIndices[SIDE].push_back(a[1]);
			m_pointsIndices[SIDE].push_back(a[2]); m_pointsIndices[SIDE].push_back(a[0]);

			break;
		}
		// �����̒��_				// �E���̒��_
		a[0] = i;					a[1] = i + 1;
		// ����̒��_				// �E��̒��_
		a[2] = i + POINT_X_NUM;		a[3] = i + POINT_X_NUM + 1;

		m_pointsIndices[SIDE].push_back(a[3]); m_pointsIndices[SIDE].push_back(a[1]);
		m_pointsIndices[SIDE].push_back(a[2]); m_pointsIndices[SIDE].push_back(a[0]);

	}

	DeleteDuplication(SIDE);

}

//------------------------------------------
// ��ƒ�ʂ��쐬
//------------------------------------------
void SoftBodyCylinder::CreateTBFaces()
{
	// �[�̖ʂ͕ʂō쐬
	int a[4];
	// �E��̒��_			// �E���̒��_
	a[0] = 0;				a[1] = 1;
	// ����̒��_			// �E��̒��_
	a[2] = POINT_X_NUM - 1;	a[3] = 2;
	// ��̖ʂ��쐬
	m_pointsIndices[BOTTOM].push_back(a[0]); m_pointsIndices[BOTTOM].push_back(a[3]);
	m_pointsIndices[BOTTOM].push_back(a[1]); m_pointsIndices[BOTTOM].push_back(a[2]);
	// ��̖ʂ��쐬
	int next = POINT_X_NUM;
	m_pointsIndices[TOP].push_back(a[3] + next); m_pointsIndices[TOP].push_back(a[2] + next);
	m_pointsIndices[TOP].push_back(a[0] + next); m_pointsIndices[TOP].push_back(a[1] + next);

	int j = 2;
	for (int i = POINT_X_NUM - 1; i > POINT_X_NUM / 2 + 1; i--)
	{
		// �����̒��_	// �E���̒��_
		a[0] = i;		a[2] = i - 1;
		// ����̒��_	// �E��̒��_
		a[1] = j;	a[3] = j + 1;
		// ��̖ʂ��쐬
		m_pointsIndices[BOTTOM].push_back(a[0]); m_pointsIndices[BOTTOM].push_back(a[3]);
		m_pointsIndices[BOTTOM].push_back(a[1]); m_pointsIndices[BOTTOM].push_back(a[2]);
		// ��̖ʂ��쐬
		m_pointsIndices[TOP].push_back(a[3] + next); m_pointsIndices[TOP].push_back(a[2] + next);
		m_pointsIndices[TOP].push_back(a[0] + next); m_pointsIndices[TOP].push_back(a[1] + next);
		
		j++;
	}

	DeleteDuplication(BOTTOM);
	DeleteDuplication(TOP);
}



//------------------------------------------
// �d�����폜
//------------------------------------------
void SoftBodyCylinder::DeleteDuplication(const Face& face)
{
	// �R�s�[
	m_faceIndices[face] = m_pointsIndices[face];
	// �����Ƀ\�[�g
	std::sort(m_faceIndices[face].begin(), m_faceIndices[face].end());
	// �d�����폜
	m_faceIndices[face].erase(
		std::unique(
			m_faceIndices[face].begin(),
			m_faceIndices[face].end()
		),
		m_faceIndices[face].end()
	);

	m_faceIndices[face].shrink_to_fit();
}

//------------------------------------------
// UV�v�Z
//------------------------------------------
void SoftBodyCylinder::CalculateUV(const int& face)
{

	int index = 0;

	DirectX::SimpleMath::Vector2 sideUV[4] =
	{
		{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}
	};
	int indexOffset;
	switch (face)
	{
	case BOTTOM:

		// �~�̌`�ɂȂ��Ă��邩�l���͎��O�ɐݒ肷��
		m_points[m_faceIndices[TOP][0]].SetUV(sideUV[0]);
		m_points[m_faceIndices[TOP][POINT_X_NUM / 4]].SetUV(sideUV[1]);
		m_points[m_faceIndices[TOP][POINT_X_NUM / 2]].SetUV(sideUV[3]);
		m_points[m_faceIndices[TOP][POINT_X_NUM / 4 * 3]].SetUV(sideUV[2]);

		m_points[m_faceIndices[BOTTOM][0]].SetUV(sideUV[0]);
		m_points[m_faceIndices[BOTTOM][POINT_X_NUM / 4]].SetUV(sideUV[1]);
		m_points[m_faceIndices[BOTTOM][POINT_X_NUM / 2]].SetUV(sideUV[3]);
		m_points[m_faceIndices[BOTTOM][POINT_X_NUM / 4 * 3]].SetUV(sideUV[2]);

		// UV���W�����킹�邽�߂̔z��I�t�Z�b�g
		indexOffset = POINT_X_NUM / 4 - 2;

		// 1�`���_��4����1�̐�
		for (int i = 1; i < POINT_X_NUM / 4; i++)
		{
			float y = i * m_uvOffset.x;

			DirectX::SimpleMath::Vector2 uv[2] =
			{
				{0.0f, y}, {1.0f, y}
			};
			m_points[m_faceIndices[TOP][i]].SetUV(uv[0]);
			m_points[m_faceIndices[BOTTOM][i]].SetUV(uv[0]);
			int next = i + POINT_X_NUM / 2 + indexOffset;
			m_points[m_faceIndices[TOP][next]].SetUV(uv[1]);
			m_points[m_faceIndices[BOTTOM][next]].SetUV(uv[1]);

			indexOffset -= 2;
		}
		indexOffset = POINT_X_NUM / 4 - 2;
		
		index = POINT_X_NUM / 4 + 1;
		// 1�`���_����4����1 + 1
		for (int i = 1; i < POINT_X_NUM / 4; i++)
		{
			float x = i * m_uvOffset.x;

			DirectX::SimpleMath::Vector2 uv[2] =
			{
				{ x, 0.0f}, { x, 1.0f}
			};
			m_points[m_faceIndices[TOP][index]].SetUV(uv[1]);
			m_points[m_faceIndices[BOTTOM][index]].SetUV(uv[1]);
			int next = index + POINT_X_NUM / 2 + indexOffset;
			m_points[m_faceIndices[TOP][next]].SetUV(uv[0]);
			m_points[m_faceIndices[BOTTOM][next]].SetUV(uv[0]);
			index++;
			indexOffset -= 2;
		}
		break;
	case SIDE:
		for (int i = 0; i < POINT_Y_NUM; i++)
		{
			float y = i * m_uvOffset.y;

			for (int j = 0; j < POINT_X_NUM; j++)
			{
				DirectX::SimpleMath::Vector2 uv = { j * m_uvOffset.x, y };
				m_points[m_faceIndices[SIDE][index]].SetUV(uv);
				index++;
			}
		}
		break;
	default:
		break;
	}
}

//------------------------------------------
// �C���f�b�N�X�v�Z
//------------------------------------------
void SoftBodyCylinder::CalculateIndex(const int& face)
{
	// ���_�̃C���f�b�N�X�ԍ����v�Z
	std::vector<uint16_t> indices;
	uint16_t p[4];
	switch (face)
	{
	case BOTTOM:
		p[0] = 0;										p[1] = 1;
		p[2] = static_cast<uint16_t>(POINT_X_NUM - 1);	p[3] = 2;
		indices.push_back(p[0]);
		indices.push_back(p[1]);
		indices.push_back(p[2]);

		indices.push_back(p[1]);
		indices.push_back(p[2]);
		indices.push_back(p[3]);

		for (int i = 2; i < POINT_X_NUM / 2; i++)
		{
			p[0] = static_cast<uint16_t>(i);		p[1] = static_cast<uint16_t>(POINT_X_NUM - i + 1);
			p[2] = static_cast<uint16_t>(i + 1);	p[3] = static_cast<uint16_t>(POINT_X_NUM - i);

			indices.push_back(p[0]);
			indices.push_back(p[1]);
			indices.push_back(p[2]);

			indices.push_back(p[1]);
			indices.push_back(p[2]);
			indices.push_back(p[3]);
		}

		m_renders[TOP]->SetIndices(indices);
		m_renders[BOTTOM]->SetIndices(m_renders[TOP]->GetIndices());
		break;
	case SIDE:
		m_renders[SIDE]->CalculateIndex(POINT_X_NUM, POINT_Y_NUM);
		break;
	default:
		break;
	}

}
