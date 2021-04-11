#include "DXUT.h"
#include "cEnemy2.h"

void cEnemy2::Init()
{
	cEnemy::Init();
	AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Enemy2");
	GetComponent<cCollider>()->AddCollider(Vec2(-20, 0), 20);
	GetComponent<cCollider>()->AddCollider(Vec2(20, 0), 20);
	GetComponent<cMapCollider>()->m_Radius = 20;
	m_Owner->m_Scale = Vec2(0.5, 0.5);

	m_Speed = 90;
	m_Hp = 40;

	m_Score = 200000;
}

void cEnemy2::Update()
{
	cEnemy::Update();
}

void cEnemy2::Render()
{
	cEnemy::Render();
}

void cEnemy2::Release()
{
	cEnemy::Release();
}

void cEnemy2::OnAlarm(string _Key)
{
	cEnemy::OnAlarm(_Key);
}

void cEnemy2::OnCollision(cObject * _Other)
{
	cEnemy::OnCollision(_Other);
}

void cEnemy2::Attack()
{
	if (Random(1, 10) <= 5)
		return;
	float Dir = Direction(OBJECT->m_Player->m_Pos - m_Owner->m_Pos);
	for (int i = 0; i < 10; i++)
	{
		FireBullet(m_Owner->m_Pos + RotateVector2(Vec2(10, 0), i * 36), Dir, 1, 7, 150, 0xff0000ff, Obj_EnemyBullet);
	}
}

void cEnemy2::Death()
{
	cEnemy::Death();

	for (int i = 0; i < 4; i++)
	{
		GAME->Explosion(m_Owner->m_Pos + RandomVector2() * 15, 1);
	}
	if (m_Hp <= 0)
	{
		for (int i = 0; i < 20; i++)
		{
			FireBullet(m_Owner->m_Pos, i * 24, 0.5, 10, 400, 0xff00ffff, Obj_PlayerBullet);
		}
	}
}

void cEnemy2::DealDamage(float _Damage)
{
	cEnemy::DealDamage(_Damage);
}
