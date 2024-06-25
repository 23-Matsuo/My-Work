//--------------------------------------------------------------------------------------
// File: Result.cpp
//
// カウントダウンクラス
//
// Date: 2023/12/09
// Author: Kota Matsuo
//-------------------------------------------------------------------------------------

#include "pch.h"

#include "Result.h"

//------------------------------------------
// コンストラクタ
//------------------------------------------
Result::Result(const float& width, const float& height)
	:
	m_defaultScale(0.4f)
	, m_windowSize(width, height)
	, m_alpha(1.0f)
	, m_timer(0.0f)
	, m_acceleration(0.0f)
	, m_gravity(100.0f)
	, m_damping{}

{
	// 初期位置
	m_position = { width / 2.0f, 0.0f };

	// スケール
	m_scale = m_defaultScale;
}

//------------------------------------------
// デストラクタ
//------------------------------------------
Result::~Result()
{

}

//------------------------------------------
// 更新
//------------------------------------------
bool Result::Update()
{
	if (m_timer >= 1.5f) return true;

	RECT rect = Graphics::GetInstance()->GetDeviceResources()->GetOutputSize();

	float elapsedTime = static_cast<float>(Graphics::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	if (m_position.y >= m_windowSize.y / 7.0f)
	{
		m_timer += elapsedTime;
		// バネの動き
		Damping(&m_scale.y, &m_damping.y, m_defaultScale.y);
	}
	else
	{
		m_acceleration += m_gravity;
		m_position.y += m_acceleration * elapsedTime;
		m_scale.y += 0.01f;
	}
	return false;
}

//------------------------------------------
// 描画
//------------------------------------------
void Result::Render()
{

	DirectX::SpriteBatch* spriteBatch = Graphics::GetInstance()->GetSpriteBatch();
	Resources::TextureTable resultTexture = Resources::GetInstance()->GetTexture(L"Result");


	// 1マス分の画像サイズ
	LONG spriteWidth = static_cast<LONG>(resultTexture.textureWidth);
	LONG spriteHeight = static_cast<LONG>(resultTexture.textureHeight);

	// 画像の切り取り位置
	RECT src = { 0, 0, spriteWidth, spriteHeight };


	// 描画
	spriteBatch->Begin();

	spriteBatch->Draw(
		resultTexture.texture.Get(),
		m_position,
		&src,
		DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, m_alpha),
		0.0f,
		DirectX::SimpleMath::Vector2(spriteWidth / 2.0f, spriteHeight),
		m_scale
	);

	spriteBatch->End();
}


//------------------------------------------
// バネの動き
//------------------------------------------
void Result::Damping(float* scale, float* velocity, float defaultScale)
{
	// バネ定数
	const float K = 0.08f;
	// 減衰率
	const float FRICTION = 0.9f;

	float a = -K * (*scale - defaultScale);
	*velocity += a;
	*velocity *= FRICTION;
	*scale += *velocity;
}
