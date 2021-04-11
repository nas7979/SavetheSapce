#include "DXUT.h"
#include "cInputManager.h"

void cInputManager::Init()
{
}

void cInputManager::Update()
{
	memcpy(m_Old, m_Cur, 128);
	memset(m_Cur, false, 128);
	for (int i = 0; i < 128; i++)
	{
		if (GetAsyncKeyState(i) & 0x8000)
		{
			m_Cur[i] = true;
		}
	}

	if (m_Cur[VK_LEFT])
	{
		m_Cur[VK_RIGHT] = false;
		m_Cur[VK_UP] = false;
		m_Cur[VK_DOWN] = false;
	}
	if (m_Cur[VK_RIGHT])
	{
		m_Cur[VK_LEFT] = false;
		m_Cur[VK_UP] = false;
		m_Cur[VK_DOWN] = false;
	}
	if (m_Cur[VK_UP])
	{
		m_Cur[VK_RIGHT] = false;
		m_Cur[VK_LEFT] = false;
		m_Cur[VK_DOWN] = false;
	}
	if (m_Cur[VK_DOWN])
	{
		m_Cur[VK_RIGHT] = false;
		m_Cur[VK_UP] = false;
		m_Cur[VK_LEFT] = false;
	}
}

void cInputManager::Render()
{
}

void cInputManager::Release()
{
}
