#include "DXUT.h"
#include "cEnemy3.h"

void cEnemy3::Init()
{
	cEnemy::Init();
	AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Enemy3");
	GetComponent<cCollider>()->AddCollider(Vec2(0, 20), 20);
	GetComponent<cCollider>()->AddCollider(Vec2(0, -20), 20);
	GetComponent<cMapCollider>()->m_Radius = 25;
	m_Owner->m_Scale = Vec2(0.75, 0.75);

	m_Speed = 75;
	m_Hp = 70;

	m_Score = 300000;
}

void cEnemy3::Update()
{
	cEnemy::Update();
}

void cEnemy3::Render()
{
	cEnemy::Render();
}

void cEnemy3::Release()
{
	cEnemy::Release();
}

void cEnemy3::OnAlarm(string _Key)
{
	cEnemy::OnAlarm(_Key);
}

void cEnemy3::OnCollision(cObject * _Other)
{
	cEnemy::OnCollision(_Other);
}

void cEnemy3::Attack()
{
	if (Random(1, 10) <= 3)
		return;
	cBullet* a;
	for (int i = 0; i < 5; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			a = FireBullet(m_Owner->m_Pos, i * 72 + j * 10, 0.75, 8, 400, 0xff0000ff, Obj_EnemyBullet);
			a->m_EndSpeed = 125;
			a->m_Friction = 0.95;
		}
	}
	for (int i = 0; i < 5; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			a = FireBullet(m_Owner->m_Pos, 36 + i * 72 + j * 10, 0.75, 8, 30, 0xffff0000, Obj_EnemyBullet);
			a->m_EndSpeed = 125;
			a->m_Friction = 1.05;
		}
	}
}

void cEnemy3::Death()
{
	cEnemy::Death();

	for (int i = 0; i < 4; i++)
	{
		GAME->Explosion(m_Owner->m_Pos + RandomVector2() * 15, 1);
	}
	if (m_Hp <= 0)
	{
		for (int i = 0; i < 25; i++)
		{
			FireBullet(m_Owner->m_Pos, i * 24, 0.5, 10, 400, 0xff00ffff, Obj_PlayerBullet);
		}
	}
}

void cEnemy3::DealDamage(float _Damage)
{
	cEnemy::DealDamage(_Damage);
}
