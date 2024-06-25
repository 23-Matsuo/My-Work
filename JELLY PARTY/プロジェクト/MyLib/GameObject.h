//--------------------------------------------------------------------------------------
// File: GameObject.h
//
// ゲーム中の3Dオブジェクトの基底クラス
//
// Last Update : 2023.10.26
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef GAMEOBJECT_DEFINED
#define GAMEOBJECT_DEFINED

#include "DirectXHelpers.h"
#include "SimpleMath.h"
#include "StepTimer.h"
#include "Model.h"

class GameObject
{
public:

	// デストラクタ
	virtual ~GameObject() = default;

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update(const float& elapsedTime, const DirectX::SimpleMath::Vector3& position) = 0;

	// 描画
	virtual void Render() = 0;

	// 後処理
	virtual void Finalize() = 0;
	
public:

	// IDを取得
	virtual int GetObjectID() = 0;

	// 位置を取得
	virtual DirectX::SimpleMath::Vector3 GetPosition() = 0;

	// モデルを取得
	virtual DirectX::Model* GetModel() = 0;

	// オブジェクトの状態を設定
	virtual void SetActive(bool isActive) = 0;
	

private:
	
};

#endif // GAMEOBJECT_DEFINED