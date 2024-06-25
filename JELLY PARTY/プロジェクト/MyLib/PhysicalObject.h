//--------------------------------------------------------------------------------------
// File: PhysicalObject.h
//
// 物理計算で必要な情報を持つ基底クラス
//
//
// Date: 2023.5.10
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once
#ifndef PHYSICAL_OBJECT_DEFINED
#define PHYSICAL_OBJECT_DEFINED


#include "DirectXHelpers.h"
#include "SimpleMath.h"
#include "StepTimer.h"

class PhysicalObject
{
public:
	
	// コンストラクタ
	PhysicalObject() :
		m_position{}
	{};
	// デストラクタ
	~PhysicalObject() {};

	// 更新
	virtual void Update(const DX::StepTimer& timer) = 0;

	// ポジションを設定
	virtual void SetPosition(const DirectX::SimpleMath::Vector3 position)	{ m_position = position; }

public:

	// ポジションを取得
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }

private:

	// ポジション
	DirectX::SimpleMath::Vector3 m_position;
	 
};

#endif // PHYSICAL_OBJECT_DEFINED