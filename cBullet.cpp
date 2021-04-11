#include "DXUT.h"
#include "cBullet.h"

void cBullet::Init()
{
	m_BaseImage = IMAGE->Find("Bullet_Base");
	AddComponent<cRenderer>()->m_Sprite = IMAGE->Find("Bullet_Color");
	AddComponent<cCollider>()->AddCollider(Vec2(0, 0), 6);
	AddComponent<cMapCollider>()->m_Radius = 1;
	GetComponent<cMapCollider>()->m_OnCollision = [&](Vec2 _Pos, Vec2 _CellPos, short _Cell)->void {
		switch (_Cell)
		{
		case Cell_Player:
			if (m_Owner->m_Tag == Obj_EnemyBullet)
				m_Owner->m_isDestroyed = true;
		break;

		case Cell_Inside:
		{
			if (m_Owner->m_Tag != Obj_EnemyBullet)
				return;
			if (m_ReflectCounter <= 3)
			{
				m_ReflectCounter = 0;
				return;
			}
			m_Owner->m_isDestroyed = true;
			for (int i = 0; i < 3; i++)
			{
				cBullet* a = OBJECT->AddObject<cBullet>("PlayerBullet", m_Owner->m_Pos, 0.45, Obj_PlayerBullet)->GetComponent<cBullet>();
				a->m_Damage = m_Damage;
				a->m_Speed = m_Friction == 1 ? m_Speed : m_EndSpeed;
				a->m_Owner->m_Rot = m_Owner->m_Rot + 180 + Random(-15.f, 15.f);
				a->GetComponent<cRenderer>()->m_Color = 0xff00ffff;
				a->GetComponent<cMapCollider>()->m_Enable = false;
				a->m_Owner->m_Scale = m_Owner->m_Scale * 0.75;
			}
		}
		break;
		}
	};
}

void cBullet::Update()
{
	m_ReflectCounter++;
	m_Owner->m_Pos += RotateVector2(Vec2(m_Speed * DT, 0), m_Owner->m_Rot);
	m_Speed *= pow(m_Friction, DT * 60);
	if (m_Friction > 1)
	{
		if (m_Speed >= m_EndSpeed)
		{
			m_Speed = m_EndSpeed;
			m_Friction = 1;
		}
	}
	if (m_Friction < 1)
	{
		if (m_Speed <= m_EndSpeed)
		{
			m_Speed = m_EndSpeed;
			m_Friction = 1;
		}
	}
}

void cBullet::Render()
{
	IMAGE->Render(m_BaseImage, Vec2((int)m_Owner->m_Pos.x, (int)m_Owner->m_Pos.y), m_Owner->m_Rot, m_Owner->m_Scale, m_Owner->m_Depth);
}

void cBullet::Release()
{
	if (m_OnDestroy)
	{
		m_OnDestroy(m_Owner);
	}
	else
	{
		cParticle* a;
		for (int i = 0; i < m_Owner->m_Scale.x * 15; i++)
		{
			a = OBJECT->AddObject<cParticle>("Particle", m_Owner->m_Pos, 0.3, Obj_Particle)->GetComponent<cParticle>();
			a->AddComponent<cRenderer>()->m_Sprite = m_BaseImage;
			a->GetComponent<cRenderer>()->m_Color = GetComponent<cRenderer>()->m_Color;
			a->m_PosVel = RandomVector2() * 200 * m_Owner->m_Scale.x;
			a->m_PosFri = 0.97;
			a->m_ScaVel = Vec2(-0.5, -0.5) * m_Owner->m_Scale.x;
			a->m_ScaFri = 1.04;
			a->m_Owner->m_Scale = Vec2(0.75, 0.75) * m_Owner->m_Scale.x;
		}
	}
}

void cBullet::OnAlarm(string _Key)
{
	if (_Key == "Destroy")
	{
		m_Owner->m_isDestroyed = true;
		return;
	}
}

void cBullet::OnCollision(cObject * _Other)
{
}
