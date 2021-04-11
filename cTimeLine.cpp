#include "DXUT.h"
#include "cTimeLine.h"


cBullet * cTimeLine::FireBullet(Vec2 _Pos, float _Dir, float _Size, float _Damage, float _Speed, D3DCOLOR _Color, ObjectTag _Tag)
{
	cBullet* a = OBJECT->AddObject<cBullet>("Bullet", _Pos, 0.4, _Tag)->GetComponent<cBullet>();
	a->m_Damage = _Damage;
	a->m_Speed = _Speed;
	a->m_Owner->m_Rot = _Dir;
	a->m_Owner->m_Scale = Vec2(_Size, _Size);
	a->GetComponent<cRenderer>()->m_Color = _Color;
	return a;
}