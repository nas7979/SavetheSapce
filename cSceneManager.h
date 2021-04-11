#pragma once
#include "cScene.h"
class cSceneManager : public cSingleton<cSceneManager>
{
public:
	void Init();
	void Update();
	void Render();
	void RenderBack();
	void Release();

	cScene* m_Cur = nullptr;
	cScene* m_New = nullptr;
	map<string, cScene*> m_Scenes;
	string m_CurKey = "";
	string m_NextScene;
	bool m_FlipBack = false;

	void AddScene(string _Key, cScene* _Scene);
	void ChangeScene(string _Key);
};

#define SCENE cSceneManager::GetInstance()