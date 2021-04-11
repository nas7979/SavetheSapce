#include "DXUT.h"
#include "cBoss3.h"
#include "cTimeLine_Boss3.h"

void cBoss3::Init()
{
	cBoss::Init();
	AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Boss3");
	GetComponent<cMapCollider>()->m_Radius = 100;
	GetComponent<cCollider>()->AddCollider(Vec2(0, 0), 150);
	AddComponent<cTimeLine_Boss3>()->m_Enable = false;
	m_Owner->m_Scale = Vec2(0.25, 0.25);

	m_Hp = m_MaxHp = 70;
	m_Speed = 100;
	m_StopTime = 7.1;
	m_RotSpeed = 100;
	m_RotFriction = 1;
	m_ChainLength = 1000;
	m_ChainLengthVel = 0;

	Vec2 Dir;
	Vec2 CurPos;
	for (int Corner = 0; Corner < 6; Corner++)
	{
		CurPos = m_Owner->m_Pos + RotateVector2(Vec2(130 * m_Owner->m_Scale.x, 0), m_Owner->m_Rot + Corner * 60);
		Dir = RotateVector2(Vec2(1, 0), m_Owner->m_Rot + Corner * 60);
		do
		{
			CurPos += Dir * 7;
		} while (GAME->GetCell(CurPos) <= 1);
		m_Corner[Corner] = CurPos;
	}
	SCENE->m_NextScene = "RANKING";

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

void cBoss3::Update()
{
	m_WaitForAttack++;
	if (m_Move)
	{
		if (OBJECT->m_Player->GetComponent<cPlayer>()->GetIsInvincible() == false)
		{
			m_Gravity = Normalize(OBJECT->m_Player->m_Pos - m_Owner->m_Pos);
		}
		m_Owner->m_Pos += m_Dir * m_RotSpeed * DT * 1.5;
		m_Dir += m_Gravity * DT * 0.5;
		D3DXVec2Normalize(&m_Dir, &m_Dir);
	}
	cBullet* a;
	Vec2 Dir;
	Vec2 CurPos;
	for (int Corner = 0; Corner < 6; Corner++)
	{
		CurPos = m_Owner->m_Pos + RotateVector2(Vec2(130 * m_Owner->m_Scale.x, 0), m_Owner->m_Rot + Corner * 60);
		Dir = Normalize(m_Corner[Corner] - CurPos);
		for (int i = 0; i < m_ChainLength; i += 7)
		{
			a = FireBullet(CurPos, Random(0, 359), 0.5, 5, 0, 0xffffff00, Obj_EnemyBullet);
			a->m_OnDestroy = [](cObject* _This)->void {};
			a->m_Owner->SetAlarm("Destroy", 0.02);
			a->m_Owner->m_Depth = 0.7;
			CurPos += Dir * 7;
			if (GAME->GetCell(CurPos) > 1)
				break;
		}
	}
	m_ChainLength = Clamp(m_ChainLength + m_ChainLengthVel * DT, 0, 1000);
	if (GAME->m_Percentage >= 80)
	{
		NextStage();
	}

	m_RotSpeed *= pow(m_RotFriction, 60 * DT);
	if (m_RotFriction > 1)
	{
		if (m_RotSpeed >= m_RotEndSpeed)
		{
			m_RotSpeed = m_RotEndSpeed;
			m_RotFriction = 1;
		}
	}
	if (m_RotFriction < 1)
	{
		if (m_RotSpeed <= m_RotEndSpeed)
		{
			m_RotSpeed = m_RotEndSpeed;
			m_RotFriction = 1;
		}
	}
	m_Owner->m_Rot += m_RotSpeed * DT;
}

void cBoss3::Render()
{
	cBoss::Render();
}

void cBoss3::Release()
{
	cBoss::Release();
}

void cBoss3::OnAlarm(string _Key)
{
	cBoss::OnAlarm(_Key);
}

void cBoss3::OnCollision(cObject * _Other)
{
	cBoss::OnCollision(_Other);
}

void cBoss3::Attack()
{
	cBullet* a;
	Vec2 Dir;
	Vec2 CurPos;
	for (int Corner = 0; Corner < 6; Corner++)
	{
		CurPos = m_Owner->m_Pos + RotateVector2(Vec2(130 * m_Owner->m_Scale.x, 0), m_Owner->m_Rot + Corner * 60);
		Dir = Normalize(m_Corner[Corner] - CurPos);
		do
		{
			if (Random(1, 7) == 1)
			{
				a = FireBullet(CurPos, Random(0, 359), 0.5, 5, 10, 0xffffff00, Obj_EnemyBullet);
				a->m_EndSpeed = 200;
				a->m_Friction = 1.05;
			}
			CurPos += Dir * 7;
		} while (GAME->GetCell(CurPos) <= 1);
	}
}

void cBoss3::Death()
{
	cBoss::Death();
}

void cBoss3::DealDamage(float _Damage)
{
	cBoss::DealDamage(_Damage);
}

void cBoss3::OnStop()
{
	GetComponent<cTimeLine_Boss3>()->m_Enable = true;
}
