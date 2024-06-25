//--------------------------------------------------------------------------------------
// File: Resources.h
//
// �V���O���g���̋��ʃ��\�[�X
//
//
// Last Update : 2023.10.23
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Resources.h"
#include "Resources/Sounds/SoundIdx.h"
#include <fstream>

std::unique_ptr<Resources> Resources::m_resources = nullptr;

//------------------------------------------
// Resources�N���X�̃C���X�^���X���擾����
//------------------------------------------
Resources* const Resources::GetInstance()
{
	if (m_resources == nullptr)
	{
		// Resources�N���X�̃C���X�^���X�𐶐�����
		m_resources.reset(new Resources());
	}
	// Resources�N���X�̃C���X�^���X��Ԃ�
	return m_resources.get();
}

//------------------------------------------
// �R���X�g���N�^
//------------------------------------------
Resources::Resources()
	:
	m_modelsMap{}
	, m_texturesMap{}
	, m_soundsMap{}
	
{
}

//------------------------------------------
// �f�X�g���N�^
//------------------------------------------
Resources::~Resources()
{
	m_modelsMap.clear();
	m_texturesMap.clear();
	m_soundsMap.clear();
}

//------------------------------------------
// ���\�[�X�����[�h����
//------------------------------------------
void Resources::LoadResources(ID3D11Device1* device)
{
	LoadModels(device);
	LoadTextures(device);
}

//------------------------------------------
// ���f���ǂݍ���
//------------------------------------------
void Resources::LoadModels(ID3D11Device1* device)
{
	struct
	{
		// �֐����֐�
		void operator()
			(
				ID3D11Device* device,
				const wchar_t* path,
				const wchar_t* modelName,
				std::unordered_map<const wchar_t*, std::unique_ptr<DirectX::Model>>* modelMap
				)
		{
			// �G�t�F�N�g�쐬
			std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
			// �f�B���N�g����ݒ�
			fx->SetDirectory(L"Resources/Models");

			// �T�E���h�̃C���X�^���X����
			std::unique_ptr<DirectX::Model> model;

			// ���f�����쐬
			model = DirectX::Model::CreateFromCMO(device, path, *fx);

			modelMap->emplace(modelName,std::move(model));
		}
	} ModelLoader;

	// �v���C���[���f���̍쐬
	ModelLoader(device, L"Resources/Models/CatBall.cmo", L"Sphere", &m_modelsMap);
	//m_modelsMap.find(L"Sphere")->second->UpdateEffects([](DirectX::IEffect* effect)
	//	{
	//	}
	//);

	// �v���C���[�̘r���f���̍쐬
	ModelLoader(device, L"Resources/Models/Arm.cmo", L"Arm", &m_modelsMap);
	m_modelsMap.find(L"Arm")->second->UpdateEffects([](DirectX::IEffect* effect)
		{
			//// ���C�g��؂�(�����_��)
			//auto lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			//if (lights)
			//{
			//	lights->SetLightEnabled(0, false);
			//	lights->SetLightEnabled(1, false);
			//	lights->SetLightEnabled(2, false);
			//	// ����������
			//	lights->SetAmbientLightColor(DirectX::Colors::Black);
			//}
			//// ���Ȕ���������
			//auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			//if (basicEffect)
			//{
			//	basicEffect->SetEmissiveColor(DirectX::Colors::White);
			//}

		}
	);

	//// �v���C���[���f���̍쐬
	//ModelLoader(device, L"Resources/Models/CatBallArm.cmo", L"CatArm", &m_modelsMap);
	//m_modelsMap.find(L"CatArm")->second->UpdateEffects([](DirectX::IEffect* effect)
	//	{
	//	}
	//);

	// �X�e�[�W�̑��ꃂ�f���̍쐬
	ModelLoader(device, L"Resources/Models/DishStage.cmo", L"Floor", &m_modelsMap);
	m_modelsMap.find(L"Floor")->second->UpdateEffects([](DirectX::IEffect* effect)
		{
		}
	);

	// �X�e�[�W�̑��ꃂ�f���̍쐬
	ModelLoader(device, L"Resources/Models/Kitchen.cmo", L"Kitchen", &m_modelsMap);
	m_modelsMap.find(L"Kitchen")->second->UpdateEffects([](DirectX::IEffect* effect)
		{
		}
	);
}

