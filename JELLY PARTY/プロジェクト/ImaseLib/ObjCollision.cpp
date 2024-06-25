//--------------------------------------------------------------------------------------
// File: CollisionMesh.cpp
//
// Obj�`���̃��b�V�����R���W�����ɂ���N���X
//
// Date: 2018.7.11
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "ObjCollision.h"
#include <fstream>

using namespace DirectX;
using namespace Imase;

// �R���X�g���N�^
ObjCollision::ObjCollision(
	const wchar_t* fileName
	, const DirectX::SimpleMath::Vector3& position
	, const DirectX::SimpleMath::Quaternion& rotate
	, const float& scale

)
{
	// obj�`���̃t�@�C���ǂݍ���
	std::ifstream ifs(fileName);

	std::string str;
	while (getline(ifs, str))
	{
		// ���_
		if (str[0] == 'v')
		{
			SimpleMath::Vector3 val;
			sscanf_s(str.data(), "v  %f %f %f", &val.x, &val.y, &val.z);
			m_vertexes.push_back(val);
		}
		// �C���f�b�N�X
		if (str[0] == 'f')
		{
			int a, b, c;
			sscanf_s(str.data(), "f %d %d %d", &a, &b, &c);
			// �O�p�`�̒��_�C���f�b�N�X�ԍ������������܂ޏꍇ�͖�������
			if (a != b && a != c && b != c)
			{
				m_indexes.push_back(a - 1);
				m_indexes.push_back(c - 1);
				m_indexes.push_back(b - 1);
			}
		}
	}
	ifs.close();

	// �O�p�`���X�g�ɓo�^
	for (size_t i = 0; i < m_indexes.size() / 3; i++)
	{
		AddTriangle(m_vertexes[m_indexes[i * 3]], m_vertexes[m_indexes[i * 3 + 1]], m_vertexes[m_indexes[i * 3 + 2]]);
	}

	m_position = position;
	m_rotation = rotate;
	m_scale = scale;

}

// �R���W�����p�O�p�`�f�[�^�̒ǉ��֐�
void ObjCollision::AddTriangle(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, DirectX::SimpleMath::Vector3 c)
{
	Collision::Triangle t(a, b, c);
	m_triangles.push_back(t);
}

// �R���W�������̍X�V
void ObjCollision::UpdateBoundingInfo(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& rotate)
{
	m_position = position;
	m_rotation = rotate;
}

// �����Ƃ̌�������֐�
bool ObjCollision::IntersectLineSegment(
	DirectX::SimpleMath::Vector3 a,
	DirectX::SimpleMath::Vector3 b,
	DirectX::SimpleMath::Vector3* hitPosition,
	DirectX::SimpleMath::Vector3* normal
)
{
	// �����ɋt�s����|����
	SimpleMath::Matrix scale = SimpleMath::Matrix::CreateScale(m_scale);
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(m_rotation);
	SimpleMath::Matrix trans = SimpleMath::Matrix::CreateTranslation(m_position);
	SimpleMath::Matrix world = scale * rotate * trans;
	SimpleMath::Matrix matInvert = world.Invert();
	a = SimpleMath::Vector3::Transform(a, matInvert);
	b = SimpleMath::Vector3::Transform(b, matInvert);

	for (int i = 0; i < m_triangles.size(); i++)
	{
		if (Collision::IntersectSegmentTriangle(a, b, m_triangles[i], hitPosition) == true)
		{
			// �Փˈʒu�����[���h���W�n�֕ϊ�
			*hitPosition = SimpleMath::Vector3::Transform(*hitPosition, world);
			// �Փ˂����O�p�`�̖@�������擾
			if (normal) *normal = SimpleMath::Vector3::Transform(m_triangles[i].p.Normal(), rotate);
			return true;
		}
	}
	return false;
}

void ObjCollision::AddDisplayCollision(Imase::DisplayCollision* displayCollision, DirectX::FXMVECTOR lineColor) const
{
	displayCollision->AddBoundingVolume(m_vertexes, m_indexes, m_position, m_rotation, lineColor);
}
