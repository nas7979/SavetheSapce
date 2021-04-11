#include "DXUT.h"
#include "cMenuScene.h"

void cMenuScene::Init()
{
	SOUND->Play("Main Music 9000", -1000, true);
	CAMERA->m_Pos = CAMERA->m_PosTo = Vec2(240, 135);
	cParticle* Part = OBJECT->AddObject<cParticle>("Particle", Vec2(0, 0), 0.01, Obj_Particle)->GetComponent<cParticle>();
	Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->m_Pixel;
	Part->GetComponent<cRenderer>()->m_Color = 0xff000000;
	Part->m_Alp = 255;
	Part->m_AlpVel = -255;
	Part->m_Owner->m_Scale = Vec2(10000, 10000);

	m_Part = OBJECT->AddObject<cParticle>("Player", Vec2(-100, 270), 0.5, Obj_Particle)->GetComponent<cParticle>();
	m_Part->m_PosVel = Vec2(1000, 0);
	m_Part->m_PosFri = 0.97;
	m_Part->m_Owner->m_Scale = Vec2(3, 3);

	m_Text[0] = "START";
	m_Text[1] = "HOW TO PLAY";
	m_Text[2] = "RANKING";
	m_Text[3] = "EXIT";

	m_Color[0] = 0xffffffff;
	m_Color[1] = m_Color[2] = m_Color[3] = 0x90ffffff;

	m_Size[0] = 1.5;
	m_Size[1] = m_Size[2] = m_Size[3] = 1;

	m_Mask = false;
	m_Selected = 0;
	m_MoveAble = true;
	m_BackPos = 0;
}

void cMenuScene::Update()
{
	if (m_MoveAble == false)
		return;
	for (int i = 0; i < 4; i++)
	{
		if (m_Selected == i)
		{
			m_Color[i] = 0xffffffff;
			m_Size[i] = Lerp<float>(m_Size[i], 1.5, 0.1);
		}
		else
		{
			m_Color[i] = 0x90ffffff;
			m_Size[i] = Lerp<float>(m_Size[i], 1, 0.1);
		}
	}

	if (!m_Mask)
	{
		if (INPUT->KeyDown(VK_UP))
		{
			m_Selected--;
			if (m_Selected == -1)
				m_Selected = 3;
		}
		if (INPUT->KeyDown(VK_DOWN))
		{
			m_Selected++;
			if (m_Selected == 4)
				m_Selected = 0;
		}
	}

	if (INPUT->KeyDown(VK_SPACE))
	{
		switch (m_Selected)
		{
		case 0:
		{
			m_Part->m_PosFri = 1.05;
			m_Part->m_PosVel = Vec2(15, 0);
			m_MoveAble = false;
			SCHEDULE->AddSchedule(1, []()->void {
				cParticle* Part = OBJECT->AddObject<cParticle>("Particle", Vec2(0, 0), 0, Obj_Particle)->GetComponent<cParticle>();
				Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->m_Pixel;
				Part->GetComponent<cRenderer>()->m_Color = 0x00000000;
				Part->m_Alp = 0;
				Part->m_AlpVel = 255;
				Part->m_Owner->m_Scale = Vec2(10000, 10000);

				SCHEDULE->AddSchedule(1, []()->void {
					SCENE->ChangeScene("STAGE1");
				});
			});
		}
		break;

		case 1:
		case 2:
		{
			m_Mask = !m_Mask;
		}
		break;

		case 3:
		{
			exit(0);
		}
		break;
		}
	}
	m_BackPos + 50 * DT;
}

void cMenuScene::Render()
{
	if (m_Mask)
	{
		IMAGE->Render(IMAGE->m_Pixel, Vec2(0, 0), 0, Vec2(10000, 10000), 0.05, Vec2(0, 0), 0x90000000);
		if(m_Selected == 1)
		{
			IMAGE->Render(IMAGE->Find("How"), Vec2(480, 270), 0, Vec2(1, 1), 0.01);
		}
		else
		{
			IMAGE->RenderText("RANKING", Vec2(480, 50), 0, Vec2(1, 1), 0.01, Vec2(0.5, 0.5), 0xffff0000);
			int i = 0;
			for (auto& iter : GAME->m_Ranking)
			{
				IMAGE->RenderText(to_string(i + 1) + ". " + iter->Key, Vec2(280, 125 + i * 40), 0, Vec2(0.5, 0.5), 0.01, Vec2(0, 0.5), 0xffffffff);
				IMAGE->RenderText(to_string(iter->Score), Vec2(680, 125 + i * 40), 0, Vec2(0.5, 0.5), 0.01, Vec2(1, 0.5), 0xffffffff);
				i++;
				if (i == 10)
					break;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		IMAGE->RenderText(m_Text[i], Vec2(20 + i * 10, 270 + 60 * i), 0, Vec2(0.75, 0.75) * m_Size[i], 0.1, Vec2(0, 0.5), m_Color[i]);
	}
	IMAGE->Render(IMAGE->Find(GAME->m_Win ? "Back4" : "Back1"), Vec2(-0, ((int)m_BackPos % 1800)), 0, Vec2(1, 1), 0.9, Vec2(0.5, 0.5));
	IMAGE->Render(IMAGE->Find(GAME->m_Win ? "Back4" : "Back1"), Vec2(-0, ((int)m_BackPos % 1800) - 1800), 0, Vec2(1, 1), 0.9, Vec2(0.5, 0.5));
	IMAGE->Render(IMAGE->Find("Player"), m_Part->m_Owner->m_Pos, 0, Vec2(3, 3), 0.5, Vec2(0.5, 0.5));
}

void cMenuScene::RenderBack()
{
}

void cMenuScene::Release()
{
}
