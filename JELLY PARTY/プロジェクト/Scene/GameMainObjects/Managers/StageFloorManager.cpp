//--------------------------------------------------------------------------------------
// File: StageFloorManager.cpp
//
// �X�e�[�W�̏�
//
// Date: 2023/10/24
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "StageFloorManager.h"

#include "MyLib/ObjectCollision.h"

// ���I�u�W�F�N�g�̃|�W�V����
const DirectX::SimpleMath::Vector3 StageFloorManager::POSITION = { 0.0f, -0.5f, 0.0f };

const float SCALE = 35.0f;

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
StageFloorManager::StageFloorManager(DX::DeviceResources* pDR)
{
	m_pDR = pDR;
	auto device = m_pDR->GetD3DDevice();
	auto context = m_pDR->GetD3DDeviceContext();

	// ��]�s����쐬
	DirectX::SimpleMath::Matrix rotation =
		DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(0.0f));

	// �X�e�[�W�̏����f�����擾
	DirectX::Model* dishModel = Resources::GetInstance()->GetModel(L"Floor");

	// �X�e�[�W�̃��f�����擾
	DirectX::Model* kitchenModel = Resources::GetInstance()->GetModel(L"Kitchen");

	// ���̈ʒu
	DirectX::SimpleMath::Vector3 position = POSITION;
	// ���̍쐬
	m_floor = std::make_unique<Floor>(m_pDR, dishModel, position, rotation, SCALE);

	// ���̍쐬
	m_stage = std::make_unique<Stage>(m_pDR, kitchenModel, position, rotation, SCALE);

	// �Փ˔���̕\���I�u�W�F�N�g�̍쐬
	m_displayCollision = std::make_unique<Imase::DisplayCollision>(device, context);
	// ���f���̃R���W�����쐬
	const wchar_t* path = L"Resources/CollisionMeshes/DishStage.obj";
	// �����蔻��̍쐬
	m_objCollisions.push_back(CreateCollider(path, DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f), rotation, 1.0f));
	// ���f���̓����蔻���ݒ�
	ObjectCollision::GetInstance()->SetModelCollision(&m_objCollisions[0]);
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
StageFloorManager::~StageFloorManager()
{
}

//------------------------------------------
// ������
//------------------------------------------
void StageFloorManager::Initialize()
{

}

//------------------------------------------
// �X�V
//------------------------------------------
void StageFloorManager::Update()
{
	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	m_floor->Update(elapsedTime, {0.0f, 0.0f ,0.0f});

	m_stage->Update(elapsedTime, {0.0f, 0.0f ,0.0f});
}

//------------------------------------------
// �`��
//------------------------------------------
void StageFloorManager::Render(
	ID3D11DeviceContext* context,
	DirectX::CommonStates* states,
	DirectX::SimpleMath::Matrix view,
	DirectX::SimpleMath::Matrix projection,
	std::function<void __cdecl()> function
)
{
	m_floor->Render(context, states, view, projection, function);

	m_stage->Render(context, states, view, projection, function);

	//// �Փ˔���̕\��
	//m_objCollisions[0].AddDisplayCollision(m_displayCollision.get());
	//m_displayCollision->DrawCollision(context, states, view, projection);

}


//------------------------------------------
// �I������
//------------------------------------------
void StageFloorManager::Finalize()
{

}

//------------------------------------------
// ���I�u�W�F�N�g�쐬
//------------------------------------------
std::unique_ptr<Floor> StageFloorManager::CreateFloor(
	DirectX::Model* model, 
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Matrix rotation,
	const float& scale
)
{
	std::unique_ptr<Floor> floor;
	// ���̃C���X�^���X����
	floor.reset(new Floor(m_pDR, model, position, rotation, scale));

	return floor;
}


//------------------------------------------
// ���̏Փ˔���쐬
//------------------------------------------
Imase::ObjCollision StageFloorManager::CreateCollider(
	const wchar_t* path,
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Matrix rotation,
	const float& scale
)
{
	// ��]�s����N�H�[�^�j�I���ɕϊ�
	DirectX::SimpleMath::Quaternion rotate = 
		DirectX::SimpleMath::Quaternion::CreateFromRotationMatrix(rotation);
	// �����蔻��I�u�W�F�N�g�̃C���X�^���X����
	Imase::ObjCollision collision(path, position, rotate, scale);

	return collision;
}
