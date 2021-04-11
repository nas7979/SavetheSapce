#pragma once
class cEnemy : public cComponent
{
public:

	// cComponent을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;
	virtual void Attack() = 0;
	virtual void Death();
	virtual void DealDamage(float _Damage);

	float m_Hp;
	float m_Score;
	float m_Speed;
	bool m_Move;
	Vec2 m_Dir;
	Vec2 m_Gravity;
	int m_WaitForAttack;

	cBullet* FireBullet(Vec2 _Pos, float _Dir, float _Size, float _Damage, float _Speed, D3DCOLOR _Color, ObjectTag _Tag);
};

