#include "DXUT.h"
#include "cStage2Scene.h"
#include "cPlayer.h"
#include "cEnemy1.h"
#include "cEnemy2.h"
#include "cEnemy3.h"
#include "cEnemy4.h"
#include "cBoss1.h"
#include "cBoss2.h"
#include "cBoss3.h"
#include "cObstacle.h"

void cStage2Scene::Init()
{
	cParticle* Part = OBJECT->AddObject<cParticle>("Particle", Vec2(0, 0), 0.01, Obj_Particle)->GetComponent<cParticle>();
	Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->m_Pixel;
	Part->GetComponent<cRenderer>()->m_Color = 0xff000000;
	Part->m_Alp = 255;
	Part->m_AlpVel = -255;
	Part->m_Owner->m_Scale = Vec2(10000, 10000);

	Part = OBJECT->AddObject<cParticle>("Particle", Vec2(480, 270), 0.01, Obj_Particle)->GetComponent<cParticle>();
	Part->AddComponent<cTextRenderer>()->m_Text = "-----STAGE2-----";
	Part->GetComponent<cTextRenderer>()->m_Color = 0xffff0000;
	Part->m_ScaVel = Vec2(0, -1);
	Part->m_Owner->m_Scale = Vec2(2, 2);

	OBJECT->AddObject<cPlayer>("Player", Vec2(OFFSET + MAPX * 0.5, OFFSET), 0.5, Obj_Player);
	GAME->PlayerInit();

	OBJECT->AddObject<cBoss2>("Boss", Vec2(OFFSET + MAPX * 0.5, OFFSET + MAPY * 0.5), 0.5, Obj_Enemy);
	for (int i = 0; i < 3; i++)
	{
		OBJECT->AddObject<cEnemy1>("Enemy1", Vec2(OFFSET + Random(100, MAPX - 100), OFFSET + Random(100, MAPY - 100)), 0.5, Obj_Enemy);
		OBJECT->AddObject<cEnemy2>("Enemy2", Vec2(OFFSET + Random(100, MAPX - 100), OFFSET + Random(100, MAPY - 100)), 0.5, Obj_Enemy);
		OBJECT->AddObject<cEnemy3>("Enemy3", Vec2(OFFSET + Random(100, MAPX - 100), OFFSET + Random(100, MAPY - 100)), 0.5, Obj_Enemy);
	}

	Vec2 Pos;
	for (int i = 0; i < 8; i++)
	{
	Reset:
		Pos = Vec2(Random(OFFSET + 50, OFFSET + MAPX - 50), Random(OFFSET + 50, MAPY - 50));
		for (auto& iter : OBJECT->m_Objects[Obj_Enemy])
		{
			if (Distance(iter->m_Pos - Pos) <= 100)
				goto Reset;
		}
		OBJECT->AddObject<cObstacle>("Obstacle", Pos, 0.7, Obj_Map);
	}

	m_BackPos = 0;
}

void cStage2Scene::Update()
{
}

void cStage2Scene::Render()
{
	if (SCENE->m_FlipBack)
	{
		IMAGE->Render(IMAGE->Find("Back4"), Vec2(-0, ((int)m_BackPos % 1800)), 0, Vec2(1, 1), 0.9, Vec2(0.5, 0.5));
		IMAGE->Render(IMAGE->Find("Back4"), Vec2(-0, ((int)m_BackPos % 1800) - 1800), 0, Vec2(1, 1), 0.9, Vec2(0.5, 0.5));
		return;
	}
	IMAGE->Render(IMAGE->Find("Back2"), Vec2(-0, ((int)m_BackPos % 1800)), 0, Vec2(1, 1), 0.9, Vec2(0.5, 0.5));
	IMAGE->Render(IMAGE->Find("Back2"), Vec2(-0, ((int)m_BackPos % 1800) - 1800), 0, Vec2(1, 1), 0.9, Vec2(0.5, 0.5));
}

void cStage2Scene::RenderBack()
{
	if (SCENE->m_FlipBack)
	{
		IMAGE->Render(IMAGE->Find("Back2"), Vec2(-0, ((int)m_BackPos % 1800)), 0, Vec2(1, 1), 0.6974, Vec2(0.5, 0.5));
		IMAGE->Render(IMAGE->Find("Back2"), Vec2(-0, ((int)m_BackPos % 1800) - 1800), 0, Vec2(1, 1), 0.6974, Vec2(0.5, 0.5));
		return;
	}
	IMAGE->Render(IMAGE->Find("Back4"), Vec2(-0, ((int)m_BackPos % 1800)), 0, Vec2(1, 1), 0.6974, Vec2(0.5, 0.5));
	IMAGE->Render(IMAGE->Find("Back4"), Vec2(-0, ((int)m_BackPos % 1800) - 1800), 0, Vec2(1, 1), 0.6974, Vec2(0.5, 0.5));
}

void cStage2Scene::Release()
{
}
