#include "DXUT.h"
#include "cBoss2.h"
#include "cTimeLine_Boss2.h"
#include "cBoss2_Tail.h"

void cBoss2::Init()
{
	cBoss::Init();
	AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Boss2");
	GetComponent<cMapCollider>()->m_Radius = 75;
	GetComponent<cCollider>()->AddCollider(Vec2(0, 0), 90);
	AddComponent<cTimeLine_Boss2>()->m_Enable = false;
	m_Owner->m_Scale = Vec2(0.2, 0.2);

	m_Hp = m_MaxHp = 60;
	m_Speed = 130;
	m_StopTime = 5;

	cObject* a;
	for (int i = 0; i < m_Size - 1; i++)
	{
		a = OBJECT->AddObject<cBoss2_Tail>("Tail", m_Owner->m_Pos, m_Owner->m_Depth + i * 0.001, Obj_Enemy);
		a->GetComponent<cBoss2_Tail>()->m_Head = this;
		a->m_Rot = m_Owner->m_Rot;
		a->m_Scale = m_Owner->m_Scale;
		m_Tails.push_back(a);
	}
	SCENE->m_NextScene = "STAGE3";

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
	IMAGE->RenderSurface("Outline", Vec2(0, 0), Vec2(1, 1), 0.1);
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

void cBoss2::Update()
{
	cBoss::Update();

	m_TailPos.push_front(new TailPos{ m_Owner->m_Pos, m_Owner->m_Rot });
	while (m_TailPos.size() > m_Size * 50 * m_Owner->m_Scale.x)
	{
		delete m_TailPos.back();
		m_TailPos.pop_back();
	}

	int Max = m_Size * 50 * m_Owner->m_Scale.x;
	int i = 0;
	int Cur = 0;
	for (auto& iter : m_TailPos)
	{
		i++;
		if (i % m_Size * 50 * m_Owner->m_Scale.x == 0)
		{
			m_Tails[Cur]->m_Pos = iter->Pos;
			m_Tails[Cur]->m_Rot = iter->Rot;
			Cur++;
			if (Cur == m_Size - 1)
				break;
		}
	}

	m_Collided.clear();
}

void cBoss2::Render()
{
	cBoss::Render();
}

void cBoss2::Release()
{
	cBoss::Release();
	for (auto& iter : m_TailPos)
	{
		delete iter;
	}
}

void cBoss2::OnAlarm(string _Key)
{
	cBoss::OnAlarm(_Key);
}

void cBoss2::OnCollision(cObject * _Other)
{
	cBoss::OnCollision(_Other);
}

void cBoss2::Attack()
{
	if (Random(1, 10) <= GAME->m_Percentage * 0.1)
		return;

	for (float j = 0; j < 5; j++)
	{
		SCHEDULE->AddSchedule(0.1 * j, [&]()->void {
			for (auto& iter : m_Tails)
			{
				for (int i = 0; i < 3; i++)
				{
					FireBullet(iter->m_Pos, iter->m_Rot + i * 120, 0.5, 9, 125, 0xffffff00, Obj_EnemyBullet);
				}
			}
		});
	}
}

void cBoss2::Death()
{
	cBoss::Death();
	if (m_Size == 1)
		return;

	m_Tails.back()->GetComponent<cEnemy>()->Death();
	m_Tails.pop_back();
	for (auto& iter : m_Tails)
	{
		iter->m_Scale *= 0.9;
	}
}

void cBoss2::DealDamage(float _Damage)
{
	cBoss::DealDamage(_Damage);
}

void cBoss2::OnStop()
{
	GetComponent<cTimeLine_Boss2>()->m_Enable = true;
}

void cBoss2::NextStage()
{
	OBJECT->m_Stop = true;
	for (int i = 0; i < m_Tails.size(); i++)
	{
		SCHEDULE->AddSchedule(i * 0.5, [&]()->void {
			m_Tails.back()->GetComponent<cEnemy>()->Death();
			m_Tails.pop_back();
		});
	}
	SCHEDULE->AddSchedule(m_Tails.size() * 0.5, [&]()->void {
		cBoss::NextStage();
	});
}
