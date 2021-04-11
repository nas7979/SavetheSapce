#pragma once
class cEnemy1 : public cEnemy
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;
	virtual void Attack() override;
	virtual void Death();
	virtual void DealDamage(float _Damage);
};

