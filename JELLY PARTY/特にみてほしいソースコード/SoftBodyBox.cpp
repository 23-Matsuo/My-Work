//--------------------------------------------------------------------------------------
// File: SoftBodyBox.cpp
//
// ���^�\�t�g�{�f�B�N���X
//
// Last Update: 2023.02.05	
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "SoftBodyBox.h"

//------------------------------------------
// �f�t�H���g�R���X�g���N�^
//------------------------------------------
SoftBodyBox::SoftBodyBox()
	:
	m_colorChanged(false)
	, m_state(PhysicalPoint::STATE::FIXED)
{

}

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
SoftBodyBox::SoftBodyBox(
	const int& xNum, const int& yNum, const int& zNum,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotation,
	const DirectX::SimpleMath::Vector3& scale,
	const DirectX::SimpleMath::Color& color,
	const PhysicalPoint::STATE& state)
	:
	POINT_X_NUM(xNum)
	, POINT_Y_NUM(yNum)
	, POINT_Z_NUM(zNum)
	, m_position(position)
	, m_initPosition(m_position)
	, m_rotation(rotation)
	, m_scale(scale)
	, m_color(color)
	, m_state(state)
	, m_colorChanged(false)
{
	// uv�̃I�t�Z�b�g���v�Z
	m_uvOffset.x = 1.0f / (POINT_X_NUM - 1);
	m_uvOffset.y = 1.0f / (POINT_Y_NUM - 1);
	m_uvOffset.z = 1.0f / (POINT_Z_NUM - 1);
}

//------------------------------------------
// ������
//------------------------------------------
void SoftBodyBox::Intialize(ID3D11ShaderResourceView* texture)
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
		// �g�b�v�ƃ��t�g�ƃt�����g�����v�Z����
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
			m_vertices[i].push_back(vertex);
		}
		// �o�b�t�@���쐬
		m_renders[i]->CreateBuffer(m_vertices[i]);
	}
}

//------------------------------------------
// �X�V
//------------------------------------------
void SoftBodyBox::Update(const DX::StepTimer& timer)
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

	// ���S�ʒu
	int num = static_cast<int>(m_points.size() - 1);
	m_position = (m_points[0].GetPosition() + m_points[num].GetPosition()) / 2;
	SetPosition(m_position);

	if (m_colorChanged)
	{
		for (int i = 0; i < m_renders.size(); i++)
		{
			m_renders[i]->SetColor(m_color);
		}
		m_colorChanged = false;
	}

}

//------------------------------------------
// �`��
//------------------------------------------
void SoftBodyBox::Render()
{
	// �ʂ�`��
	for (auto& render : m_renders)
	{
		render->Render();
	}
}

