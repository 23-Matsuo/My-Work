//--------------------------------------------------------------------------------------
// File: Resources.h
//
// シングルトンの共通リソース
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

// Resourcesクラスを定義する
class Resources
{
public:

	// テクスチャ情報
	struct TextureTable
	{
		// テクスチャサイズ
		int textureWidth = 0;
		int textureHeight = 0;
		// テクスチャ
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = nullptr;
	};

public:
	// Resoucesクラスのインスタンスを取得する
	static Resources* const GetInstance();

	// デストラクタ
	~Resources();

	// リソースをロードする
	void LoadResources(ID3D11Device1* device);
	// サウンドをロードする
	void LoadSounds(DirectX::WaveBank* waveBank);

	void ReadJsonFile(nlohmann::json* jsonData, const std::wstring& fileName);

	void WriteJsonFile(const nlohmann::json& jsonData, const std::wstring& fileName);

private:

	// モデルロード
	void LoadModels(ID3D11Device1* device);
	// テクスチャロード
	void LoadTextures(ID3D11Device1* device);

public:	// モデルゲッター

	// モデルを取得
	DirectX::Model* GetModel(const wchar_t* name) const {
		return m_modelsMap.find(name)->second.get();
	}

	// テクスチャ情報を取得
	TextureTable GetTexture(const wchar_t* name) const {
		return m_texturesMap.find(name)->second; 
	}

	// サウンドを取得
	DirectX::SoundEffectInstance* GetSound(const wchar_t* name) const {
		return m_soundsMap.find(name)->second.get();
	}

private:

	// コンストラクタ
	Resources();
	// 代入は許容しない
	void operator=(const Resources& object) = delete;
	// コピーコンストラクタは許容しない
	Resources(const Resources& object) = delete;
	
private:

	// Resourcesクラスのインスタンスへのポインタ
	static std::unique_ptr<Resources> m_resources;

private:

	// テクスチャのマップ
	std::unordered_map<const wchar_t*, TextureTable> m_texturesMap;

	// サウンドのマップ
	std::unordered_map<const wchar_t*, std::unique_ptr<DirectX::SoundEffectInstance>> m_soundsMap;

	// モデルのマップ
	std::unordered_map<const wchar_t*, std::unique_ptr<DirectX::Model>> m_modelsMap;

};

#endif // RESOURCES_DEFINED