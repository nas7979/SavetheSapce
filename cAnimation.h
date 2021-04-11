#pragma once
struct Animation
{
	float Speed;
	int MaxFrame;
	vector<Texture*>* Anim;
	string Key;
};
class cAnimation : public cComponent
{
public:

	// cComponent을(를) 통해 상속됨
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;
	virtual void OnAlarm(string _Key) override;
	virtual void OnCollision(cObject * _Other) override;

	int m_Last;
	float m_Frame;
	float m_Speed;
	Animation* m_CurAnim;
	map<string, Animation*> m_Animations;

	void AddAnimation(string _Key, string _Path, float _Speed);
	void SetAnimation(string _Key);
};

