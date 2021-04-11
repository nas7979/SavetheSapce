#pragma once
class cParticle : public cComponent
{
public:

	// cComponent을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;
	void OnAnimation();

	Vec2 m_PosVel = Vec2(0, 0);
	float m_PosFri = 1;

	Vec2 m_ScaVel = Vec2(0, 0);
	float m_ScaFri = 1;

	float m_RotVel = 0;
	float m_RotFri = 1;

	float m_Alp = 255;
	float m_AlpVel = 0;
	float m_AlpFri = 1;
	
	float m_FuncTimer = 0;
	float m_FuncTime = 1000;
	function<void(cObject* _This)> m_Func = nullptr;
};

