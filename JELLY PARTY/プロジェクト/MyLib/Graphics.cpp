//--------------------------------------------------------------------------------------
// File: Graphics.h
//
// ���ʃ��\�[�X
//
// Last Update : 2023.10.29
// Author: Kota Matsuo
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "Graphics.h"

std::unique_ptr<Graphics> Graphics::m_graphics = nullptr;

//----------------------------------------
// Graphics�N���X�̃C���X�^���X���擾����
//----------------------------------------
Graphics* const Graphics::GetInstance()
{
	if (m_graphics == nullptr)
	{
		// Graphics�N���X�̃C���X�^���X�𐶐�����
		m_graphics.reset(new Graphics());
	}
	// Graphics�N���X�̃C���X�^���X��Ԃ�
	return m_graphics.get();
}


//-----------------------------------
// �R���X�g���N�^
//-----------------------------------
Graphics::Graphics()
	:
	m_timer(nullptr)
	, m_deviceResources(nullptr)
	, m_keyboardTracker(nullptr)
	, m_mouseTracker(nullptr)
	, m_states(nullptr)
	, m_spriteBatch(nullptr)
	, m_primitiveVPCT(nullptr)
	, m_primitiveVPT(nullptr)
	, m_primitiveVPC(nullptr)
	, m_basicEffect(nullptr)
	, m_inputLayout(nullptr)
	, m_debugFont(nullptr)
	, m_view{}
	, m_projection{}
{
}

//-----------------------------------
// �f�X�g���N�^
//-----------------------------------
Graphics::~Graphics()
{

}
