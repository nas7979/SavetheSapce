#include "DXUT.h"
#include "cSoundManager.h"

void cSoundManager::Init()
{
	m_Manager.Initialize(DXUTGetHWND(), 1);
}

void cSoundManager::Update()
{
	DWORD Status;
	for (auto& iter = m_Channels.begin(); iter != m_Channels.end();)
	{
		(*iter)->GetStatus(&Status);
		if (Status & DSBSTATUS_PLAYING == 0)
		{
			(*iter)->Release();
			iter = m_Channels.erase(iter);
		}
		else
			iter++;
	}
}

void cSoundManager::Render()
{
}

void cSoundManager::Release()
{
	StopAll();
	for (auto& iter : m_Sounds)
	{
		delete iter.second;
	}
}

void cSoundManager::AddSound(string _Key, string _Path)
{
	CSound* a;
	m_Manager.Create(&a, (LPWSTR)wstring(_Path.begin(), _Path.end()).c_str(), DSBCAPS_CTRLVOLUME);
	m_Sounds[_Key] = a;
}

void cSoundManager::Play(string _Key, long _Volume, bool _Loop)
{
	LPDIRECTSOUNDBUFFER a;
	m_Manager.GetDirectSound()->DuplicateSoundBuffer(m_Sounds[_Key]->GetBuffer(0), &a);
	a->Play(0, 0, _Loop ? DSBPLAY_LOOPING : 0);
	a->SetVolume(_Volume);
	m_Channels.push_back(a);
}
	
void cSoundManager::StopAll()
{
	for (auto& iter : m_Channels)
	{
		iter->Release();
	}
	m_Channels.clear();
}
