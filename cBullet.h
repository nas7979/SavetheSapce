#pragma once
class cBullet : public cComponent
{
public:

	// cComponent을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;

	float m_Damage;
	float m_Speed;
	float m_Friction = 1;
	float m_EndSpeed = 0;
	float m_ReflectCounter = 0;
	Texture* m_BaseImage = nullptr;
	function<void(cObject* _This)> m_OnDestroy = nullptr;
};

