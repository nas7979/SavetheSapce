#include "DXUT.h"
#include "cCollider.h"

void cCollider::Init()
{
}

void cCollider::Update()
{
	if (m_Owner->m_isDestroyed)
		return;
	for (auto& iter : m_Colliders)
	{
		iter->WorldPos = m_Owner->m_Pos + RotateVector2(iter->LocalPos * m_Owner->m_Scale.x, m_Owner->m_Rot);
		iter->Radius = iter->OriginalRadius * m_Owner->m_Scale.x;
	}

	cCollider* Other;
	for (auto& Tag : m_CollWith)
	{
		for (auto& iter : OBJECT->m_Objects[Tag])
		{
			Other = iter->GetComponent<cCollider>();
			if (Other->m_Enable == false || iter->m_isDestroyed)
				continue;
			for (auto& SelfColl : m_Colliders)
			{
				for (auto& OtherColl : Other->m_Colliders)
				{
					if (Distance(SelfColl->WorldPos - OtherColl->WorldPos) <= SelfColl->Radius + OtherColl->Radius)
					{
						for (auto& Comp : m_Owner->m_Components)
						{
							Comp->OnCollision(iter);
						}
						goto Next;
					}
				}
			}
		Next:
			continue;
		}
	}
}

void cCollider::Render()
{
}

void cCollider::Release()
{
	for (auto& iter : m_Colliders)
	{
		delete iter;
	}
}

void cCollider::OnAlarm(string _Key)
{
}

void cCollider::OnCollision(cObject * _Other)
{
}

void cCollider::AddCollider(Vec2 _Pos, float _Radius)
{
	Collider* a = new Collider;
	a->LocalPos = _Pos;
	a->WorldPos = m_Owner->m_Pos + RotateVector2(_Pos, m_Owner->m_Rot);
	a->OriginalRadius = _Radius;
	a->Radius = _Radius * m_Owner->m_Scale.x;
	m_Colliders.push_back(a);
}

void cCollider::AddCollWith(ObjectTag _Tag)
{
	m_CollWith.push_back(_Tag);
}
