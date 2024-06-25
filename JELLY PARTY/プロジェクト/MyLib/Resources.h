//--------------------------------------------------------------------------------------
// File: Resources.h
//
// �V���O���g���̋��ʃ��\�[�X
//
// Last Update: 2023.10.23
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#pragma once

#ifndef RESOURCES_DEFINED
#define RESOURCES_DEFINED

#include "Model.h"
#include "Audio.h"
#include <unordered_map>
#include "nlohmann/json.hpp"

// Resources�N���X���`����
class Resources
{
public:

	// �e�N�X�`�����
	struct TextureTable
	{
		// �e�N�X�`���T�C�Y
		int textureWidth = 0;
		int textureHeight = 0;
		// �e�N�X�`��
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr;
	};

public:
	// Resouces�N���X�̃C���X�^���X���擾����
	static Resources* const GetInstance();

	// �f�X�g���N�^
	~Resources();

	// ���\�[�X�����[�h����
	void LoadResources(ID3D11Device1* device);
	// �T�E���h�����[�h����
	void LoadSounds(DirectX::WaveBank* waveBank);

	void ReadJsonFile(nlohmann::json* jsonData, const std::wstring& fileName);

	void WriteJsonFile(const nlohmann::json& jsonData, const std::wstring& fileName);

private:

	// ���f�����[�h
	void LoadModels(ID3D11Device1* device);
	// �e�N�X�`�����[�h
	void LoadTextures(ID3D11Device1* device);

public:	// ���f���Q�b�^�[

	// ���f�����擾
	DirectX::Model* GetModel(const wchar_t* name) const {
		return m_modelsMap.find(name)->second.get();
	}

	// �e�N�X�`�������擾
	TextureTable GetTexture(const wchar_t* name) const {
		return m_texturesMap.find(name)->second; 
	}

	// �T�E���h���擾
	DirectX::SoundEffectInstance* GetSound(const wchar_t* name) const {
		return m_soundsMap.find(name)->second.get();
	}

private:

	// �R���X�g���N�^
	Resources();
	// ����͋��e���Ȃ�
	void operator=(const Resources& object) = delete;
	// �R�s�[�R���X�g���N�^�͋��e���Ȃ�
	Resources(const Resources& object) = delete;
	
private:

	// Resources�N���X�̃C���X�^���X�ւ̃|�C���^
	static std::unique_ptr<Resources> m_resources;

private:

	// �e�N�X�`���̃}�b�v
	std::unordered_map<const wchar_t*, TextureTable> m_texturesMap;

	// �T�E���h�̃}�b�v
	std::unordered_map<const wchar_t*, std::unique_ptr<DirectX::SoundEffectInstance>> m_soundsMap;

	// ���f���̃}�b�v
	std::unordered_map<const wchar_t*, std::unique_ptr<DirectX::Model>> m_modelsMap;

};

#endif // RESOURCES_DEFINED