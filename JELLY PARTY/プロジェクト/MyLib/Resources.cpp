//--------------------------------------------------------------------------------------
// File: Resources.h
//
// シングルトンの共通リソース
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
// Resourcesクラスのインスタンスを取得する
//------------------------------------------
Resources* const Resources::GetInstance()
{
	if (m_resources == nullptr)
	{
		// Resourcesクラスのインスタンスを生成する
		m_resources.reset(new Resources());
	}
	// Resourcesクラスのインスタンスを返す
	return m_resources.get();
}

//------------------------------------------
// コンストラクタ
//------------------------------------------
Resources::Resources()
	:
	m_modelsMap{}
	, m_texturesMap{}
	, m_soundsMap{}
	
{
}

//------------------------------------------
// デストラクタ
//------------------------------------------
Resources::~Resources()
{
	m_modelsMap.clear();
	m_texturesMap.clear();
	m_soundsMap.clear();
}

//------------------------------------------
// リソースをロードする
//------------------------------------------
void Resources::LoadResources(ID3D11Device1* device)
{
	LoadModels(device);
	LoadTextures(device);
}

//------------------------------------------
// モデル読み込み
//------------------------------------------
void Resources::LoadModels(ID3D11Device1* device)
{
	struct
	{
		// 関数内関数
		void operator()
			(
				ID3D11Device* device,
				const wchar_t* path,
				const wchar_t* modelName,
				std::unordered_map<const wchar_t*, std::unique_ptr<DirectX::Model>>* modelMap
				)
		{
			// エフェクト作成
			std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
			// ディレクトリを設定
			fx->SetDirectory(L"Resources/Models");

			// サウンドのインスタンス生成
			std::unique_ptr<DirectX::Model> model;

			// モデルを作成
			model = DirectX::Model::CreateFromCMO(device, path, *fx);

			modelMap->emplace(modelName,std::move(model));
		}
	} ModelLoader;

	// プレイヤーモデルの作成
	ModelLoader(device, L"Resources/Models/CatBall.cmo", L"Sphere", &m_modelsMap);
	//m_modelsMap.find(L"Sphere")->second->UpdateEffects([](DirectX::IEffect* effect)
	//	{
	//	}
	//);

	// プレイヤーの腕モデルの作成
	ModelLoader(device, L"Resources/Models/Arm.cmo", L"Arm", &m_modelsMap);
	m_modelsMap.find(L"Arm")->second->UpdateEffects([](DirectX::IEffect* effect)
		{
			//// ライトを切る(ラムダ式)
			//auto lights = dynamic_cast<DirectX::IEffectLights*>(effect);
			//if (lights)
			//{
			//	lights->SetLightEnabled(0, false);
			//	lights->SetLightEnabled(1, false);
			//	lights->SetLightEnabled(2, false);
			//	// 環境光を黒に
			//	lights->SetAmbientLightColor(DirectX::Colors::Black);
			//}
			//// 自己発光させる
			//auto basicEffect = dynamic_cast<DirectX::BasicEffect*>(effect);
			//if (basicEffect)
			//{
			//	basicEffect->SetEmissiveColor(DirectX::Colors::White);
			//}

		}
	);

	//// プレイヤーモデルの作成
	//ModelLoader(device, L"Resources/Models/CatBallArm.cmo", L"CatArm", &m_modelsMap);
	//m_modelsMap.find(L"CatArm")->second->UpdateEffects([](DirectX::IEffect* effect)
	//	{
	//	}
	//);

	// ステージの足場モデルの作成
	ModelLoader(device, L"Resources/Models/DishStage.cmo", L"Floor", &m_modelsMap);
	m_modelsMap.find(L"Floor")->second->UpdateEffects([](DirectX::IEffect* effect)
		{
		}
	);

	// ステージの足場モデルの作成
	ModelLoader(device, L"Resources/Models/Kitchen.cmo", L"Kitchen", &m_modelsMap);
	m_modelsMap.find(L"Kitchen")->second->UpdateEffects([](DirectX::IEffect* effect)
		{
		}
	);
}

