#pragma once
#include "cComponent.h"
enum ObjectTag
{
	Obj_Player,
	Obj_Enemy,
	Obj_PlayerBullet,
	Obj_EnemyBullet,
	Obj_Item,
	Obj_Map,
	Obj_Particle,
	Obj_None,
	Obj_Back,
	Obj_End
};
class cObject
{
public:
	void Init();
	void Update();
	void Render();
	void Release();

	Vec2 m_Pos;
	Vec2 m_Scale = Vec2(1, 1);
	float m_Rot = 0;
	string m_Name;
	ObjectTag m_Tag;
	float m_Depth;
	bool m_isDestroyed = false;
	list<pair<string, float>> m_Alarms;
	list<cComponent*> m_Components;

	template<typename T>
	T* AddComponent()
	{
		T* a = new T;
		a->m_Owner = this;
		m_Components.push_back(a);
		a->Init();
		return a;
	}

	template<typename T>
	T* GetComponent()
	{
		for (auto& iter : m_Components)
		{
			if (typeid(T*) == typeid(iter))
			{
				return static_cast<T*>(iter);
			}
		}

		T* Cast = nullptr;
		for (auto& iter : m_Components)
		{
			Cast = dynamic_cast<T*>(iter);
			if (Cast != nullptr)
			{
				return Cast;
			}
		}
		return nullptr;
	}

	void SetAlarm(string _Key, float _Time);
};

