//--------------------------------------------------------------------------------------
// File: ObjCollision.h
//
// Obj�`���̃��b�V�����R���W�����ɂ���N���X
//
// Date: 2018.7.11
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "Mylib/Collision.h"
#include "ImaseLib/DisplayCollision.h"

namespace Imase
{
	// Obj�`���̃��b�V�����R���W�����ɂ���N���X
	class ObjCollision
	{
	private:

		// �ʒu
		DirectX::SimpleMath::Vector3 m_position;

		// ��]
		DirectX::SimpleMath::Quaternion m_rotation;
		// �傫��
		float m_scale;

	private:

		// ���_���
		std::vector<DirectX::SimpleMath::Vector3> m_vertexes;

		// �C���f�b�N�X���
		std::vector<uint32_t> m_indexes;

		// �R���W�����p�O�p�`�f�[�^
		std::vector<Collision::Triangle> m_triangles;

		// �R���W�����p�O�p�`�f�[�^�̒ǉ��֐�
		void AddTriangle(DirectX::SimpleMath::Vector3 a, DirectX::SimpleMath::Vector3 b, DirectX::SimpleMath::Vector3 c);

	public:

		// �R���X�g���N�^
		ObjCollision(
			const wchar_t* fileName
			, const DirectX::SimpleMath::Vector3& position
			, const DirectX::SimpleMath::Quaternion& rotate
			, const float& scale
		);

		// �R���W�������̍X�V
		void UpdateBoundingInfo(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& rotate);

		// �����Ƃ̌�������֐�
		bool IntersectLineSegment(
			DirectX::SimpleMath::Vector3 a,
			DirectX::SimpleMath::Vector3 b,
			DirectX::SimpleMath::Vector3* hitPosition,
			DirectX::SimpleMath::Vector3* normal = nullptr
		);

		// �Փ˔���̕\���ɓo�^����֐�
		void AddDisplayCollision(Imase::DisplayCollision* displayCollision, DirectX::FXMVECTOR lineColor = DirectX::XMVECTORF32{ 0.0f, 0.0f, 0.0f, 0.0f }) const;

	};
}

