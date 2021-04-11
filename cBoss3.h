#pragma once
class cBoss3 : public cBoss
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
	virtual void OnStop() override;

	Vec2 m_Corner[6];
	float m_RotSpeed;
	float m_RotEndSpeed;
	float m_RotFriction;
	float m_ChainLength;
	float m_ChainLengthVel;
};

