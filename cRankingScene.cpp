#include "DXUT.h"
#include "cRankingScene.h"

void cRankingScene::Init()
{
	SOUND->StopAll();
	m_Typing = true;
	m_Name.clear();
	if (GAME->m_Continued)
	{
		m_Typing = false;
	}
}

void cRankingScene::Update()
{
	if (!m_Typing)
	{
		if (INPUT->KeyDown(VK_RETURN))
		{
			cParticle* Part = OBJECT->AddObject<cParticle>("Particle", Vec2(0, 0), 0, Obj_Particle)->GetComponent<cParticle>();
			Part->m_Owner->m_Scale = Vec2(10000, 10000);
			Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->m_Pixel;
			Part->GetComponent<cRenderer>()->m_Color = 0x00000000;
			Part->m_Alp = 0;
			Part->m_AlpVel = 255;
			SCHEDULE->AddSchedule(1, []()->void {
				SCENE->ChangeScene("MENU");
				GAME->m_Continued = false;
			});
		}
	}
	else
	{
		if (m_Name.size() != 3)
		{
			for (int i = 'A'; i < 'Z'; i++)
			{
				if (INPUT->KeyDown(i))
				{
					m_Name.push_back(i);
					break;
				}
			}
		}
		else
		{
			if (INPUT->KeyDown(VK_RETURN))
			{
				m_Typing = false;
				GAME->AddRanking(m_Name, GAME->m_Score);
			}
		}
		if (m_Name.size() != 0)
		{
			if (INPUT->KeyDown(VK_BACK))
			{
				m_Name.pop_back();
			}
		}
	}
}

void cRankingScene::Render()
{
	IMAGE->Render(IMAGE->m_Pixel, Vec2(0, 0), 0, Vec2(10000, 10000), 0.1, Vec2(0, 0), 0xff000000);
	IMAGE->RenderText(GAME->m_Win ? "GAME CLEAR!!" : "GAME OVER", Vec2(480, 40), 0, Vec2(1, 1), 0.1, Vec2(0.5, 0.5), 0xffff0000);
	IMAGE->RenderText(GAME->m_Continued ? "You Can't Save Your Score Due To Continue." : "Enter Your Name.", Vec2(480, 100), 0, Vec2(0.5, 0.5), 0.1, Vec2(0.5, 0.5), 0xffff0000);

	IMAGE->RenderText(m_Name, Vec2(480, 150), 0, Vec2(1, 1), 0.1, Vec2(0.5, 0.5), 0xffff0000);

	int i = 0;
	for (auto& iter : GAME->m_Ranking)
	{
		IMAGE->RenderText(to_string(i + 1) + ". " + iter->Key, Vec2(280, 200 + i * 32), 0, Vec2(0.5, 0.5), 0.1, Vec2(0, 0.5), iter->Score == GAME->m_Score ? 0xffff0000 : 0xffffffff);
		IMAGE->RenderText(to_string(iter->Score), Vec2(680, 200 + i * 32), 0, Vec2(0.5, 0.5), 0.1, Vec2(1, 0.5), iter->Score == GAME->m_Score ? 0xffff0000 : 0xffffffff);
		i++;
		if (i == 10)
			break;
	}
}

void cRankingScene::RenderBack()
{
}

void cRankingScene::Release()
{
}
