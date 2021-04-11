#include "DXUT.h"
#include "cParticle.h"

void cParticle::Init()
{
}

void cParticle::Update()
{
	m_PosVel *= pow(m_PosFri, DT * 60);
	m_ScaVel *= pow(m_ScaFri, DT * 60);
	m_RotVel *= pow(m_RotFri, DT * 60);
	m_AlpVel *= pow(m_AlpFri, DT * 60);

	m_Owner->m_Pos += m_PosVel * DT;
	m_Owner->m_Scale += m_ScaVel * DT;
	m_Owner->m_Rot += m_RotVel * DT;
	m_Alp += m_AlpVel * DT;

	cRenderer* Renderer = GetComponent<cRenderer>();
	if (Renderer)
	{
		Renderer->m_Color = (Renderer->m_Color & 0x00ffffff) | ((int)Clamp(m_Alp, 0, 255) << 24);
	}

	m_FuncTimer -= DT;
	if (m_Func)
	{
		if (m_FuncTimer <= 0)
		{
			m_Func(m_Owner);
			m_FuncTimer = m_FuncTime;
		}
	}

	if (m_Owner->m_Scale.x <= 0 || m_Owner->m_Scale.y <= 0 || m_Alp <= 0)
	{
		m_Owner->m_isDestroyed = true;
	}
}

void cParticle::Render()
{
}

void cParticle::Release()
{
}

void cParticle::OnAlarm(string _Key)
{
	if (_Key == "Destroy")
	{
		m_Owner->m_isDestroyed = true;
		return;
	}
}

void cParticle::OnCollision(cObject * _Other)
{
}

void cParticle::OnAnimation()
{
	GetComponent<cRenderer>()->m_Enable = false;
	m_Owner->m_isDestroyed = true;
}