//------------------------------------------
// �e�`��
//------------------------------------------
void SoftBodyBox::ShadowRender(
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
void SoftBodyBox::UpdateRenderPosition()
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
/// �\�t�g�{�f�B���`���i3�����j
//------------------------------------------
void SoftBodyBox::CreateFigure()
{
	// �R���e�i������
	m_points.clear();

	int ID = 1;
	// Z���̓_�̐������[�v
	for (int h = 0; h < POINT_Z_NUM; h++)
	{
		// Y���̓_�̐������[�v
		for (int i = 0; i < POINT_Y_NUM; i++)
		{
			// X���̓_�̐������[�v
			for (int j = 0; j < POINT_X_NUM; j++)
			{
				// ���W�v�Z(j - �_�̐�����1�����ā�2������) 
				// �_�̐��������Ȃ�.5����n�܂�A��Ȃ琮��
				float x = (j - static_cast<float>(POINT_X_NUM - 1) / 2.0f);
				float y = (i - static_cast<float>(POINT_Y_NUM - 1) / 2.0f);
				float z = (h - static_cast<float>(POINT_Z_NUM - 1) / 2.0f);

				// Vector3�ɕϊ�
				DirectX::SimpleMath::Vector3 pos = { x,y,z };
				// �g�k
				pos *= m_scale;
				// ��]
				pos = DirectX::SimpleMath::Vector3::Transform(pos, m_rotation);
				// �ړ�
				pos += m_position;
				// �_�̃|�W�V�����ݒ�
				PhysicalPoint p(pos, ID);
				// vector�z��Ɋi�[
				m_points.push_back(p);

				ID++;
			}
		}
	}
	// �]���ȃL���p�폜
	m_points.shrink_to_fit();
	// �\�t�g�{�f�B�̏�Ԃ�����
	//CheckBodyState();
	// �΂˂��쐬
	CreateArms();
	// �E���ɖʂ��쐬
	CreateFaces();
}

//------------------------------------------
// �_�Ɠ_���Ȃ��΂˂��쐬�i3�����j
//------------------------------------------
void SoftBodyBox::CreateArms()
{
	// �E,�E��A��A����A���E�E�E�ɓ_�����邩

	//// �΂߉����q���Ȃ�
	//const static int diagonally[7][3] = { 
	//	{1, 0, 0}, {1, -1, 0}, {0, -1, 0}, {-1, -1, 0},
	//	{0, 0, 1}, {0, -1, 1}, {0, -1, -1},
	//};

	//// �΂߉������������q����
	//const static int diagonally[11][3] = { 
	//	{1, 0, 0}, {1, -1, 0}, {0, -1, 0}, 
	//	{-1, -1, 0}, {0, 0, 1}, {1, 0, 1},
	//	{0, -1, 1}, {0, -1, -1}, {-1, 0, -1},
	//  {1, 0, -1}
	//};

	// �S�������q����
	const static int diagonally[14][3] = {
		{1, 0, 0}, {1, -1, 0}, {0, -1, 0},			// �E, �E��, ��
		{-1, -1, 0}, {0, 0, 1}, {1, 0, 1},			// ����, ��, �E��
		{1, -1, 1}, {0, -1, 1}, {0, -1, -1},		// �E����, ����, ����O
		{-1, -1, -1} , {-1, 0, -1},	{1, 0, -1},		// ������O, ����O, �E��O
		{1, -1, -1}, {1, 1, -1}						// �E����O, �E���O
	};

	// �R���e�i������
	m_springs.clear();

	for (int h = 0; h < POINT_Z_NUM; h++)
	{
		for (int i = 0; i < POINT_Y_NUM; i++)
		{
			for (int j = 0; j < POINT_X_NUM; j++)
			{
				// �_�̉E��E�����㍶���ɓ_�����邩
				for (const auto& dia : diagonally)
				{
					// dia = -1�`1�̒l
					int	x = j + dia[0];
					int y = i + dia[1];
					int z = h + dia[2];

					// ����a��b���_�̐��͈̔͊O����Ȃ�������
					if (x >= 0 && x < POINT_X_NUM &&
						y >= 0 && y < POINT_Y_NUM &&
						z >= 0 && z < POINT_Z_NUM)
					{
						// �Q�Ƃ��钸�_�̃|�C���^�̈ʒu���v�Z
						int a = h * POINT_X_NUM * POINT_Y_NUM + i * POINT_X_NUM + j;

						// ���̒��_�̃|�C���^�̈ʒu���v�Z
						int b = z * POINT_X_NUM * POINT_Y_NUM + y * POINT_X_NUM + x;
						// �΂˂̍쐬
						Spring spring(&m_points[a], &m_points[b]);
						// �΂˂̏���ۑ�
						m_springs.push_back(spring);
					}
				}
			}
		}
	}
	// �]���ȃL���p�폜
	m_springs.shrink_to_fit();
}

//------------------------------------------
// �ʂ��쐬
//------------------------------------------
void SoftBodyBox::CreateFaces()
{

	// �ʍ쐬
	CreateTBFaces();
	CreateLRFaces();
	CreateFBFaces();

	for (auto& pointIndices : m_pointsIndices)
	{
		pointIndices.clear();
	}

}

//------------------------------------------
// �O�ƌ��ʂ��쐬
//------------------------------------------
void SoftBodyBox::CreateFBFaces()
{
	int j = 0, row = 1;
	// ���_�̍��v��
	int totalPointNum = static_cast<int>(m_points.size());

	// �����@ xNum * yNum����1�񕪌��炵�����̒��_����`�悷��
	//�i��Fx�̒��_�����R��y���R�̎��Ai��0�`1,3�`4�̎��Ɏl�p�`�|���S����`�悷��)
	for (int i = 0; i < POINT_X_NUM * POINT_Y_NUM - POINT_X_NUM - 1; i++)
	{
		// �����A i �� xNum - 1 * �s�� ��� �傫���Ȃ����� �s�����X�V
		if (i > POINT_X_NUM * row - 1)
		{
			row++; j++;
		}
		// i �� �O�̎��͖���
		if (i != 0)
		{
			// �����B i �� xNum - 1 + xNum * j ��i��������[�����Ȃ̂�i���X�V����
			if (i == (POINT_X_NUM - 1 + POINT_X_NUM * j))
			{
				i++;
			}
		}
		int a[4];
		// �����̒��_           // �E���̒��_
		a[0] = i;				a[2] = i + 1;
		// ����̒��_			// �E��̒��_
		a[1] = i + POINT_X_NUM;	a[3] = i + POINT_X_NUM + 1;
		// ���̖ʂ��쐬
		m_pointsIndices[Face::BACK].push_back(a[0]); m_pointsIndices[Face::BACK].push_back(a[2]);
		m_pointsIndices[Face::BACK].push_back(a[3]); m_pointsIndices[Face::BACK].push_back(a[1]);

		int next = totalPointNum - (POINT_X_NUM * POINT_Y_NUM);

		// ��O�̖ʂ��쐬
		m_pointsIndices[Face::FRONT].push_back(a[3] + next); m_pointsIndices[Face::FRONT].push_back(a[2] + next);
		m_pointsIndices[Face::FRONT].push_back(a[0] + next); m_pointsIndices[Face::FRONT].push_back(a[1] + next);
	}

	DeleteDuplication(BACK);
	DeleteDuplication(FRONT);

}

//------------------------------------------
// ��ƒ�ʂ��쐬
//------------------------------------------
void SoftBodyBox::CreateTBFaces()
{
	int j = 0, row = 0;
	int totalPointNum = static_cast<int>(m_points.size());

	// �����@ xNum * yNum����1�񕪌��炵�����̒��_����`�悷��
	//�i��Fx�̒��_�����R��y���R�̎��Ai��0�`1,3�`4�̎��Ɏl�p�`�|���S����`�悷��)
	for (int i = 0; i < totalPointNum - POINT_Y_NUM; i++)
	{
		// �����A i �� xNum - 1 * �s�� ��� �傫���Ȃ����� �s�����X�V
		if (i > POINT_X_NUM + (POINT_X_NUM * POINT_Y_NUM) * row - 1)
		{
			row++;	j += POINT_Y_NUM;
		}
		// i �� �O�̎��͖���
		if (i != 0)
		{
			// �����B i �� xNum - 1 + xNum * j ��i��������[�����Ȃ̂�i�X�V����
			if (i == (POINT_X_NUM - 1 + POINT_X_NUM * j))
			{
				i += (POINT_X_NUM * POINT_Y_NUM) - (POINT_X_NUM - 1);
			}
		}
		int a[4];
		// �����̒��_						// �E���̒��_
		a[0] = i;								a[1] = i + 1;
		// ����̒��_						// �E��̒��_
		a[2] = i + POINT_X_NUM * POINT_Y_NUM;	a[3] = i + POINT_X_NUM * POINT_Y_NUM + 1;
		// ��̖ʂ��쐬
		m_pointsIndices[BOTTOM].push_back(a[3]); m_pointsIndices[BOTTOM].push_back(a[2]);
		m_pointsIndices[BOTTOM].push_back(a[0]); m_pointsIndices[BOTTOM].push_back(a[1]);
		// ��̖ʂ��쐬
		int next = (POINT_X_NUM * POINT_Y_NUM) - POINT_X_NUM;
		// ��̖ʂ��쐬
		m_pointsIndices[TOP].push_back(a[0] + next); m_pointsIndices[TOP].push_back(a[1] + next);
		m_pointsIndices[TOP].push_back(a[3] + next); m_pointsIndices[TOP].push_back(a[2] + next);

		// �Ō�̒��_�ɂ������甲����
		if (a[3] == totalPointNum - POINT_X_NUM * POINT_Y_NUM + POINT_X_NUM - 1)
		{
			break;
		}
	}

	DeleteDuplication(BOTTOM);
	DeleteDuplication(TOP);
}

//------------------------------------------
// ���ʂ��쐬
//------------------------------------------
void SoftBodyBox::CreateLRFaces()
{
	int j = 0, row = 1;
	int totalPointNum = static_cast<int>(m_points.size());

	// �����@ xNum * yNum����1�񕪌��炵�����̒��_����`�悷��
	//�i��Fx�̒��_�����R��y���R�̎��Ai��0�`1,3�`4�̎��Ɏl�p�`�|���S����`�悷��)
	for (int i = 0; i < totalPointNum - POINT_X_NUM; i += POINT_X_NUM)
	{
		// �����A i �� xNum - 1 * �s�� ��� �傫���Ȃ����� �s�����X�V
		if (i > (POINT_X_NUM * POINT_Y_NUM) * row - 1)
		{
			row++; j += POINT_Y_NUM;
		}
		// i �� �O�̎��͖���
		if (i != 0)
		{
			// �����B i �� xNum - 1 + xNum * j ��i��������[�����Ȃ̂�i���X�V����
			if (i == (POINT_X_NUM * POINT_Y_NUM - POINT_X_NUM + POINT_X_NUM * j))
			{
				i += POINT_X_NUM;
			}
		}
		int a[4];
		// �����̒��_							// �E���̒��_
		a[0] = i;								a[1] = i + POINT_X_NUM;
		// ����̒��_							// �E��̒��_
		a[2] = i + POINT_X_NUM * POINT_Y_NUM;	a[3] = i + POINT_X_NUM * POINT_Y_NUM + POINT_X_NUM;
		// ���̖ʂ��쐬
		m_pointsIndices[Face::LEFT].push_back(a[3]); m_pointsIndices[Face::LEFT].push_back(a[2]);
		m_pointsIndices[Face::LEFT].push_back(a[0]); m_pointsIndices[Face::LEFT].push_back(a[1]);
		// �E�̖ʂ��쐬
		int next = POINT_X_NUM - 1;
		m_pointsIndices[Face::RIGHT].push_back(a[0] + next); m_pointsIndices[Face::RIGHT].push_back(a[1] + next);
		m_pointsIndices[Face::RIGHT].push_back(a[3] + next); m_pointsIndices[Face::RIGHT].push_back(a[2] + next);

		// �Ō�̒��_�ɂ������甲����
		if (a[3] == totalPointNum - POINT_X_NUM)
		{
			break;
		}
	}

	DeleteDuplication(LEFT);
	DeleteDuplication(RIGHT);
}

//------------------------------------------
// �d�����폜
//------------------------------------------
void SoftBodyBox::DeleteDuplication(const Face& face)
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
// ��]
//------------------------------------------
void SoftBodyBox::Rotate(
	DirectX::SimpleMath::Matrix rotation)
{
	for (int i = 0; i < m_points.size(); i++)
	{
		//// ��]�O�̈ʒu���L��
		//m_points[i].SetPrevPosition(GetPosition());

		// �ʒu����]
		m_points[i].SetPosition(
			DirectX::SimpleMath::Vector3::Transform(m_points[i].GetPosition(), rotation));
	}
	UpdateRenderPosition();

}

//------------------------------------------
// ���f���Ƃ̏Փ˔���
//------------------------------------------
void SoftBodyBox::CheckHitModel2Point(Imase::ObjCollision* pObj)
{
	DirectX::SimpleMath::Vector3 line = { 0.0f, -1.0f,0.0f };
	// �����W��
	float restitution = 0.8f;
	for (PhysicalPoint& p : m_points)
	{
		line.x = p.GetPosition().x;
		line.z = p.GetPosition().z;
		DirectX::SimpleMath::Vector3 hitPosition;
		DirectX::SimpleMath::Vector3 normal;
		// �����Ə��̌���������s��
		if (pObj->IntersectLineSegment(line, p.GetPosition(), &hitPosition, &normal))
		{
			// ���˃x�N�g�����v�Z
			DirectX::SimpleMath::Vector3 force = p.GetVector();
			force = -force * restitution;

			// �͍X�V
			p.UpdateVector(force);

			// �ʒu��ݒ�
			p.SetPosition(hitPosition);
		}
	}
}


//------------------------------------------
// UV�v�Z
//------------------------------------------
void SoftBodyBox::CalculateUV(const int& face)
{
	int index = 0;

	switch (face)
	{
	case TOP:
		for (int i = 0; i < POINT_Z_NUM; i++)
		{
			float y = i * m_uvOffset.z;

			for (int j = 0; j < POINT_X_NUM; j++)
			{
				DirectX::SimpleMath::Vector2 uv = { j * m_uvOffset.x, y };
				m_points[m_faceIndices[TOP][index]].SetUV(uv);
				m_points[m_faceIndices[BOTTOM][index]].SetUV(uv);
				index++;
			}
		}
		break;
	case LEFT:
		for (int i = 0; i < POINT_Z_NUM; i++)
		{
			float y = i * m_uvOffset.z;

			for (int j = 0; j < POINT_Y_NUM; j++)
			{
				DirectX::SimpleMath::Vector2 uv = { j * m_uvOffset.y, y };
				m_points[m_faceIndices[LEFT][index]].SetUV(uv);
				m_points[m_faceIndices[RIGHT][index]].SetUV(uv);
				index++;
			}
		}
		break;
	case FRONT:
		for (int i = 0; i < POINT_Y_NUM; i++)
		{
			float y = i * m_uvOffset.y;
			for (int j = 0; j < POINT_X_NUM; j++)
			{
				DirectX::SimpleMath::Vector2 uv = { j * m_uvOffset.x, y };
				m_points[m_faceIndices[FRONT][index]].SetUV(uv);
				m_points[m_faceIndices[BACK][index]].SetUV(uv);
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
void SoftBodyBox::CalculateIndex(const int& face)
{
	// ���_�̃C���f�b�N�X�ԍ����v�Z
	switch (face)
	{
	case TOP:
		m_renders[TOP]->CalculateIndex(POINT_X_NUM, POINT_Z_NUM);
		m_renders[BOTTOM]->SetIndices(m_renders[TOP]->GetIndices());
		break;
	case LEFT:
		m_renders[LEFT]->CalculateIndex(POINT_Y_NUM, POINT_Z_NUM);
		m_renders[RIGHT]->SetIndices(m_renders[LEFT]->GetIndices());
		break;
	case FRONT:
		m_renders[FRONT]->CalculateIndex(POINT_X_NUM, POINT_Y_NUM);
		m_renders[BACK]->SetIndices(m_renders[FRONT]->GetIndices());
		break;
	default:
		break;
	}
}


//------------------------------------------
// �p�����[�^��������
//------------------------------------------
void SoftBodyBox::ResetParameters(DirectX::SimpleMath::Vector3 position)
{
	for (auto& p : m_points)
	{
		p.ParameterReset(position);
	}
}

//------------------------------------------
// ���_����ݒ�
//------------------------------------------
void SoftBodyBox::SetPointsNum(const int& x, const int& y, const int& z)
{
	POINT_X_NUM = x;
	POINT_Y_NUM = y;
	POINT_Z_NUM = z;
}

//------------------------------------------
// �`��I�u�W�F�N�g�̐ݒ�
//------------------------------------------
void SoftBodyBox::SetRenders(const std::array<std::unique_ptr<RenderSoftBody>, Face::FACENUM>& renders)
{

	for (int i = 0; i < FACENUM; i++)
	{
		// �����_�[�̍쐬
		m_renders[i] = std::make_unique<RenderSoftBody>();

		renders[i]->Clone(m_renders[i].get(), m_vertices[i]);
	}
}

//------------------------------------------
// �F��ݒ�
//------------------------------------------
void SoftBodyBox::SetColor(const DirectX::SimpleMath::Color& color)
{
	// �F���ς����
	m_colorChanged = true;
	m_color = color;
}

//------------------------------------------
// �C���f�b�N�X��ݒ�
//------------------------------------------
void SoftBodyBox::SetFaceIndices(const std::array<std::vector<unsigned int>, Face::FACENUM>& faceIndices)
{
	for (int i = 0; i < Face::FACENUM; i++)
	{
		m_faceIndices[i] = faceIndices[i];
	}
}

//------------------------------------------
// ���_��ݒ�
//------------------------------------------
void SoftBodyBox::SetVertices(const std::array<std::vector<DirectX::VertexPositionColorTexture>, Face::FACENUM>& vertices)
{
	for (int i = 0; i < Face::FACENUM; i++)
	{
		m_vertices = vertices;
	}
}

//------------------------------------------
// ���_�̈ʒu��ݒ�
//------------------------------------------
void SoftBodyBox::SetPointsPosition()
{
	for (int i = 0; i < m_points.size(); i++)
	{
		DirectX::SimpleMath::Vector3 temp = m_points[i].GetPosition();
		m_points[i].SetPosition(m_points[i].GetPosition() + m_points[i].GetInitPosition());
	}
}

//------------------------------------------
// ���U���g�̏�����
//------------------------------------------
void SoftBodyBox::ResultReset(const DirectX::SimpleMath::Vector3& position)
{
	for (auto& p : m_points)
	{
		p.ResultReset(position);
	}
}
