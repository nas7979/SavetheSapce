#include "DXUT.h"
#include "cCameraManager.h"

void cCameraManager::Init()
{
	m_Pos = m_PosTo = Vec2(240, 135);
	m_Scale = m_ScaleTo = 1;
	m_ShakePos = Vec2(0, 0);
	m_ShakeForce = 0;
}

void cCameraManager::Update()
{
	if (m_ShakeForce != 0)
	{
		m_ShakePos = Vec2(Random(-m_ShakeForce, m_ShakeForce), Random(-m_ShakeForce, m_ShakeForce));
		m_ShakeForce -= m_ShakeTime * DT;
		if (m_ShakeForce <= 0)
		{
			m_ShakeForce = 0;
			m_ShakePos = Vec2(0, 0);
		}
	}
	m_Pos = Lerp<Vec2>(m_Pos, m_PosTo, 0.1);
	m_Scale = Lerp<float>(m_Scale, m_ScaleTo, 0.1);
}

void cCameraManager::Render()
{
}

void cCameraManager::Release()
{
}

void cCameraManager::Shake(float _Force, float _Time)
{
	m_ShakeForce = _Force;
	m_ShakeTime = _Force / _Time;
}

void cCameraManager::Move(Vec2 _To)
{
	m_PosTo = _To;
}

void cCameraManager::Scale(float _To)
{
	m_ScaleTo = _To;
}
