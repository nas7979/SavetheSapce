#include "DXUT.h"
#include "cTimeLine_Boss2.h"

void cTimeLine_Boss2::Init()
{
}

void cTimeLine_Boss2::Update()
{
	m_Time += DT * 10;
	if (m_Last != (int)m_Time)
	{
		m_Last = (int)m_Time;
		switch (m_Last)
		{
		case 20:
			m_Dir = Direction(OBJECT->m_Player->m_Pos - m_Owner->m_Pos);
			FireBullet(m_Owner->m_Pos, m_Dir, 2, 30, 150, 0xffffff00, Obj_EnemyBullet);
			break;

		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
		{
			float Dir = m_Dir + Random(-12.f, 12.f);
			for (int i = -3; i <= 3; i++)
			{
				FireBullet(m_Owner->m_Pos, Dir + i * 2, 1, 7, 400, 0xffff0000, Obj_EnemyBullet);
			}
		}
			break;

		case 50:
			m_Time = 0;
			m_Dir = 0;
			m_Enable = false;
			break;
		}
	}
}

void cTimeLine_Boss2::Render()
{
}

void cTimeLine_Boss2::Release()
{
}

void cTimeLine_Boss2::OnAlarm(string _Key)
{
}

void cTimeLine_Boss2::OnCollision(cObject * _Other)
{
}
