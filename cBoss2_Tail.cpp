#include "DXUT.h"
#include "cBoss2_Tail.h"

void cBoss2_Tail::Init()
{
	cEnemy::Init();
	GetComponent<cMapCollider>()->m_Enable = false;
	AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Boss2_Tail");
	GetComponent<cCollider>()->AddCollider(Vec2(0, 0), 60);

	m_Hp = 100000000;
	m_Speed = 0;
}

void cBoss2_Tail::Update()
{
}

void cBoss2_Tail::Render()
{
}

void cBoss2_Tail::Release()
{
}

void cBoss2_Tail::OnAlarm(string _Key)
{
	cEnemy::OnAlarm(_Key);
}

void cBoss2_Tail::OnCollision(cObject * _Other)
{
	if (_Other->m_Tag != Obj_PlayerBullet)
		return;

	for (auto& iter : m_Head->m_Collided)
	{
		if (iter == _Other)
			return;
	}
	_Other->m_isDestroyed = true;
	m_Head->m_Collided.push_back(_Other);
	DealDamage(_Other->GetComponent<cBullet>()->m_Damage);
}

void cBoss2_Tail::Attack()
{
}

void cBoss2_Tail::Death()
{
	m_Owner->m_isDestroyed = true;
	for (int i = 0; i < m_Head->m_Size * 0.5 + 1; i++)
	{
		GAME->Explosion(m_Owner->m_Pos + RandomVector2() * m_Head->m_Size * 2, 1);
	}
}

void cBoss2_Tail::DealDamage(float _Damage)
{
	m_Head->m_Hp -= _Damage;
	m_Owner->SetAlarm("Damage", 0.075);
	SOUND->Play("MG " + to_string(Random(1, 14)));
	GetComponent<cRenderer>()->m_Color = 0xffff0000;
	if (m_Head->m_Hp <= 0)
	{
		m_Head->Death();
	}
}
