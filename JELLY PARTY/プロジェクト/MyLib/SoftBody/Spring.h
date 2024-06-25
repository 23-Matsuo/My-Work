//--------------------------------------------------------------------------------------
// File: Spring.h
//
// 頂点を繋げるバネクラス
//
//
// Date: 2023.5.17
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#pragma once

#ifndef SPRING_DEFINED
#define SPRING_DEFINED

#include "PhysicalPoint.h"

class Spring
{
public:
	
	// コンストラクタ
	Spring(PhysicalPoint *p1, PhysicalPoint *p2);
	// デストラクタ
	~Spring() {};

	// 力の計算
	void CalculateForce();

	// 力を加える
	virtual void AddForce();

private:
	//// 力の限度
	//const float MAX_FORCE = 100.0f;

	// 頂点１
	PhysicalPoint* m_point1;
	// 頂点２
	PhysicalPoint* m_point2;

	// 点と点の距離
	float m_distance;
	// 点と点の現在の距離
	float m_currentDistance;

	// 頂点１に加算する力
	DirectX::SimpleMath::Vector3 m_force1;
	// 頂点２に加算する力
	DirectX::SimpleMath::Vector3 m_force2;
	// 力の増減
	DirectX::SimpleMath::Vector3 m_fluctuation;

	// 向き
	DirectX::SimpleMath::Vector3 m_direction;

	// 抵抗力
	DirectX::SimpleMath::Vector3 m_resist;

	// 抵抗率
	float m_resistance;

	// 弾性率
	float m_elasticity;

	// パラメータ
	nlohmann::json m_parameters;
};

#endif // SPRING_DEFINED