//------------------------------------------
// 画像読み込み
//------------------------------------------
void Resources::LoadTextures(ID3D11Device1* device)
{
	struct
	{
		// 関数内関数
		void operator()
			(
				ID3D11Device* device,
				const wchar_t* path,
				const wchar_t* textureName,
				std::unordered_map<const wchar_t*, TextureTable>* textureMap
				)
		{
			// テーブルのインスタンス生成
			TextureTable textureTable;

			Microsoft::WRL::ComPtr<ID3D11Resource> resource;
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

			DX::ThrowIfFailed
			(
				// テクスチャを作成
				DirectX::CreateDDSTextureFromFile(device, path, &resource, &texture)
			);

			Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
			DX::ThrowIfFailed(resource.As(&tex));

			D3D11_TEXTURE2D_DESC desc;
			tex->GetDesc(&desc);

			// テクスチャ設定
			textureTable.texture = texture;
			// テクスチャサイズを設定
			textureTable.textureHeight = desc.Height;
			textureTable.textureWidth = desc.Width;

			textureMap->emplace(textureName, textureTable);
		}
	} TextureLoader;


	// タイトル画像ロード
	TextureLoader(device, L"Resources/Textures/Title.dds",L"Title", &m_texturesMap);
	// 背景画像ロード
	TextureLoader(device, L"Resources/Textures/BackGround.dds", L"BackGround", &m_texturesMap);
	// スペース押して画像ロード
	TextureLoader(device, L"Resources/Textures/PressSpace.dds", L"PressSpace", &m_texturesMap);
	// 遊び方画像ロード
	TextureLoader(device, L"Resources/Textures/HowToPlay.dds", L"HowToPlay", &m_texturesMap);
	// ローディング中のスプライト画像ロード
	TextureLoader(device, L"Resources/Textures/LoadingSprite.dds", L"LoadingSprite", &m_texturesMap);
	// ゼリーのテクスチャロード
	TextureLoader(device, L"Resources/Textures/OutlineJelly.dds", L"Jelly", &m_texturesMap);
	// 数字スプライトロード
	TextureLoader(device, L"Resources/Textures/NumberSprite.dds", L"NumberSprite", &m_texturesMap);
	// カウントダウンのスプライトロード
	TextureLoader(device, L"Resources/Textures/CountDownSprite.dds", L"CountDownSprite", &m_texturesMap);
	// カウントダウン後の表示文字画像ロード
	TextureLoader(device, L"Resources/Textures/DoWhat.dds", L"DoWhat", &m_texturesMap);
	// 終了文字画像ロード
	TextureLoader(device, L"Resources/Textures/Finished.dds", L"Finish", &m_texturesMap);
	// 終了文字画像ロード
	TextureLoader(device, L"Resources/Textures/Result.dds", L"Result", &m_texturesMap);
	// ステージ番号背景画像ロード
	TextureLoader(device, L"Resources/Textures/BaitNumBack.dds", L"BaitNumBack", &m_texturesMap);
	// ボタンベースロード
	TextureLoader(device, L"Resources/Textures/Button.dds", L"ButtonBase", &m_texturesMap);
	// 「スタート」画像ロード
	TextureLoader(device, L"Resources/Textures/Start.dds", L"Start", &m_texturesMap);
	// 「タイトルへ」画像ロード
	TextureLoader(device, L"Resources/Textures/ToTitle.dds", L"ToTitle", &m_texturesMap);
	// 「メニューへ」画像ロード
	TextureLoader(device, L"Resources/Textures/ToMenu.dds", L"ToMenu", &m_texturesMap);
	// 「もういちど」画像ロード
	TextureLoader(device, L"Resources/Textures/Restart.dds", L"ReStart", &m_texturesMap);
	// 「おわる」画像ロード
	TextureLoader(device, L"Resources/Textures/Quit.dds", L"Quit", &m_texturesMap);
	// 「メインメニュー」画像ロード
	TextureLoader(device, L"Resources/Textures/MainMenu.dds", L"MainMenu", &m_texturesMap);
	// 煙エフェクト画像ロード
	TextureLoader(device, L"Resources/Textures/Smoke.dds", L"Smoke", &m_texturesMap);
	// パーティクル画像ロード
	TextureLoader(device, L"Resources/Textures/Particle.dds", L"Particle", &m_texturesMap);
	// ゼリーのスコア画像ロード
	TextureLoader(device, L"Resources/Textures/Bait.dds", L"Bait", &m_texturesMap);
	// 「ハイスコア！」画像ロード
	TextureLoader(device, L"Resources/Textures/HighScore.dds", L"HighScore", &m_texturesMap);
}

//------------------------------------------
// サウンド読み込み
//------------------------------------------
void Resources::LoadSounds(DirectX::WaveBank* waveBank)
{
	// ボタン音
	m_soundsMap.emplace(L"Button", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_BUTTON));
	// ヒット音
	m_soundsMap.emplace(L"HitJelly", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_HITJELLY));
	// 選択音
	m_soundsMap.emplace(L"Select", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_SELECTMENU));
	// 終了音
	m_soundsMap.emplace(L"Finish", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_FINISH));
	// 終了のゴング音
	m_soundsMap.emplace(L"FinishGong", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_FINISHGONG));
	// 拍手音
	m_soundsMap.emplace(L"Clap", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_CLAP));
	// 衝撃音
	m_soundsMap.emplace(L"Impact", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_IMPACT));
	// ジャンプ音
	m_soundsMap.emplace(L"Jump", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_JUMP));
	// ハイスコア音
	m_soundsMap.emplace(L"HighScore", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_HIGHSCORE));

	// タイトルのBGM
	m_soundsMap.emplace(L"TitleBGM", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_TITLEBGM));
	// ゲームのBGM
	m_soundsMap.emplace(L"PlayBGM", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_PLAYBGM));
	// リザルトのBGM
	m_soundsMap.emplace(L"ResultBGM", waveBank->CreateInstance(XACT_WAVEBANK_AUDIO_RESULTBGM));
}

//------------------------------------------
// jsonファイルの読み込み
//------------------------------------------
void Resources::ReadJsonFile(nlohmann::json* jsonData, const std::wstring& fileName)
{
	std::wstring jsonFileName = fileName + L".json";

	// JSONデータの解析
	std::ifstream fileData(jsonFileName);
	*jsonData = nlohmann::json::parse(fileData);
}

//------------------------------------------
// jsonファイルの書き込み
//------------------------------------------
void Resources::WriteJsonFile(const nlohmann::json& jsonData, const std::wstring& fileName)
{
	// ファイルに書き込む
	std::ofstream file(fileName + L".json");
	file << jsonData << std::endl;
	file.close();
}
