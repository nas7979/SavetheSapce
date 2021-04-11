#include "DXUT.h"
#include "cIntroScene.h"
#include "cEnemy1.h"

void cIntroScene::Init()
{
	for (int i = -5; i <= 5; i++)
	{
		cObject* a = OBJECT->AddObject<cEnemy1>("Enemy1", Vec2(-100 - abs(i) * 5, 270 + i * 30), 0.5, Obj_Enemy);
		a->GetComponent<cRenderer>()->m_Sprite = IMAGE->Find("Player");
		a->GetComponent<cEnemy>()->m_Dir = Vec2(1, 0);
		a->GetComponent<cEnemy>()->m_Move = false;
		a->AddComponent<cParticle>()->m_PosVel = Vec2(100, 0);
		a->m_Scale = Vec2(2, 2);
		a->m_Rot = 0;
	}

	cParticle* a = OBJECT->AddObject<cParticle>("Boss", Vec2(1500, 270), 0.5, Obj_Player)->GetComponent<cParticle>();
	a->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Boss1");
	a->m_Owner->m_Rot = 180;
	a->m_Owner->m_Scale = Vec2(0.3, 0.3);
	a->m_PosVel = Vec2(-200, 0);
	a->m_FuncTimer = 1000;
	a->m_FuncTime = 0;
	a->m_Func = [](cObject* _This)->void {
		CAMERA->Move(_This->m_Pos - Vec2(240, 135));
	};

	for (float i = 0; i < 1000; i++)
	{
		SCHEDULE->AddSchedule(0.01 * i, [=]()->void {
			IMAGE->BeginSurface("Ground");
			for (int i = 0; i < 54; i++)
			{
				IMAGE->Render(IMAGE->Find("Paint" + to_string(Random(1, 3))), Vec2(a->m_Owner->m_Pos.x + Random(-20.f, 20.f), i * 10), Random(0, 359), Vec2(0.06, 0.06) * Random(0.75f, 1.25f), 0.6974);
			}
			IMAGE->EndSurface();
		});
	}

	SCHEDULE->AddSchedule(3, [=]()->void {
		a->m_FuncTimer = 0;

	});
	SCHEDULE->AddSchedule(4, [=]()->void {
		for (float i = 0; i < 30; i++)
		{
			SCHEDULE->AddSchedule(0.03 * i, [=]()->void {
				float Dir = Random(130.f, 230.f);
				for (int j = -5; j <= 5; j++)
				{
					cBullet* Bullet = OBJECT->AddObject<cBullet>("Bullet", a->m_Owner->m_Pos, 0.6, Obj_PlayerBullet)->GetComponent<cBullet>();
					Bullet->GetComponent<cRenderer>()->m_Color = 0xffff0000;
					Bullet->m_Speed = 300;
					Bullet->m_Damage = 10;
					Bullet->m_Owner->m_Rot = Dir + j * 3;
				}
			});
		}
	});

	SCHEDULE->AddSchedule(5, [=]()->void {
		cParticle* Part = OBJECT->AddObject<cParticle>("Particle", Vec2(2000, 0), 0.01, Obj_Particle)->GetComponent<cParticle>();
		Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->m_Pixel;
		Part->GetComponent<cRenderer>()->m_Color = 0xff000000;
		Part->m_Owner->m_Scale = Vec2(2000, 2000);
		Part->m_PosVel = Vec2(-300, 0);
		a->m_FuncTimer = 1000;
	});

	SCHEDULE->AddSchedule(9, []()->void {
		SCENE->ChangeScene("MENU");
	});
}

void cIntroScene::Update()
{
}

void cIntroScene::Render()
{
	IMAGE->Render(IMAGE->Find("Back4"), Vec2(0, 0), 0, Vec2(1, 1), 0.9, Vec2(0, 0.5));
}

void cIntroScene::RenderBack()
{
	IMAGE->Render(IMAGE->Find("Back1"), Vec2(0, 0), 0, Vec2(1, 1), 0.6974, Vec2(0, 0.5));
}

void cIntroScene::Release()
{
	IMAGE->ClearSurface("Ground");
}
