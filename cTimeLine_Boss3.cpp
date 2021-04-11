#include "DXUT.h"
#include "cTimeLine_Boss3.h"
#include "cBoss3.h"

void cTimeLine_Boss3::Init()
{
	m_Boss = GetComponent<cBoss3>();
}

void cTimeLine_Boss3::Update()
{
	m_Time += DT * 10;
	if (m_Last != (int)m_Time)
	{
		m_Last = (int)m_Time;
		switch (m_Last)
		{
		case 1:
			m_Boss->m_ChainLengthVel = -500;
			m_Boss->m_RotEndSpeed = 0;
			m_Boss->m_RotFriction = 0.9;
			break;

		case 20:
			m_Boss->m_RotSpeed = 10;
			m_Boss->m_RotEndSpeed = 400;
			m_Boss->m_RotFriction = 1.1;
		break;

		case 50:
			m_Boss->m_RotEndSpeed = 0;
			m_Boss->m_RotFriction = 0.9;
		break;

		case 60:
		{
			m_Boss->m_ChainLengthVel = 500;

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
				m_Boss->m_Corner[Corner] = CurPos;
			}
		}

			break;

		case 70:
			m_Boss->m_RotSpeed = 10;
			m_Boss->m_RotEndSpeed = 150;
			m_Boss->m_RotFriction = 1.05;
			m_Time = 0;
			m_Enable = false;
			break;
		}
		
		if (m_Last >= 20)
		{
			cBullet* a;
			for (int i = 0; i < 6; i++)
			{
				a = FireBullet(m_Owner->m_Pos + RotateVector2(Vec2(130 * m_Owner->m_Scale.x, 0), m_Owner->m_Rot + i * 60), m_Owner->m_Rot + i * 60, 0.5, 10, 400, 0xffffff00, Obj_EnemyBullet);
				a->m_EndSpeed = 300;
				a->m_Friction = 0.99;
			}
		}
	}
}

void cTimeLine_Boss3::Render()
{
}

void cTimeLine_Boss3::Release()
{
}

void cTimeLine_Boss3::OnAlarm(string _Key)
{
}

void cTimeLine_Boss3::OnCollision(cObject * _Other)
{
}
