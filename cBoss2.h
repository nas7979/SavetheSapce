#pragma once
struct TailPos
{
	Vec2 Pos;
	float Rot;
};
class cBoss2 : public cBoss
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
	virtual void NextStage() override;

	vector<cObject*> m_Collided;
	vector<cObject*> m_Tails;
	list<TailPos*> m_TailPos;
};

