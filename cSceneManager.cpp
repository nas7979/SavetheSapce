#include "DXUT.h"
#include "cSceneManager.h"

void cSceneManager::Init()
{
}

void cSceneManager::Update()
{
	if (m_New)
	{
		OBJECT->Release();
		SCHEDULE->Release();
		if (m_Cur)
			m_Cur->Release();
		m_Cur = m_New;
		m_New = nullptr;
		m_Cur->Init();
	}
	if (m_Cur)
	{
		m_Cur->Update();
	}
}

void cSceneManager::Render()
{
	if (m_Cur)
	{
		m_Cur->Render();
	}
}

void cSceneManager::RenderBack()
{
	if (m_Cur)
	{
		m_Cur->RenderBack();
	}
}

void cSceneManager::Release()
{
	for (auto& iter : m_Scenes)
	{
		iter.second->Release();
		delete iter.second;
	}
}

void cSceneManager::AddScene(string _Key, cScene * _Scene)
{
	m_Scenes[_Key] = _Scene;
}

void cSceneManager::ChangeScene(string _Key)
{
	m_New = m_Scenes[_Key];
	m_CurKey = _Key;
}
