#include "DXUT.h"
#include "cAnimation.h"

void cAnimation::Init()
{
	m_Frame = 0;
	m_Last = -1;
	m_Speed = 1;
	m_CurAnim = nullptr;
}

void cAnimation::Update()
{
	if (m_CurAnim)
	{
		m_Frame += m_Speed * m_CurAnim->Speed * DT;
		if (m_Frame >= m_CurAnim->MaxFrame)
		{
			m_Frame -= m_CurAnim->MaxFrame;
			if (GetComponent<cParticle>() != nullptr)
			{
				GetComponent<cParticle>()->OnAnimation();
			}
		}
		if (m_Last != (int)m_Frame)
		{
			m_Last = (int)m_Frame;
			GetComponent<cRenderer>()->m_Sprite = (*m_CurAnim->Anim)[m_Last];
		}
	}
}

void cAnimation::Render()
{
}

void cAnimation::Release()
{
	for (auto& iter : m_Animations)
	{
		delete iter.second;
	}
}

void cAnimation::OnAlarm(string _Key)
{
}

void cAnimation::OnCollision(cObject * _Other)
{
}

void cAnimation::AddAnimation(string _Key, string _Path, float _Speed)
{
	Animation* a = new Animation;
	a->Key = _Key;
	a->Speed = 1 / _Speed;
	a->Anim = IMAGE->FindAnimation(_Path);
	a->MaxFrame = a->Anim->size();
	m_Animations[_Key] = a;
	m_CurAnim = a;
	GetComponent<cRenderer>()->m_Sprite = (*m_CurAnim->Anim)[0];
}

void cAnimation::SetAnimation(string _Key)
{
}
