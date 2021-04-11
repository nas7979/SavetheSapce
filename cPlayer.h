#pragma once
class cPlayer : public cComponent
{
public:

	// cComponent을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;

	Vec2 m_LastAngle;
	float m_Speed;
	int m_Life;
	bool m_Armor;
	float m_OriginalSpeed;
	float m_Invincible;
	float m_Shield;
	bool m_Retreat;
	Vec2 m_RespawnPos;
	bool m_Dead;
	bool m_Glow;
	bool m_CheatInv;
	vector<Vec2> m_DrawingCells;
	list<Vec2> m_Outlines;
	D3DCOLOR m_ShieldColor;
	
	void Fill(Vec2 _Pos);
	short BossFill(Vec2 _Pos, Vec2 _BossPos);
	bool GetIsInvincible();
	void ResetItem();
};

