//--------------------------------------------------------------------------------------
// File: GameCamera.h
//
// �Q�[���J�����N���X
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#pragma once
#ifndef GAME_CAMERA_DEFINED
#define GAME_CAMERA_DEFINED

#include "MyLib/Camera.h"
#include "MyLib/DebugCamera.h"

class GameCamera : public Camera
{
public:
	// �J�����̃^�C�v
	enum class Type
	{
		GAMEMAIN,
		DEAD,
		RESULT,

		DEBUG,

	};
public:
	// �R���X�g���N�^
	GameCamera(RECT rect);
	// �f�X�g���N�^
	~GameCamera();

	// �X�V
	void Update(float elapsedTime);

public:
	// �v���C���[�̃|�W�V�����ݒ�
	void SetPlayerPosition(const DirectX::SimpleMath::Vector3& pos)	{ m_playerPos = pos; }
	
	// �J�����̃^�C�v��ݒ肷��
	void SetType(Type type) { m_type = type; }
public:

	// �ˉe�s����擾
	DirectX::SimpleMath::Matrix GetProjection() const { return m_projection; }
private:

	// ���݂̃J�����^�C�v
	Type m_type;

	// �f�o�b�O�J�����ւ̃|�C���^
	std::unique_ptr<DebugCamera> m_debugCamera;

	// �v���C���[�̈ʒu
	DirectX::SimpleMath::Vector3 m_playerPos;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;

	float m_vel;

};

#endif // GAME_CAMERA_DEFINED