#include "DXUT.h"
#include "cBoss1.h"
#include "cTimeLine_Boss1.h"

void cBoss1::Init()
{
	cBoss::Init();
	AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Boss1");
	GetComponent<cMapCollider>()->m_Radius = 75;
	GetComponent<cCollider>()->AddCollider(Vec2(0, 0), 90);
	GetComponent<cCollider>()->AddCollider(Vec2(0, -125), 60);
	GetComponent<cCollider>()->AddCollider(Vec2(0, 125), 60);
	AddComponent<cTimeLine_Boss1>()->m_Enable = false;
	m_Owner->m_Scale = Vec2(0.3, 0.2);

	m_Hp = m_MaxHp = 50;
	m_Speed = 80;
	m_StopTime = 5;
	SCENE->m_NextScene = "STAGE2";

	SCENE->m_FlipBack = true;
	OBJECT->m_Stop = true;

	SCHEDULE->AddSchedule(0, [&]()->void {
		for (auto& iter : OBJECT->m_Objects[Obj_Enemy])
		{
			iter->GetComponent<cRenderer>()->m_Enable = false;
			cObject* a = OBJECT->AddObject<cParticle>("Particle", iter->m_Pos, 0.6974, Obj_Back);
			a->AddComponent<cRenderer>()->m_Sprite = iter->GetComponent<cRenderer>()->m_Sprite;
			a->m_Scale = iter->m_Scale;
			a->m_Rot = iter->m_Rot;
		}
	});

	IMAGE->BeginSurface("Drawing");
	IMAGE->RenderSurface("Outline", Vec2(0,0),Vec2(1,1),0.1);
	IMAGE->EndSurface();
	SCHEDULE->AddSchedule(2, [&]()->void {
		cParticle* Part = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, 0.6974, Obj_Back)->GetComponent<cParticle>();
		Part->m_ScaVel = Vec2(2.5, 2.5);
		Part->m_Owner->m_Scale = Vec2(0.001, 0.001);
		Part->m_Owner->SetAlarm("Destroy", 2);
		Part->m_FuncTime = 0;
		Part->m_Func = [](cObject* _This)->void {
			IMAGE->BeginSurface("Ground");
			IMAGE->Render(IMAGE->Find("Circle"), _This->m_Pos, _This->m_Rot, _This->m_Scale, 0.6974);
			IMAGE->EndSurface();
		};
		IMAGE->BeginSurface("Ground");
		IMAGE->Render(IMAGE->Find("Bullet_Base"), m_Owner->m_Pos, m_Owner->m_Rot, Vec2(2, 2), 0.6974);
		IMAGE->EndSurface();

		Part = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, 0.6974, Obj_None)->GetComponent<cParticle>();
		Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Circle");
		Part->GetComponent<cRenderer>()->m_Color = 0xffff0000;
		Part->m_ScaVel = Vec2(3, 3);
		Part->m_Owner->m_Scale = Vec2(0.01, 0.01);
		Part->m_Owner->SetAlarm("Destroy", 2);
	});

	SCHEDULE->AddSchedule(4, []()->void {
		for (auto& iter : OBJECT->m_Objects[Obj_Back])
		{
			iter->m_isDestroyed = true;
		}
		for (auto& iter : OBJECT->m_Objects[Obj_Enemy])
		{
			iter->GetComponent<cRenderer>()->m_Enable = true;
		}
		OBJECT->m_Stop = false;
		SCENE->m_FlipBack = false;
		IMAGE->ClearSurface("Ground");
	});
}

void cBoss1::Update()
{
	cBoss::Update();
}

void cBoss1::Render()
{
	cBoss::Render();
}

void cBoss1::Release()
{
	cBoss::Release();
}

void cBoss1::OnAlarm(string _Key)
{
	cBoss::OnAlarm(_Key);
}

void cBoss1::OnCollision(cObject * _Other)
{
	cBoss::OnCollision(_Other);
}

void cBoss1::Attack()
{
	if (Random(1, 10) <= GAME->m_Percentage * 0.1f)
		return;

	cBullet* a;
	float Dir = Direction(OBJECT->m_Player->m_Pos - m_Owner->m_Pos);
	for (int i = -5; i <= 5; i++)
	{
		a = FireBullet(m_Owner->m_Pos, Dir + i * 7, 0.5, 10, 300, 0xffff0000, Obj_EnemyBullet);
		a->m_EndSpeed = 100;
		a->m_Friction = 0.94;
	}
}

void cBoss1::Death()
{
	cBoss::Death();
}

void cBoss1::DealDamage(float _Damage)
{
	cBoss::DealDamage(_Damage);
}

void cBoss1::OnStop()
{
	GetComponent<cTimeLine_Boss1>()->m_Enable = true;
}
