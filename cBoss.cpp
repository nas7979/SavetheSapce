#include "DXUT.h"
#include "cBoss.h"
#include "cEnemy1.h"
#include "cEnemy2.h"
#include "cEnemy3.h"
#include "cEnemy4.h"
#include "cScore.h"

void cBoss::Init()
{
	cEnemy::Init();
	m_Owner->SetAlarm("Move", 0);
	m_Size = 10;
	OBJECT->m_Boss = m_Owner;
}

void cBoss::Update()
{
	cEnemy::Update();

	if (GAME->m_Percentage >= 80)
	{
		NextStage();
	}
}

void cBoss::Render()
{
	cEnemy::Render();

	IMAGE->Render(IMAGE->Find("HpBar"), Vec2(70, 10), 0, Vec2((m_Hp / m_MaxHp) * 650, 1), 0.1, Vec2(0, 0.5), 0xffff0000);
	IMAGE->RenderText("BOSS", Vec2(20, 10), 0, Vec2(0.25, 0.25), 0.1, Vec2(0, 0.5), 0xffff0000);
}

void cBoss::Release()
{
	cEnemy::Release();
	OBJECT->m_Boss = nullptr;
}

void cBoss::OnAlarm(string _Key)
{
	cEnemy::OnAlarm(_Key);
	if (_Key == "Stop")
	{
		OnStop();
		m_Move = false;
		m_Owner->SetAlarm("Move", m_StopTime);
		return;
	}

	if (_Key == "Move")
	{
		m_Move = true;
		m_Owner->SetAlarm("Stop", Random(5.f, 10.f));
		return;
	}
}

void cBoss::OnCollision(cObject * _Other)
{
	cEnemy::OnCollision(_Other);
}

void cBoss::Attack()
{
}

void cBoss::Death()
{
	if (m_Size == 1)
		return;
	m_Size--;
	m_Hp = m_MaxHp;
	m_Owner->m_Scale *= 0.9;
	for (int i = 0; i < m_Size; i++)
	{
		GAME->Explosion(m_Owner->m_Pos + RotateVector2(Vec2(Random(-5.f, 5.f) * m_Size, 0), Random(0, 359)), Random(0.1f, 0.15f) * m_Size);
	}
	SOUND->Play("Destruction " + to_string(Random(1, 5)));
	GAME->m_TimeScale = 0;
	for (float i = 0; i < 100; i++)
	{
		SCHEDULE->AddSchedule(0.2 + i * 0.01, []()->void {
			GAME->m_TimeScale += 0.01;
			if (GAME->m_TimeScale >= 1)
				GAME->m_TimeScale = 1;
		});
	}

	for (auto& iter : OBJECT->m_Objects[Obj_PlayerBullet])
	{
		iter->m_isDestroyed = true;
	}

	int Count = m_RespawnQueue.size() * (1 - GAME->m_Percentage * 0.01f) * 0.75f;
	for (int i = 0; i < Count; i++)
	{
		if (m_RespawnQueue.front() == "Enemy1")
			OBJECT->AddObject<cEnemy1>("Enemy1", m_Owner->m_Pos, 0.55, Obj_Enemy);
		if (m_RespawnQueue.front() == "Enemy2")
			OBJECT->AddObject<cEnemy2>("Enemy2", m_Owner->m_Pos, 0.55, Obj_Enemy);
		if (m_RespawnQueue.front() == "Enemy3")
			OBJECT->AddObject<cEnemy3>("Enemy3", m_Owner->m_Pos, 0.55, Obj_Enemy);
		if (m_RespawnQueue.front() == "Enemy4")
			OBJECT->AddObject<cEnemy4>("Enemy4", m_Owner->m_Pos, 0.55, Obj_Enemy);
		m_RespawnQueue.pop();
	}
}

void cBoss::DealDamage(float _Damage)
{
	cEnemy::DealDamage(_Damage);
}

void cBoss::NextStage()
{
	OBJECT->m_Stop = true;

	for (auto& iter : OBJECT->m_Objects[Obj_Item])
	{
		iter->m_isDestroyed = true;
	}
	for (auto& iter : OBJECT->m_Objects[Obj_Enemy])
	{
		if (iter == m_Owner || iter->m_isDestroyed == true)
			continue;
		SCHEDULE->AddSchedule(Random(0.f, 1.5f), [=]()->void {
			iter->GetComponent<cEnemy>()->m_Hp = 100000;
			iter->GetComponent<cEnemy>()->Death();
		});
	}
	for (auto& iter : OBJECT->m_Objects[Obj_Map])
	{
		if (iter == m_Owner)
			continue;
		SCHEDULE->AddSchedule(Random(0.f, 1.5f), [=]()->void {
			iter->m_isDestroyed = true;
		});
	}

	SCHEDULE->AddSchedule(1.5f, []()->void {
		for (auto& iter : OBJECT->m_Objects[Obj_EnemyBullet])
		{
			iter->m_isDestroyed = true;
		}
		for (auto& iter : OBJECT->m_Objects[Obj_PlayerBullet])
		{
			iter->m_isDestroyed = true;
		}
	});

	SCHEDULE->AddSchedule(2.5f, [&]()->void {
		for (float i = 0; i < 200; i++)
		{
			SCHEDULE->AddSchedule(0.03 * i, [=]()->void {
				GAME->Explosion(m_Owner->m_Pos + RotateVector2(Vec2(Random(0.f, i * 7), 0), Random(0, 359)), i * 0.04, false);
				CAMERA->Shake(i * 0.05, 0.5);
			});
		}
	});

	SCHEDULE->AddSchedule(7.5f, [&]()->void {
		CAMERA->Move(m_Owner->m_Pos);
		CAMERA->Scale(2);
	});

	SCHEDULE->AddSchedule(9.f, [&]()->void {
		for (int i = 0; i < 10; i++)
		{
			GAME->Explosion(m_Owner->m_Pos + RotateVector2(Vec2(Random(0, m_Size * 5), 0), Random(0, 359)), 1 + m_Size * 0.1, false);
		}
		SOUND->Play("Destruction 1");
		SOUND->Play("Destruction 2");
		SOUND->Play("Destruction 3");
		m_Owner->m_isDestroyed = true;
		CAMERA->Shake(15, 2);
	});

	SCHEDULE->AddSchedule(11, []()->void {
		CAMERA->Move(Vec2(240, 135));
		CAMERA->Scale(1);
	});

	SCHEDULE->AddSchedule(12, []()->void {
		for (int i = 0; i < 500; i++)
		{
			SCHEDULE->AddSchedule(Random(0.f, 3.f), [=]()->void {
				IMAGE->BeginSurface("Ground");
				IMAGE->Render(IMAGE->Find("Paint" + to_string(Random(1, 3))), Vec2(OFFSET + MAPX * 0.5, OFFSET + MAPY * 0.5) + RandomVector2() * i * 2, Random(0, 359), Vec2(0.1, 0.1) + Vec2(0.002, 0.002) * i, 0.6974);
				IMAGE->EndSurface();
			});
		}
	});
	SCHEDULE->AddSchedule(15, [=]()->void {
		IMAGE->BeginSurface("Ground");
		IMAGE->Render(IMAGE->Find("Paint" + to_string(Random(1, 3))), Vec2(0, 0), 0, Vec2(1000, 1000), 0.6974);
		IMAGE->EndSurface();
	});

	SCHEDULE->AddSchedule(16, []()->void {
		cObject* a = OBJECT->AddObject<cScore>("Score", Vec2(0, 0), 0, Obj_None);
	});
}
