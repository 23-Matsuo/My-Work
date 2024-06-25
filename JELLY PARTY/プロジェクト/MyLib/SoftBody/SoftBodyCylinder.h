//--------------------------------------------------------------------------------------
// File: SoftBodyCylinder.h
//
// �~���\�t�g�{�f�B�N���X
//
// 
// Last Update: 2023.11.14
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef SOFTBODY_CYLINDER_DEFINED
#define SOFTBODY_CYLINDER_DEFINED

#include "MyLib/PhysicalObject.h"
#include "Spring.h"
#include "RenderSoftBody.h"

#include "ImaseLib/ObjCollision.h"
#include <array>

class SoftBodyCylinder : public PhysicalObject
{
public:

	// �G�C���A�X�錾
	using Points = std::vector<PhysicalPoint>;

public:
	enum class STATE
	{
		FIXED,
		UNFIXED
	};

	// �_��X�������̐�
	int POINT_X_NUM = 8;
	// �_��Y��������
	int POINT_Y_NUM = 2;

	enum Face
	{
		BOTTOM,
		TOP,
		SIDE,

		FACENUM
	};

public:

	// �R���X�g���N�^
	SoftBodyCylinder(
		const int& xNum, const int& yNum,
		const float& radius,
		const float& height,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotation,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Color& color,
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED
	);

	// �f�X�g���N�^
	~SoftBodyCylinder() {};

	// ������
	void Intialize(ID3D11ShaderResourceView* texture);

	// �X�V
	void Update(const DX::StepTimer& timer) override;

	// ��]
	void Rotate(DirectX::SimpleMath::Matrix rotation);

	// �`��
	void Render();

	// �e�`��
	void ShadowRender(
		const DirectX::SimpleMath::Matrix& lightView,
		const DirectX::SimpleMath::Matrix& lightProjection
	);

public:

	// �`�����
	void CreateFigure();

public:	// �Q�b�^�[

	// ���_�擾
	Points* GetPoints()									{ return &m_points; }
	// ��]�擾
	const DirectX::SimpleMath::Quaternion GetRotation() { return m_rotation; }
	// �������Ă��邩�ǂ����擾
	const bool GetIsHit() 								{ return m_isHit; }

	const int GetXNum()									{ return POINT_X_NUM; }
	const int GetYNum()									{ return POINT_Y_NUM; }

	const DirectX::SimpleMath::Color GetColor()			{ return m_color; }
private:

	void UpdateRenderPosition();

	// �{�f�B�̏�Ԃ��m�F
	void CheckBodyState();

	// �_�Ɠ_�̊Ԃ̂΂˂����
	void CreateArms();

	// ���̖ʂ��쐬
	void CreateFaces();

	void CreateSideFaces();
	void CreateTBFaces();

	// �d�����폜
	void DeleteDuplication(const Face& face);
	// UV���W�̌v�Z
	void CalculateUV(const int& face);

	// UV���W�̌v�Z
	void CalculateIndex(const int& face);


private:

	// ���_�̃R���e�i
	Points m_points;

	// �΂˂̃R���e�i
	std::vector<Spring> m_springs;

	// �ʂ��\�����钸�_�ԍ����i�[����
	std::array<std::vector<unsigned int>, Face::FACENUM> m_pointsIndices;

	// �ʂ��\�����钸�_�ԍ��̏d�������������̂��i�[����
	std::array<std::vector<unsigned int>, Face::FACENUM> m_faceIndices;

	// �[���[�̖ʕ`��p�N���X�ւ̃|�C���^ * 6
	std::array<std::unique_ptr<RenderSoftBody>, Face::FACENUM> m_renders;
private:

	// ���
	PhysicalPoint::STATE m_state;

	// �\�t�g�{�f�B�̃|�W�V����
	DirectX::SimpleMath::Vector3 m_position;

	// �����|�W�V����
	DirectX::SimpleMath::Vector3 m_initPosition;

	// ��]�p
	DirectX::SimpleMath::Quaternion m_rotation;

	// �傫��
	DirectX::SimpleMath::Vector3  m_scale;

	// ���a
	float m_radius;
	// ����
	float m_height;

	// �F
	DirectX::SimpleMath::Color m_color;

	// ��������
	bool m_isHit;

	// UV�̃I�t�Z�b�g
	DirectX::SimpleMath::Vector3 m_uvOffset;
};

#endif	// SOFTBODY_CYLINDER_DEFINED