#pragma once
class cTimeLine : public cComponent
{
public:

	// cComponent을(를) 통해 상속됨
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
	virtual void OnAlarm(string _Key) = 0;
	virtual void OnCollision(cObject * _Other) = 0;

	float m_Time = 0;
	float m_Dir = 0;
	int m_Last = -1;

	cBullet* FireBullet(Vec2 _Pos, float _Dir, float _Size, float _Damage, float _Speed, D3DCOLOR _Color, ObjectTag _Tag);
};

