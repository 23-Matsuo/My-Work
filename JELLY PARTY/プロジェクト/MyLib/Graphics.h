//--------------------------------------------------------------------------------------
// File: Graphics.h
//
// 共通リソース
//
// Last Update: 2023.10.24
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------
#pragma once

#ifndef GRAPHICS_DEFINED
#define GRAPHICS_DEFINED

#include "StepTimer.h"
#include "DeviceResources.h"
#include "ImaseLib/DebugFont.h"

// Graphicsクラスを定義する
class Graphics
{
public:

	// Graphicsクラスのインスタンスを取得する
	static Graphics* const GetInstance();
	// デストラクタ
	~Graphics();

public: // セッター

	// ステップタイマーを設定
	void SetStepTimerStates(DX::StepTimer* timer)												{ m_timer = timer; }
	// デバイスリソースを設定
	void SetDeviceResources(DX::DeviceResources* deviceResources)								{ m_deviceResources = deviceResources; }
	// キーボードステートトラッカーを設定
	void SetKeyboardStateTracker(DirectX::Keyboard::KeyboardStateTracker* tracker)				{ m_keyboardTracker = tracker; }
	// マウスステートトラッカーを設定
	void SetMouseStateTracker(DirectX::Mouse::ButtonStateTracker* tracker)						{ m_mouseTracker = tracker; }
	// 共通ステートを設定
	void SetCommonStates(DirectX::CommonStates* states)											{ m_states = states; }
	// スプライトバッチを設定
	void SetSpriteBatch(DirectX::SpriteBatch* spriteBatch)										{ m_spriteBatch = spriteBatch; }
	// プリミティブバッチを設定(VPCT)
	void SetPrimitiveBatch(DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* vpct)	{ m_primitiveVPCT = vpct; }
	// プリミティブバッチを設定(VPT)
	void SetPrimitiveBatch(DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* vpt)		{ m_primitiveVPT = vpt; }
	// プリミティブバッチを設定(VPC)
	void SetPrimitiveBatch(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* vpc)			{ m_primitiveVPC = vpc; }
	// ベーシックエフェクトを設定
	void SetBasicEffect(DirectX::BasicEffect* effect)											{ m_basicEffect = effect; }
	// インプットレイアウトを設定
	void SetInputLayout(ID3D11InputLayout* inputLayout)											{ m_inputLayout = inputLayout; }
	// デバッグフォントを設定
	void SetDebugFont(Imase::DebugFont* debugFont)												{ m_debugFont = debugFont; }
	// ビュー行列を設定
	void SetViewMatrix(DirectX::SimpleMath::Matrix view)										{ m_view = view; }
	// 射影行列を設定
	void SetProjectionMatrix(DirectX::SimpleMath::Matrix projection)							{ m_projection = projection; }
	// カメラのターゲット位置を設定
	void SetCameraTarget(DirectX::SimpleMath::Vector3 target)									{ m_cameraTarget = target; }
	// カメラの目の位置を設定
	void SetCameraEye(DirectX::SimpleMath::Vector3 eye)											{ m_cameraEye = eye; }

public:	// ゲッター

	// ステップタイマーを取得
	DX::StepTimer* GetStepTimer()																{ return m_timer; }
	// デバイスリソースを取得
	DX::DeviceResources* GetDeviceResources()													{ return m_deviceResources; }
	// キーボードステートトラッカーを取得
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardStateTracker()							{ return m_keyboardTracker; }
	// マウスステートトラッカーを取得
	DirectX::Mouse::ButtonStateTracker* GetMouseStateTracker()									{ return m_mouseTracker; }
	// 共通ステートを取得
	DirectX::CommonStates* GetCommonStates()													{ return m_states; }
	// スプライトバッチを取得
	DirectX::SpriteBatch* GetSpriteBatch()														{ return m_spriteBatch; }
	// プリミティブバッチ(VPCT)を取得
	DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* GetPrimitiveBatchVPCT()		{ return m_primitiveVPCT; }
	// プリミティブバッチ(VPT)を取得
	DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* GetPrimitiveBatchVPT()				{ return m_primitiveVPT; }
	// プリミティブバッチ(VPC)を取得
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* GetPrimitiveBatchVPC()				{ return m_primitiveVPC; }
	// ベーシックエフェクトを取得
	DirectX::BasicEffect* GetBasicEffect()														{ return m_basicEffect; }
	// インプットレイアウトを取得
	ID3D11InputLayout* GetInputLayout()															{ return m_inputLayout; }
	// デバッグフォントを取得
	Imase::DebugFont* GetDebugFont()															{ return m_debugFont; }
	// ビュー行列を取得
	const DirectX::SimpleMath::Matrix& GetViewMatrix()											{ return m_view; }
	// 射影行列を取得
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix()									{ return m_projection; }
	// カメラのターゲットを取得
	const DirectX::SimpleMath::Vector3& GetCameraTarget()										{ return m_cameraTarget; }
	// カメラのアイを取得
	const DirectX::SimpleMath::Vector3& GetCameraEye()											{ return m_cameraEye; }

private:

	// コンストラクタ
	Graphics();
	// 代入は許容しない
	void operator=(const Graphics& object) = delete;
	// コピーコンストラクタは許容しない
	Graphics(const Graphics& object) = delete;

private:

	// Graphicsクラスのインスタンスへのポインタ
	static std::unique_ptr<Graphics> m_graphics;

private:
	// ステップタイマー
	DX::StepTimer* m_timer;

	// デバイスリソース
	DX::DeviceResources* m_deviceResources;

	// キーボードステートトラッカー
	DirectX::Keyboard::KeyboardStateTracker* m_keyboardTracker;

	// マウスステートトラッカー
	DirectX::Mouse::ButtonStateTracker* m_mouseTracker;

	// ステートへのポインタ
	DirectX::CommonStates* m_states;

	// スプライトバッチへのポインタ
	DirectX::SpriteBatch* m_spriteBatch;

	// プリミティブバッチ
	DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* m_primitiveVPCT;
	DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>* m_primitiveVPT;
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* m_primitiveVPC;

	// ベーシックエフェクト
	DirectX::BasicEffect* m_basicEffect;

	// インプットレイアウト
	ID3D11InputLayout* m_inputLayout;

	// デバッグ用文字列表示へのポインタ
	Imase::DebugFont* m_debugFont;

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// 射影行列
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	// 射影行列
	DirectX::SimpleMath::Vector3 m_cameraEye;
};

#endif // GRAPHICS_DIFINED