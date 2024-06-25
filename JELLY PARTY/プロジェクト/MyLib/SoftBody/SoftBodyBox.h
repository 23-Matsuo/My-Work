//--------------------------------------------------------------------------------------
// File: SoftBodyBox.h
//
// �\�t�g�{�f�B�̊Ǘ��N���X
//
// 
// Last Update: 2023.10.21
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef SOFTBODY_BOX_DEFINED
#define SOFTBODY_BOX_DEFINED

#include "MyLib/PhysicalObject.h"
#include "Spring.h"
#include "RenderSoftBody.h"

#include "ImaseLib/ObjCollision.h"
#include <array>

class SoftBodyBox : public PhysicalObject
{
public:

	// �G�C���A�X�錾
	using Points = std::vector<PhysicalPoint>;

public:
	enum class STATE
	{
		FIXED,
		UNFIXED,
		CORNERFIXED,
	};

	// �_��X�������̐�
	int POINT_X_NUM = 3;
	// �_��Y��������
	int POINT_Y_NUM = 3;
	// �_��Z�������̐�
	int POINT_Z_NUM = 3;

	enum Face
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		FRONT,
		BACK,

		FACENUM
	};

public:

	// �R���X�g���N�^
	SoftBodyBox();

	// �R���X�g���N�^
	SoftBodyBox(
		const int& xNum,
		const int& yNum,
		const int& zNum,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotation,
		const DirectX::SimpleMath::Vector3& scale,
		const DirectX::SimpleMath::Color& color,
		const PhysicalPoint::STATE& state = PhysicalPoint::STATE::FIXED
	);

	// �f�X�g���N�^
	~SoftBodyBox() {};

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

	// �N���[��
	void Clone(SoftBodyBox* other)
	{
		other->SetPointsNum(POINT_X_NUM, POINT_Y_NUM, POINT_Z_NUM);
		other->SetState(m_state);
		other->SetPosition(m_position);
		other->SetRotation(m_rotation);
		other->SetPoints(m_points);
		other->CreateArms();
		other->SetVertices(m_vertices);
		other->SetColor(m_color);
		other->SetRenders(m_renders);
		other->SetFaceIndices(m_faceIndices);
	}

public:

	// �`�����
	void CreateFigure();

	// ���f���Ƃ̏Փ˔���
	void CheckHitModel2Point(Imase::ObjCollision* pObj);

	// �ʒu��������
	void ResetParameters(DirectX::SimpleMath::Vector3 position);

	// ���U���g�̏�����
	void ResultReset(const DirectX::SimpleMath::Vector3& position);

public:

	// ���_��ݒ�
	void SetPointsNum(const int& x, const int& y, const int& z);
	// ���_��ݒ�
	void SetPoints(Points points) { m_points = points; }
	// �΂˂�ݒ�
	void SetSprings(std::vector<Spring> springs) { m_springs = springs; }
	// ��]��ݒ�
	void SetRotation(const DirectX::SimpleMath::Quaternion& rotation) { m_rotation = rotation; }
	// �F��ݒ�
	void SetColor(const DirectX::SimpleMath::Color& color);
	// �C���f�b�N�X��ݒ�
	void SetFaceIndices(const std::array<std::vector<unsigned int>, Face::FACENUM>& faceIndices);
	// ���_��ݒ�
	void SetVertices(const std::array<std::vector<DirectX::VertexPositionColorTexture>, Face::FACENUM>& vertices);
	// ���_�̈ʒu��ݒ�
	void SetPointsPosition();
	// ��Ԃ�ݒ�
	void SetState(const PhysicalPoint::STATE& state) { m_state = state; }
	// �`��̐ݒ�
	void SetRenders(const std::array<std::unique_ptr<RenderSoftBody>, Face::FACENUM>& renders);

public:	// �Q�b�^�[

	// ���_�擾
	Points* GetPoints()											{ return& m_points; }
	// ��]�擾
	const DirectX::SimpleMath::Quaternion& GetRotation()		{ return m_rotation; }
	// �������Ă��邩�ǂ����擾
	const DirectX::SimpleMath::Color& GetColor()				{ return m_color; }

	const DirectX::SimpleMath::Vector3& GetInitialPosition()	{ return m_initPosition; }

	// �X�P�[���擾
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }

private:

	void UpdateRenderPosition();

	// �_�Ɠ_�̊Ԃ̂΂˂����
	void CreateArms();

	// ���̖ʂ��쐬
	void CreateFaces();

	void CreateFBFaces();
	void CreateTBFaces();
	void CreateLRFaces();

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
	
	// �`�悷�钸�_
	std::array<std::vector<DirectX::VertexPositionColorTexture>, Face::FACENUM> m_vertices;

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


	// �F
	DirectX::SimpleMath::Color m_color;


	// UV�̃I�t�Z�b�g
	DirectX::SimpleMath::Vector3 m_uvOffset;

	bool m_colorChanged;
};

#endif	// SOFTBODY_BOX_DEFINED