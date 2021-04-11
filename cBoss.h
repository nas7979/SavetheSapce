#pragma once
#include "cEnemy.h"
class cBoss : public cEnemy
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;
	virtual void Attack() = 0;
	virtual void Death();
	virtual void DealDamage(float _Damage);
	virtual void OnStop() = 0;
	virtual void NextStage();

	float m_StopTime;
	float m_MaxHp;
	int m_Size;
	queue<string> m_RespawnQueue;
};

