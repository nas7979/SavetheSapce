#pragma once
class cSoundManager : public cSingleton<cSoundManager>
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	list<LPDIRECTSOUNDBUFFER> m_Channels;
	map<string, CSound*> m_Sounds;
	CSoundManager m_Manager;

	void AddSound(string _Key, string _Path);
	void Play(string _Key, long _Volume = -2000, bool _Loop = false);
	void StopAll();
};

#define SOUND cSoundManager::GetInstance()