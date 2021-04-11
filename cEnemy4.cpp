#include "DXUT.h"
#include "cEnemy4.h"

void cEnemy4::Init()
{
	cEnemy::Init();
	AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Explosion16");
	GetComponent<cCollider>()->AddCollider(Vec2(0, 20), 40);
	GetComponent<cMapCollider>()->m_Radius = 30;
	m_Owner->m_Scale = Vec2(0.25, 0.25) * Random(1.f, 2.f);

	m_Speed = 30;
	m_Hp = 50;

	m_Score = 300000;
}

void cEnemy4::Update()
{
	cEnemy::Update();
	cParticle* a = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, m_Owner->m_Depth - 0.01, Obj_Particle)->GetComponent<cParticle>();
	a->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Smoke" + to_string(Random(1, 3)));
	a->GetComponent<cRenderer>()->m_Color = GetComponent<cRenderer>()->m_Color = 0xffffffff ? 0x90ff6060 : 0xffffffff;
	a->m_PosVel = RandomVector2() * 50;
	a->m_PosFri = 0.95;
	a->m_RotVel = Random(-200, 200);
	a->m_RotFri = 0.95;
	a->m_ScaVel = Vec2(-1, -1) * m_Owner->m_Scale.x;
	a->m_Owner->m_Scale = m_Owner->m_Scale * 0.5f;
	a->m_Owner->m_Rot = Random(0, 359);
}

void cEnemy4::Render()
{
}

void cEnemy4::Release()
{
}

void cEnemy4::OnAlarm(string _Key)
{
	cEnemy::Release();
}

void cEnemy4::OnCollision(cObject * _Other)
{
	cEnemy::OnCollision(_Other);
}

void cEnemy4::Attack()
{
	Vec2 PrevPos = m_Owner->m_Pos;
	do
	{
		m_Owner->m_Pos = Vec2(Random(OFFSET, MAPX), Random(OFFSET, MAPY));
	} while (GAME->GetCell(m_Owner->m_Pos) != Cell_Enemy);

	cBullet* a;
	for (int i = 0; i < 15; i++)
	{
		a = FireBullet(m_Owner->m_Pos, Random(0, 359), Random(0.25f, 0.5f), 10, Random(5.f, 15.f), 0xff0000ff, Obj_EnemyBullet);
		a->m_EndSpeed = 125;
		a->m_Friction = 1.02;
	}

	Vec2 Dir = Normalize(m_Owner->m_Pos - PrevPos);
	int Dist = Distance(m_Owner->m_Pos - PrevPos) * 0.1;
	cParticle* Part;
	for (int i = 0; i < Dist; i++)
	{
		Part = OBJECT->AddObject<cParticle>("Particle", PrevPos + Dir * i * 10, m_Owner->m_Depth - 0.01, Obj_Particle)->GetComponent<cParticle>();
		Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Smoke" + to_string(Random(1, 3)));
		Part->GetComponent<cRenderer>()->m_Color = 0x90ff6060;
		Part->m_PosVel = RandomVector2() * 25;
		Part->m_PosFri = 0.95;
		Part->m_RotVel = Random(-200, 200);
		Part->m_RotFri = 0.95;
		Part->m_ScaVel = Vec2(-0.5, -0.5) * m_Owner->m_Scale.x;
		Part->m_Owner->m_Scale = m_Owner->m_Scale * 0.25f;
		Part->m_Owner->m_Rot = Random(0, 359);
	}
}

void cEnemy4::Death()
{
	cEnemy::Death();

	cParticle* Part;
	for (int i = 0; i < 50; i++)
	{
		Part = OBJECT->AddObject<cParticle>("Particle",m_Owner->m_Pos, m_Owner->m_Depth - 0.01, Obj_Particle)->GetComponent<cParticle>();
		Part->AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Smoke" + to_string(Random(1, 3)));
		Part->GetComponent<cRenderer>()->m_Color = 0x90ff6060;
		Part->m_PosVel = RandomVector2() * 200;
		Part->m_PosFri = 0.95;
		Part->m_RotVel = Random(-200, 200);
		Part->m_RotFri = 0.95;
		Part->m_ScaVel = Vec2(-1, -1) * m_Owner->m_Scale.x;
		Part->m_Owner->m_Scale = m_Owner->m_Scale * 0.5;
		Part->m_Owner->m_Rot = Random(0, 359);
	}

	if (m_Hp <= 0)
	{
		for (int i = 0; i < 25; i++)
		{
			FireBullet(m_Owner->m_Pos, i * 24, 0.5, 10, 400, 0xff00ffff, Obj_PlayerBullet);
		}
	}
}

void cEnemy4::DealDamage(float _Damage)
{
	cEnemy::DealDamage(_Damage);
}
