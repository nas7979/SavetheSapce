#include "DXUT.h"
#include "cLoadingScene.h"
#include <filesystem>

void cLoadingScene::Init()
{
	string Key;
	vector<string> Anim;
	IMAGE->AddSprite("Pixel", "./Resource/Image/Pixel.png");
	for (auto& iter : experimental::filesystem::recursive_directory_iterator("./Resource/Image"))
	{
		Key = iter.path().filename().string();
		Key.pop_back();
		Key.pop_back();
		Key.pop_back();
		Key.pop_back();
		IMAGE->AddSprite(Key, iter.path().string());
		if (Key.back() == '1')
		{
			Key.pop_back();
			Anim.push_back(Key);
		}
	}
	for (auto& iter : Anim)
	{
		IMAGE->AddAnimation(iter);
	}

	for (auto& iter : experimental::filesystem::recursive_directory_iterator("./Resource/Sound"))
	{
		Key = iter.path().filename().string();
		Key.pop_back();
		Key.pop_back();
		Key.pop_back();
		Key.pop_back();
		SOUND->AddSound(Key, iter.path().string());
	}

	m_Wait = 0;
	IMAGE->m_Pixel = IMAGE->Find("Pixel");
}

void cLoadingScene::Update()
{
	m_Wait++;
	if (m_Wait == 3)
		SCENE->ChangeScene("INTRO");
}

void cLoadingScene::Render()
{
}

void cLoadingScene::RenderBack()
{
}

void cLoadingScene::Release()
{
}
