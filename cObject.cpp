#include "DXUT.h"
#include "cObject.h"

void cObject::Init()
{
}

void cObject::Update()
{
	for (auto&iter : m_Components)
	{
		if (iter->m_Enable)
			iter->Update();
	}
	for (auto& iter = m_Alarms.begin(); iter != m_Alarms.end();)
	{
		(*iter).second -= DT;
		if ((*iter).second <= 0)
		{
			for (auto& Comp : m_Components)
			{
				Comp->OnAlarm((*iter).first);
			}
			iter = m_Alarms.erase(iter);
		}
		else
			iter++;
	}
}

void cObject::Render()
{
	for (auto&iter : m_Components)
	{
		if (iter->m_Enable)
			iter->Render();
	}
}

void cObject::Release()
{
	for (auto&iter : m_Components)
	{
		iter->Release();
		delete iter;
	}
}

void cObject::SetAlarm(string _Key, float _Time)
{
	m_Alarms.push_back(make_pair(_Key, _Time));
}
