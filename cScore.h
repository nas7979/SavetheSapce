#pragma once
class cScore : public cComponent
{
public:

	// cComponent을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;

	list<int> m_Bonuses;
	int m_TargetBonus;
	int m_CurBonus;
	int m_Bonus;
	float m_Alpha;
	bool m_BonusEnd;

	void AddBonus();
};