//------------------------------------------
// �摜�ǂݍ���
//------------------------------------------
void Resources::LoadTextures(ID3D11Device1* device)
{
	struct
	{
		// �֐����֐�
		void operator()
			(
				ID3D11Device* device,
				const wchar_t* path,
				const wchar_t* textureName,
				std::unordered_map<const wchar_t*, TextureTable>* textureMap
				)
		{
			// �e�[�u���̃C���X�^���X����
			TextureTable textureTable;

			Microsoft::WRL::ComPtr<ID3D11Resource> resource;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

			DX::ThrowIfFailed
			(
				// �e�N�X�`�����쐬
				DirectX::CreateDDSTextureFromFile(device, path, &resource, &texture)
			);

			Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
			DX::ThrowIfFailed(resource.As(&tex));

			D3D11_TEXTURE2D_DESC desc;
			tex->GetDesc(&desc);

			// �e�N�X�`���ݒ�
			textureTable.texture = texture;
			// �e�N�X�`���T�C�Y��ݒ�
			textureTable.textureHeight = desc.Height;
			textureTable.textureWidth = desc.Width;

			textureMap->emplace(textureName, textureTable);
		}
	} TextureLoader;


	// �^�C�g���摜���[�h
	TextureLoader(device, L"Resources/Textures/Title.dds",L"Title", &m_texturesMap);
	// �w�i�摜���[�h
	TextureLoader(device, L"Resources/Textures/BackGround.dds", L"BackGround", &m_texturesMap);
	// �X�y�[�X�����ĉ摜���[�h
	TextureLoader(device, L"Resources/Textures/PressSpace.dds", L"PressSpace", &m_texturesMap);
	// �V�ѕ��摜���[�h
	TextureLoader(device, L"Resources/Textures/HowToPlay.dds", L"HowToPlay", &m_texturesMap);
	// ���[�f�B���O���̃X�v���C�g�摜���[�h
	TextureLoader(device, L"Resources/Textures/LoadingSprite.dds", L"LoadingSprite", &m_texturesMap);
	// �[���[�̃e�N�X�`�����[�h
	TextureLoader(device, L"Resources/Textures/OutlineJelly.dds", L"Jelly", &m_texturesMap);
	// �����X�v���C�g���[�h
	TextureLoader(device, L"Resources/Textures/NumberSprite.dds", L"NumberSprite", &m_texturesMap);
	// �J�E���g�_�E���̃X�v���C�g���[�h
	TextureLoader(device, L"Resources/Textures/CountDownSprite.dds", L"CountDownSprite", &m_texturesMap);
	// �J�E���g�_�E����̕\�������摜���[�h
	TextureLoader(device, L"Resources/Textures/DoWhat.dds", L"DoWhat", &m_texturesMap);
	// �I�������摜���[�h
	TextureLoader(device, L"Resources/Textures/Finished.dds", L"Finish", &m_texturesMap);
	// �I�������摜���[�h
	TextureLoader(device, L"Resources/Textures/Result.dds", L"Result", &m_texturesMap);
	// �X�e�[�W�ԍ��w�i�摜���[�h
	TextureLoader(device, L"Resources/Textures/BaitNumBack.dds", L"BaitNumBack", &m_texturesMap);
	// �{�^���x�[�X���[�h
	TextureLoader(device, L"Resources/Textures/Button.dds", L"ButtonBase", &m_texturesMap);
	// �u�X�^�[�g�v�摜���[�h
	TextureLoader(device, L"Resources/Textures/Start.dds", L"Start", &m_texturesMap);
	// �u�^�C�g���ցv�摜���[�h
	TextureLoader(device, L"Resources/Textures/ToTitle.dds", L"ToTitle", &m_texturesMap);
	// �u���j���[�ցv�摜���[�h
	TextureLoader(device, L"Resources/Textures/ToMenu.dds", L"ToMenu", &m_texturesMap);
	// �u���������ǁv�摜���[�h
	TextureLoader(device, L"Resources/Textures/Restart.dds", L"ReStart", &m_texturesMap);
	// �u�����v�摜���[�h
	TextureLoader(device, L"Resources/Textures/Quit.dds", L"Quit", &m_texturesMap);
	// �u���C�����j���[�v�摜���[�h
	TextureLoader(device, L"Resources/Textures/MainMenu.dds", L"MainMenu", &m_texturesMap);
	// ���G�t�F�N�g�摜���[�h
	TextureLoader(device, L"Resources/Textures/Smoke.dds", L"Smoke", &m_texturesMap);
	// �p�[�e�B�N���摜���[�h
	TextureLoader(device, L"Resources/Textures/Particle.dds", L"Particle", &m_texturesMap);
	// �[���[�̃X�R�A�摜���[�h
	TextureLoader(device, L"Resources/Textures/Bait.dds", L"Bait", &m_texturesMap);
	// �u�n�C�X�R�A�I�v�摜���[�h
	TextureLoader(device, L"Resources/Textures/HighScore.dds", L"HighScore", &m_texturesMap);
}

//------------------------------------------
// �T�E���h�ǂݍ���
//------------------------------------------
void Resources::LoadSounds(DirectX::WaveBank* waveBank)
{
	// �{�^����
	m_soundsMap.emplace(L"Button", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_BUTTON));
	// �q�b�g��
	m_soundsMap.emplace(L"HitJelly", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_HITJELLY));
	// �I����
	m_soundsMap.emplace(L"Select", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_SELECTMENU));
	// �I����
	m_soundsMap.emplace(L"Finish", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_FINISH));
	// �I���̃S���O��
	m_soundsMap.emplace(L"FinishGong", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_FINISHGONG));
	// ���艹
	m_soundsMap.emplace(L"Clap", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_CLAP));
	// �Ռ���
	m_soundsMap.emplace(L"Impact", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_IMPACT));
	// �W�����v��
	m_soundsMap.emplace(L"Jump", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_JUMP));
	// �n�C�X�R�A��
	m_soundsMap.emplace(L"HighScore", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_HIGHSCORE));

	// �^�C�g����BGM
	m_soundsMap.emplace(L"TitleBGM", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_TITLEBGM));
	// �Q�[����BGM
	m_soundsMap.emplace(L"PlayBGM", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_PLAYBGM));
	// ���U���g��BGM
	m_soundsMap.emplace(L"ResultBGM", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_RESULTBGM));
}

//------------------------------------------
// json�t�@�C���̓ǂݍ���
//------------------------------------------
void Resources::ReadJsonFile(nlohmann::json* jsonData, const std::wstring& fileName)
{
	std::wstring jsonFileName = fileName + L".json";

	// JSON�f�[�^�̉��
	std::ifstream fileData(jsonFileName);
	*jsonData = nlohmann::json::parse(fileData);
}

//------------------------------------------
// json�t�@�C���̏�������
//------------------------------------------
void Resources::WriteJsonFile(const nlohmann::json& jsonData, const std::wstring& fileName)
{
	// �t�@�C���ɏ�������
	std::ofstream file(fileName + L".json");
	file << jsonData << std::endl;
	file.close();
}
