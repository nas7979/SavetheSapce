#pragma once
struct Collider
{
	Vec2 LocalPos;
	Vec2 WorldPos;
	float Radius;
	float OriginalRadius;
};
class cCollider : public cComponent
{
public:

	// cComponent을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;

	vector<Collider*> m_Colliders;
	vector<ObjectTag> m_CollWith;

	void AddCollider(Vec2 _Pos, float _Radius);
	void AddCollWith(ObjectTag _Tag);
};

