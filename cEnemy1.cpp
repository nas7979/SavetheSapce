#include "DXUT.h"
#include "cEnemy1.h"

void cEnemy1::Init()
{
	cEnemy::Init();
	AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Enemy1");
	GetComponent<cCollider>()->AddCollider(Vec2(0, 0), 20);
	GetComponent<cMapCollider>()->m_Radius = 15;
	m_Owner->m_Scale = Vec2(0.5, 0.5);
	
	m_Speed = 100;
	m_Hp = 15;

	m_Score = 100000;
}

void cEnemy1::Update()
{
	cEnemy::Update();
	if (m_Speed > 100)
	{
		cParticle* a = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, 0.7, Obj_Particle)->GetComponent<cParticle>();
		a->m_Alp = 200;
		a->m_AlpVel = -400;
		a->m_Owner->m_Rot = m_Owner->m_Rot;
		a->m_Owner->m_Scale = m_Owner->m_Scale;
		a->AddComponent<cRenderer>()->m_Sprite = GetComponent<cRenderer>()->m_Sprite;
		a->GetComponent<cRenderer>()->m_Color = 0xffff0000;
	}
}

void cEnemy1::Render()
{
	cEnemy::Render();
}

void cEnemy1::Release()
{
	cEnemy::Release();
}

void cEnemy1::OnAlarm(string _Key)
{
	cEnemy::OnAlarm(_Key);
	if (_Key == "Speed")
	{
		m_Speed = 100;
		return;
	}
}

void cEnemy1::OnCollision(cObject * _Other)
{
	cEnemy::OnCollision(_Other);
}

void cEnemy1::Attack()
{
	m_Speed = 150;
	m_Owner->SetAlarm("Speed", 2);

	for (int i = 0; i < 4; i++)
	{
		cBullet* a = FireBullet(m_Owner->m_Pos, m_Owner->m_Rot + i * 90, 0.5, 10, 30, 0xff00ff00, Obj_EnemyBullet);
		a->m_EndSpeed = 200;
		a->m_Friction = 1.03;
	}
}

void cEnemy1::Death()
{
	if (SCENE->m_CurKey != "INTRO")
	{
		cEnemy::Death();
		if (m_Hp <= 0)
		{
			for (int i = 0; i < 15; i++)
			{
				FireBullet(m_Owner->m_Pos, i * 24, 0.5, 10, 400, 0xff00ffff, Obj_PlayerBullet);
			}
		}
	}
	else
		m_Owner->m_isDestroyed = true;

	for (int i = 0; i < 2; i++)
	{
		GAME->Explosion(m_Owner->m_Pos + RandomVector2() * 10, 1);
	}

}

void cEnemy1::DealDamage(float _Damage)
{
	cEnemy::DealDamage(_Damage);
}
