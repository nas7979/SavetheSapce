#include "DXUT.h"
#include "cTimeLine_Boss1.h"

void cTimeLine_Boss1::Init()
{
}

void cTimeLine_Boss1::Update()
{
	m_Time += DT * 10;
	if (m_Last != (int)m_Time)
	{
		m_Last = (int)m_Time;
		switch (m_Last)
		{
		case 10:
		case 12:
		case 14:
		case 16:
			for (int i = 0; i < 18; i++)
			{
				FireBullet(m_Owner->m_Pos, m_Dir + i * 20, 1, 10, 200, 0xffff0000, Obj_EnemyBullet);
			}
			m_Dir += 8;
			break;

		case 30:
		case 32:
		case 34:
		case 36:
			for (int i = 0; i < 18; i++)
			{
				FireBullet(m_Owner->m_Pos, m_Dir + i * 20, 1, 10, 200, 0xffff0000, Obj_EnemyBullet);
			}
			m_Dir -= 8;
			break;

		case 40:
			m_Time = 0;
			m_Dir = 0;
			m_Enable = false;
			break;
		}
	}
}

void cTimeLine_Boss1::Render()
{
}

void cTimeLine_Boss1::Release()
{
}

void cTimeLine_Boss1::OnAlarm(string _Key)
{
}

void cTimeLine_Boss1::OnCollision(cObject * _Other)
{
}
