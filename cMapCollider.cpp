#include "DXUT.h"
#include "cMapCollider.h"

void cMapCollider::Init()
{
	m_PrevPos = m_Owner->m_Pos;
}

void cMapCollider::Update()
{
	float Dist = ceil(Distance(m_PrevPos - m_Owner->m_Pos));
	float Radius = D3DX_PI * 2 * m_Radius;
	float RotPerIter = D3DXToRadian(360 / Radius);
	Vec2 CurPos;
	Vec2 Dir = Normalize(m_Owner->m_Pos - m_PrevPos);
	for (int Pos = 1; Pos <= Dist; Pos++)
	{
		for (int i = 0; i < Radius; i++)
		{
			CurPos = m_PrevPos + Dir * Pos + Vec2(cos(RotPerIter * i), sin(RotPerIter * i)) * m_Radius * m_Owner->m_Scale.x;
			if (GAME->GetCell(CurPos) != Cell_Enemy)
			{
				m_OnCollision(m_PrevPos, CurPos, GAME->GetCell(CurPos));
				goto Out;
			}
		}
	}
Out:
	m_PrevPos = m_Owner->m_Pos;
}

void cMapCollider::Render()
{
}

void cMapCollider::Release()
{
}

void cMapCollider::OnAlarm(string _Key)
{
}

void cMapCollider::OnCollision(cObject * _Other)
{
}
